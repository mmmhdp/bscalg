#include <stdlib.h>
#include <strings.h>

#include "list.h"
#include "utils.h"

void
test_list_init_and_free (void)
{

  LIST *l;

  l = list_init ();

  TEST_PASSED ();

  goto cleanup;

cleanup:
  list_free (l);
}

#ifdef VERBOSE
void
int_printer (NODE_DATA *d, int is_top_node)
{
  void *tv;
  int *itv;

  if (!d)
  {
    printf ("(NULL node)");
    return;
  }

  tv = list_node_data_get_value (d);

  itv = (int *)tv;
  if (is_top_node)
    printf ("(top node val : %d)-> ", *itv);
  else 
    printf ("(node val : %d)-> ", *itv);
}
#endif

void
test_list_add_basic (void)
{
  LIST *l;
  int a;

  l = list_init ();

  a = 3;

  list_add_node (l, &a, sizeof (int));

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  goto cleanup;

cleanup:
  list_free (l);
  TEST_PASSED ();
}

void
test_list_add_complex_case (void)
{
  LIST *l;
  int a, b, c;

  l = list_init ();

  a = 3;
  b = 4;
  c = 5;

  list_add_node (l, &a, sizeof (int));
  list_add_node (l, &b, sizeof (int));
  list_add_node (l, &c, sizeof (int));

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  goto cleanup;

cleanup:
  list_free (l);
  TEST_PASSED ();
}

void
test_list_move_node_to_tail (void)
{
  LIST *l;
  LIST_NODE *tn;
  int i, a, b, c;

  l = list_init ();

  a = 3;
  b = 4;
  c = 5;

  list_add_node (l, &a, sizeof (int));
  list_add_node (l, &b, sizeof (int));
  list_add_node (l, &c, sizeof (int));

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  for (i = 0; i < 3; i++)
  {
    tn = list_get_top_node(l);
    list_move_node_to_tail(l, tn);

#ifdef VERBOSE
    list_print (l, int_printer);
#endif
  }

  goto cleanup;

cleanup:
  list_free (l);
  TEST_PASSED ();
}

int
main (void)
{
#ifdef TESTING
  printf ("\nRUNNING TESTS:\n\n");

  test_list_init_and_free ();
  test_list_add_basic ();
  test_list_add_complex_case ();
  test_list_move_node_to_tail ();

  printf ("\nTESTS ARE COMPLETED\n");
#endif

  return 0;
}
