#include "cache.h"
#include "hshtbl.h"
#include "list.h"
#include <stdlib.h>

#include "utils.h"

void
test_cache_init_and_free (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  CACHE *c = ch_init (10);

#ifdef VERBOSE
#endif

  goto cleanup;

cleanup:
  ch_free (c);
  TEST_PASSED ();
}

void
test_cache_put_basic (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  CACHE *c = ch_init (5);

  int value;
  size_t v_sz;

  value = 3;
  v_sz = sizeof (int);
  ch_put (c, &value, v_sz);

  value = 5;
  ch_put (c, &value, v_sz);

#ifdef VERBOSE
  ch_print (c);
#endif

  goto cleanup;

cleanup:
  ch_free (c);
  TEST_PASSED ();
}

void
test_cache_put_complex (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  CACHE *c = ch_init (5);

  int value;

  for (value = 0; value < 10; value++)
    {
      size_t v_sz = sizeof (int);
      ch_put (c, &value, v_sz);
    }

#ifdef VERBOSE
  ch_print (c);
#endif

  goto cleanup;

cleanup:
  ch_free (c);
  TEST_PASSED ();
}

int
main (void)
{
#ifdef TESTING
  printf ("\nRUNNING %s:\n\n", __FILE__);

  test_cache_init_and_free ();
  test_cache_put_basic ();
  test_cache_put_complex ();

  printf ("\nTESTS ARE COMPLETED\n");
#endif

  return 0;
}
