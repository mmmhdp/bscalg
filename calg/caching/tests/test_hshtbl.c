#include <stdint.h>
#include <stdlib.h>

#include "hshtbl.h"
#include "utils.h"

void
test_hshtbl_init_and_free (void)
{
  int ht_sz;
  HSH_TBL *ht;

  ht_sz = 10;
  ht = ht_init (ht_sz);

  goto cleanup;

cleanup:
  ht_free (ht);
}

int
main (void)
{
#ifdef TESTING
  printf ("\nRUNNING %s:\n\n", __FILE__);

  test_hshtbl_init_and_free ();

  printf ("\nTESTS ARE COMPLETED\n");
#endif

  return 0;
}
