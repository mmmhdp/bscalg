#define _GNU_SOURCE

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hshtbl.h"
#include "list.h"

#define HSH_BASE_TB_CP 1048576 // 2**20
#define HST_BASE_2_POW_STEPS_TO_INT_32 12
#define HSH_P 9973
#define HSH_A 123
#define HSH_B 23

typedef struct hshtbl
{
  uint32_t(*hsh_func) (char*, uint32_t);
  LIST **mem;
  uint32_t sz;
} HSHTBL;

uint32_t
ht_hsf (char *key, uint32_t hstb_sz)
{
  int i;
  uint32_t hs;
  char c;

  if (strlen (key) == 0)
    hs = 0;

  for (i = 0; i < (int)strlen (key); i++)
    {
      c = key[i];
      hs = ((HSH_A * c + HSH_B) % HSH_P) % hstb_sz;
    }

  return hs;
}
