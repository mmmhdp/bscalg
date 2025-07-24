#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

int
check_scanf_res (int expected, int nitems)
{
  int status_code = (expected != nitems) ? FALSE : TRUE;
  return status_code;
}

int
check_malloc_res (int *p)
{
  int status_code = (p == NULL) ? FALSE : TRUE;
  return status_code;
}

void
fill_arr (int *arr, int len)
{
  int i, tmp, nitems;
  for (i = 0; i < len; i++)
    {
      nitems = scanf ("%d", &tmp);
      if (!check_scanf_res (1, nitems))
        abort ();
      arr[i] = tmp;
    }
}

unsigned
get_idx_min (int *arr, int len, unsigned last_sorted_idx)
{
  unsigned min_idx = last_sorted_idx;
  int tmp_min = arr[last_sorted_idx];

  for (int i = last_sorted_idx + 1; i < len; i++)
    {
      if (tmp_min > arr[i])
        {
          tmp_min = arr[i];
          min_idx = i;
        }
    }

  return min_idx;
}

void
swap (int *arr, unsigned isrc, unsigned idist)
{
  int tmp = arr[isrc];
  arr[isrc] = arr[idist];
  arr[idist] = tmp;
}

void
gswap (void *parr, int eltsize, int isrc, int idst)
{
  char *tmp_parr = parr;
  unsigned el_byte_size = sizeof (char) * eltsize;
  char *ptmp = malloc (sizeof (char) * eltsize);
  char *psrc = tmp_parr + isrc * el_byte_size;
  char *pdst = tmp_parr + idst * el_byte_size;

  memcpy (ptmp, psrc, el_byte_size);
  memcpy (psrc, pdst, el_byte_size);
  memcpy (pdst, ptmp, el_byte_size);

  free (ptmp);
}

void
sort_iteration (int *arr, unsigned len, unsigned idx)
{
  if (idx == len - 1)
    return;
  unsigned imin = get_idx_min (arr, len, idx);
  // swap(arr, idx, imin);
  gswap ((void *)arr, sizeof (int), idx, imin);
}

// Компаратор возвращает 1 (TRUE) если lhs логически меньше, чем rhs, иначе 0
// (FALSE)

typedef int (*cmp_t) (void const *lhs, void const *rhs);

int
cmp_int (void const *lhs, void const *rhs)
{
  int *plhs = (int *)lhs;
  int *prhs = (int *)rhs;
  if ((*plhs) < (*prhs))
    return 1;
  return 0;
}

unsigned
gminidx (void *parr, int eltsize, int numelts, int nsorted, cmp_t cmp)
{
  int i, min_idx = nsorted;
  char *tparr = parr, *ptmp, *pcmin;
  unsigned el_byte_size = sizeof (char) * eltsize;

  for (i = nsorted + 1; i < numelts; i++)
    {
      pcmin = (void *)(tparr + min_idx * el_byte_size);
      ptmp = (void *)(tparr + i * el_byte_size);
      if (cmp (ptmp, pcmin))
        {
          min_idx = i;
        }
    }

  return min_idx;
}

int
selstep (void *parr, int eltsize, int numelts, int nsorted, cmp_t cmp)
{
  if (nsorted == numelts - 1)
    return 0;
  int imin = gminidx (parr, eltsize, numelts, nsorted, cmp);
  gswap (parr, eltsize, nsorted, imin);
  return 0;
}

void
print_arr (int *arr, int len)
{
  for (int i = 0; i < len; i++)
    printf ("%d ", arr[i]);
  printf ("\n");
}

int
main (void)
{
  int nitems, *arr, last_sorted_idx;
  unsigned len;

  nitems = scanf ("%u", &len);
  if (!check_scanf_res (1, nitems))
    abort ();

  arr = (int *)malloc (len * sizeof (int));
  if (!check_malloc_res (arr))
    abort ();

  fill_arr (arr, len);

  nitems = scanf ("%d", &last_sorted_idx);
  if (!check_scanf_res (1, nitems))
    abort ();

#if 0
#if 0
  printf("Before:\n");
  print_arr(arr, len);

  sort_iteration(arr, len, last_sorted_idx);

  printf("After:\n");
  print_arr(arr, len);
#else
  sort_iteration(arr, len, last_sorted_idx);

  print_arr(arr, len);
#endif
#endif

  printf ("Before:\n");
  print_arr (arr, len);

  cmp_t cmp = cmp_int;

  selstep (arr, sizeof (int), len, last_sorted_idx, cmp);

  printf ("After:\n");
  print_arr (arr, len);

  free (arr);
}
