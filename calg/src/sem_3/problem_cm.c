#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cmp will return positive if lhs > rhs, zero is lhs = rhs and negative if lhs
// < rhs
int compare(void *lhs, int lsz, void *rhs, int rsz) {
  if (lsz == rsz) {
  }
  int *tlhs = (int *)lhs;
  int *trhs = (int *)rhs;
  return (*tlhs - *trhs);
}

typedef int (*xcmp_t)(void *lhs, int lsz, void *rhs, int rsz);
/*calculates a number of bytes need for mem alloc of buffer
 * with respect of different size of el stored in sizes
 */
int get_buffsz(int sizes[], int l_idx, int r_idx) {
  int bytes_accum = 0;
  for (int i = l_idx; i < r_idx + 1; i++)
    bytes_accum += sizes[i];
  return bytes_accum;
}

int get_offset(int sizes[], int idx) {
  int offset = 0;
  for (int i = 0; i < idx; i++)
    offset += sizes[i];
  return offset;
}

void *get_ptrbyoffset(void *mem, int bytes_offset) {
  int8_t *pmem = mem;
  pmem += bytes_offset;
  return (void *)pmem;
}

void byteshift(void **p, int nbytes) { *p = (int8_t *)(*p) + nbytes; }

void upd_buff(void *buffer, int *ibuffszs, int buffszs[], void *src,
              int srcsz) {
  int buffer_offset = get_offset(buffszs, *ibuffszs);
  void *tmp_pbuffer = get_ptrbyoffset(buffer, buffer_offset);

  memcpy(tmp_pbuffer, src, srcsz);
  buffszs[*ibuffszs] = srcsz;
  (*ibuffszs)++;
}

void mem_insert(void *mem, int sizes[], void *buffer, int buffszs[],
                int ibuffszs, int il, int ir) {
  int offset, buffer_offset, sz;
  void *pmem, *tmp_pbuffer;

  for (int ibuff = 0, jmem = il; ibuff < ibuffszs && jmem <= ir;
       ibuff++, jmem++) {
    offset = get_offset(sizes, jmem);
    pmem = get_ptrbyoffset(mem, offset);

    buffer_offset = get_offset(buffszs, ibuff);
    tmp_pbuffer = get_ptrbyoffset(buffer, buffer_offset);

    sz = buffszs[ibuff];
    sizes[jmem] = sz;

    memcpy(pmem, tmp_pbuffer, sz);
  }
}

void xmerge(void *mem, int *sizes, int l, int m, int r, xcmp_t cmp) {
  int il = l, ir = m + 1;

  int nbytes4buff = get_buffsz(sizes, l, r);
  void *buffer = calloc(nbytes4buff, sizeof(int8_t));

  int buffsizessize = r - l + 1;
  int32_t *buffszs = calloc(buffsizessize, sizeof(int32_t));
  int ibuffszs = 0;

  void *plval, *prval;
  int loffset, lsz, roffset, rsz;
  while ((ibuffszs < r - l + 1) && (il < m + 1) && (ir < r + 1)) {
    loffset = get_offset(sizes, il);
    lsz = sizes[il];
    plval = get_ptrbyoffset(mem, loffset);

    roffset = get_offset(sizes, ir);
    rsz = sizes[ir];
    prval = get_ptrbyoffset(mem, roffset);

    if (cmp(plval, lsz, prval, rsz) <= 0) {
      upd_buff(buffer, &ibuffszs, buffszs, (void *)plval, lsz);
      il++;
    } else {
      upd_buff(buffer, &ibuffszs, buffszs, (void *)prval, rsz);
      ir++;
    }
  }

  if (il == m + 1 && ir == r + 1) {
    ;
  } else if (ir < r + 1) {
    while (ir < r + 1) {
      roffset = get_offset(sizes, ir);
      rsz = sizes[ir];
      prval = get_ptrbyoffset(mem, roffset);
      upd_buff(buffer, &ibuffszs, buffszs, (void *)prval, rsz);
      ir++;
    }
  } else {
    while (il < m + 1) {
      loffset = get_offset(sizes, il);
      lsz = sizes[il];
      plval = get_ptrbyoffset(mem, loffset);
      upd_buff(buffer, &ibuffszs, buffszs, (void *)plval, lsz);
      il++;
    }
  }

  mem_insert(mem, sizes, buffer, buffszs, ibuffszs, l, r);

  free(buffer);
  free(buffszs);
}

void print_arr(int *arr, int len) {
  for (int i = 0; i < len; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

void xmsort_imp(void *mem, int *sizes, int l, int r, xcmp_t cmp) {
  if (l >= r)
    return;
  int m = (l + r) / 2;
  xmsort_imp(mem, sizes, l, m, cmp);
  xmsort_imp(mem, sizes, m + 1, r, cmp);
  xmerge(mem, sizes, l, m, r, cmp);
}

void xmsort(void *mem, int *sizes, int nelts, xcmp_t cmp) {
  xmsort_imp(mem, sizes, 0, nelts - 1, cmp);
}

int main(void) {
  int32_t arr[4] = {15, 2, -3, 5};
  int arr_sizes[4] = {sizeof(int32_t), sizeof(int32_t), sizeof(int32_t),
                      sizeof(int32_t)};

  int nelts = 4;
  xcmp_t cmp = compare;
  xmsort((void *)arr, arr_sizes, nelts, cmp);
  print_arr(arr, nelts);
}
