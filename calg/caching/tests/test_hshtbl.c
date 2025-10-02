#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hshtbl.h"
#include "utils.h"

void
test_hshtbl_init_and_free (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  int ht_sz;
  HSH_TBL *ht;

  ht_sz = 10;
  ht = ht_init (ht_sz);

  goto cleanup;

cleanup:
  ht_free (ht);
  TEST_PASSED ();
}

void
test_hshtbl_add_basic (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  int ht_sz, value, v_sz;
  HSH_TBL *ht;

  ht_sz = 10;
  ht = ht_init (ht_sz);

  value = 4;
  v_sz = sizeof (int);

  ht_add (ht, "Foo", strlen ("Foo"), &value, v_sz);

#ifdef VERBOSE
  ht_print (ht);
#endif

  goto cleanup;

cleanup:
  ht_free (ht);
  TEST_PASSED ();
}

static const int key_mocks_len = 5;
static const char *key_mocks[] = { "Foo", "Boo", "Zoo", "Buzz", "Fuzz" };

void
test_hshtbl_add_complex (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  int i, ht_sz, value, v_sz;
  char *key;
  HSH_TBL *ht;

  ht_sz = 10;
  ht = ht_init (ht_sz);

  for (i = 0; i < 5; i++)
    {
      value = i;
      v_sz = sizeof (int);
      key = (char *)key_mocks[i % key_mocks_len];
      ht_add (ht, key, strlen (key), &value, v_sz);
    }

#ifdef VERBOSE
  ht_print (ht);
#endif

  goto cleanup;

cleanup:
  ht_free (ht);
  TEST_PASSED ();
}

void
test_hshtbl_delete (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  int i, ht_sz, value, v_sz;
  char *key;
  HSH_TBL *ht;

  ht_sz = 10;
  ht = ht_init (ht_sz);

  for (i = 0; i < 5; i++)
    {
      value = i;
      v_sz = sizeof (int);
      key = (char *)key_mocks[i % key_mocks_len];
      ht_add (ht, key, strlen (key), &value, v_sz);
    }

  ht_delete (ht, key, strlen (key));

#ifdef VERBOSE
  ht_print (ht);
#endif

  goto cleanup;

cleanup:
  ht_free (ht);
  TEST_PASSED ();
}

void
test_hshtbl_find (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  int i, ht_sz, value, v_sz, *ptest_value;
  char *key;
  HSH_TBL *ht;
  HSH_VAL *v;

  ht_sz = 10;
  ht = ht_init (ht_sz);

  for (i = 0; i < 5; i++)
    {
      value = i;
      v_sz = sizeof (int);
      key = (char *)key_mocks[i % key_mocks_len];
      ht_add (ht, key, strlen (key), &value, v_sz);
    }

  i = 3;
  key = (char *)key_mocks[i % key_mocks_len];
  v = ht_find (ht, key, strlen (key));
  ptest_value = ht_get_value (v);

  if (v == NULL)
    TEST_FAILED ("Key %s is not found!", key);
  else
    printf ("Key %s is found with stored value %d!\n", key, *ptest_value);

  key = "Trash";
  v = ht_find (ht, key, strlen (key));
  ptest_value = ht_get_value (v);

  if (v == NULL)
    printf ("Key %s is not found, but it should be like it!\n", key);
  else
    printf ("Key %s is found with stored value %d!\n", key, *ptest_value);

#ifdef VERBOSE
  ht_print (ht);
#endif

  goto cleanup;

cleanup:
  ht_free (ht);
  TEST_PASSED ();
}

int
main (void)
{
#ifdef TESTING
  printf ("\nRUNNING %s:\n\n", __FILE__);

  test_hshtbl_init_and_free ();
  test_hshtbl_add_basic ();
  test_hshtbl_add_complex ();
  test_hshtbl_delete ();
  test_hshtbl_find ();

  printf ("\nTESTS ARE COMPLETED\n");
#endif

  return 0;
}
