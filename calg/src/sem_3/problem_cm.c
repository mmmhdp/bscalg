#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*xcmp_t) (void *lhs, int lsz, void *rhs, int rsz);

int
cmp (void *lhs, int lsz, void *rhs, int rsz)
{
  int *plhs, *prhs;
  plhs = (int *)lhs;
  prhs = (int *)rhs;

  return *plhs - *prhs;
}

void
xmerge (void *mem, int *sizes, int il, int im, int ir, xcmp_t cmp,
        char *membuff, int *memsizes_buff, int *offsets)
{
  char *lhs, *rhs, *tmp_membuff;
  int i, ibegin, ipivot, is_left_exhausted, current_buffer_window_size,
      current_window_buffer_nelts, base_membuffer_offset, *tmp_memsizes_buff,
      left_offset, right_offset, buffer_offset, sizes_buffer_offset;

  ibegin = il;
  ipivot = im;

  buffer_offset = 0;
  base_membuffer_offset = 0;
  left_offset = 0;
  sizes_buffer_offset = 0;

  right_offset = 0;

  current_buffer_window_size = 0;
  current_window_buffer_nelts = 0;

#if 0
  for (i = 0; i <= ir; i++)
    {
      if (i < ibegin){
        left_offset += sizes[i];
        base_membuffer_offset += sizes[i];
        buffer_offset += sizes[i];
        sizes_buffer_offset += 1;
      }

      if (i < ipivot + 1)
        right_offset += sizes[i];

      if (i >= ibegin && i <= ir){
        current_window_buffer_nelts += 1;
        current_buffer_window_size += sizes[i];
      }
    }
#endif

  // assert(buffer_offset == offsets[ibegin]);
  buffer_offset = offsets[ibegin];

  // assert(base_membuffer_offset == offsets[ibegin]);
  base_membuffer_offset = offsets[ibegin];

  // assert(left_offset == offsets[ibegin]);
  left_offset = offsets[ibegin];

  // assert(sizes_buffer_offset == ibegin);
  sizes_buffer_offset = ibegin;

  // assert(right_offset == offsets[ipivot + 1]);
  right_offset = offsets[ipivot + 1];

  // assert(current_buffer_window_size == offsets[ir + 1] - offsets[ibegin]);
  current_buffer_window_size = offsets[ir + 1] - offsets[ibegin];

  // assert(current_window_buffer_nelts == ir - ibegin + 1);
  current_window_buffer_nelts = ir - ibegin + 1;

  // merge root loop
  lhs = ((char *)mem) + left_offset;
  rhs = ((char *)mem) + right_offset;

  tmp_memsizes_buff = memsizes_buff + sizes_buffer_offset;
  tmp_membuff = membuff + buffer_offset;

  im++;
  for (i = 0; il <= ipivot && im <= ir; i++)
    {
      if (cmp (lhs, sizes[il], rhs, sizes[im]) < 0)
        {
          memcpy (tmp_membuff, lhs, sizes[il]);

          *tmp_memsizes_buff = sizes[il];
          tmp_memsizes_buff++;
          tmp_membuff += sizes[il];
          lhs += sizes[il];
          il++;
        }
      else
        {
          memcpy (tmp_membuff, rhs, sizes[im]);

          *tmp_memsizes_buff = sizes[im];
          tmp_memsizes_buff++;
          tmp_membuff += sizes[im];
          rhs += sizes[im];
          im++;
        }
    }

  // Merge suffix loop
  is_left_exhausted = ipivot - il;
  if (is_left_exhausted < 0)
    {
      for (; im <= ir; i++)
        {
          memcpy (tmp_membuff, rhs, sizes[im]);

          *tmp_memsizes_buff = sizes[im];
          tmp_memsizes_buff++;
          tmp_membuff += sizes[im];
          rhs += sizes[im];
          im++;
        }
    }
  else
    {
      for (; il <= ipivot; i++)
        {
          memcpy (tmp_membuff, lhs, sizes[il]);

          *tmp_memsizes_buff = sizes[il];
          tmp_memsizes_buff++;
          tmp_membuff += sizes[il];
          lhs += sizes[il];
          il++;
        }
    }

  // Buff and sizes buff to main
  memcpy ((char *)mem + base_membuffer_offset, membuff + base_membuffer_offset,
          current_buffer_window_size);
  memcpy (sizes + ibegin, memsizes_buff + sizes_buffer_offset,
          current_window_buffer_nelts * sizeof (int));
}

void
xmerge_sort_imp (void *mem, int *sizes, int il, int ir, xcmp_t cmp,
                 char *buffer, int *sizes_buffer, int *offsets)
{
  int im;
  if (il >= ir)
    return;
  im = il + (ir - il) / 2;
  xmerge_sort_imp (mem, sizes, il, im, cmp, buffer, sizes_buffer, offsets);
  xmerge_sort_imp (mem, sizes, im + 1, ir, cmp, buffer, sizes_buffer, offsets);
  xmerge (mem, sizes, il, im, ir, cmp, buffer, sizes_buffer, offsets);
}

void
xmsort (void *mem, int *sizes, int nelts, xcmp_t cmp)
{
  int i, buffer_size, *sizes_buffer, *offset_buffer;
  char *buffer;

  buffer_size = 0;
  offset_buffer = calloc (nelts + 1, sizeof (int));

  for (i = 0; i < nelts; i++)
    {
      buffer_size += sizes[i];
      offset_buffer[i + 1] = offset_buffer[i] + sizes[i];
    }

  buffer = malloc (buffer_size * sizeof (char));
  sizes_buffer = malloc (nelts * sizeof (int));

  xmerge_sort_imp (mem, sizes, 0, nelts - 1, cmp, buffer, sizes_buffer,
                   offset_buffer);

  free (buffer);
  free (sizes_buffer);
  free (offset_buffer);
}

void
print_arr (int *arr, int *sizes, int len)
{
  for (int i = 0; i < len; i++)
    {
      printf ("%d ", *arr);
      arr += sizes[i] / sizeof (int);
    }
  printf ("\n");
}

int
main (void)
{
  int *arr, *mem, len, tmp, *sizes, total_size;

  scanf ("%d", &len);

  sizes = calloc (len, sizeof (int));
  total_size = 0;
  for (int i = 0, basezs = 0; i < len; i++)
    {
      basezs = (sizeof (int) + basezs) % (2 * sizeof (int));
      sizes[i] = basezs + sizeof (int);
      total_size += basezs + sizeof (int);
    }

#if VISUALIZE
  printf ("Sizes: \n");
  for (int i = 0; i < len; i++)
    printf ("%d ", sizes[i]);
  printf ("\n");

  printf ("Total size in chars: \n");
  printf ("%d ", total_size);
  printf ("\n");
#endif

  arr = calloc (total_size / sizeof (int), sizeof (int));
  mem = arr;
  for (int i = 0; i < len; i++)
    {
      scanf ("%d", &tmp);
      *mem = tmp;
      mem += sizes[i] / sizeof (int);
    }

#if VISUALIZE
  printf ("Before\n");
  print_arr (arr, sizes, len);
#endif

#if 1
  xmsort (arr, sizes, len, cmp);
#elif 1
  int *res = get_element_by_idx (arr, sizes, len - 1);
  printf ("%d \n", *res);
#endif

#if VISUALIZE
  printf ("After\n");
#endif

  print_arr (arr, sizes, len);

  free (arr);
  free (sizes);
  return 0;
}
