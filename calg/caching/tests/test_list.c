#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "list.h"
#include "utils.h"

void
test_list_init_and_free (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  LIST *l;

  l = list_init ();

  TEST_PASSED ();

  goto cleanup;

cleanup:
  list_free (l);
}

void
free_val (NODE_DATA *d)
{
  void *v = list_node_data_get_value (d);
  free (v);
}

void
test_list_init_and_free_by_caller (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  LIST *l;

  l = list_init ();

  TEST_PASSED ();

  goto cleanup;

cleanup:
  list_free_by_caller (l, free_val);
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
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

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
val_copy (void **dst, void *v, size_t v_sz)
{
  *dst = malloc (v_sz);
  memcpy (*dst, v, v_sz);
}

void
test_list_add_by_caller_basic (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  LIST *l;
  int a;

  l = list_init ();

  a = 3;

  list_add_node_by_caller (l, &a, sizeof (int), val_copy);

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  goto cleanup;

cleanup:
  list_free_by_caller (l, free_val);
  TEST_PASSED ();
}

struct nested_val
{
  void *mem;
  int mem_sz;
};

struct nested_val *
create_nested_val (int x)
{
  struct nested_val *nv;

  nv = malloc (sizeof (struct nested_val));
  nv->mem_sz = sizeof (int);
  nv->mem = malloc (nv->mem_sz);
  memcpy (nv->mem, &x, sizeof (int));

  return nv;
}

void
base_free_nested_val (struct nested_val *nv)
{
  free (nv->mem);
  free (nv);
}

void
free_nested_val (NODE_DATA *d)
{
  struct nested_val *v = (struct nested_val *)list_node_data_get_value (d);

  free (v->mem);
  free (v);
}

void
nested_val_copy (void **dst, void *v, size_t v_sz)
{
  struct nested_val *tmp = (struct nested_val *)v;
  struct nested_val *buff;

  buff = malloc (v_sz);
  memcpy (buff, tmp, v_sz);

  buff->mem = calloc (tmp->mem_sz, 1);
  memcpy (buff->mem, tmp->mem, tmp->mem_sz);

  *dst = buff;
}

#ifdef VERBOSE
void
nested_val_printer (NODE_DATA *d, int is_top_node)
{
  struct nested_val *tv;
  int *itv;

  if (!d)
    {
      printf ("(NULL node)");
      return;
    }

  tv = (struct nested_val *)list_node_data_get_value (d);

  itv = (int *)tv->mem;
  if (is_top_node)
    printf ("(top node val : %d)-> ", *itv);
  else
    printf ("(node val : %d)-> ", *itv);
}
#endif

void
test_list_add_by_caller_with_nested_node_val (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  LIST *l;
  struct nested_val *nv1, *nv2, *nv3;

  l = list_init ();

  nv1 = create_nested_val (3);
  nv2 = create_nested_val (4);
  nv3 = create_nested_val (5);

  list_add_node_by_caller (l, nv1, sizeof (struct nested_val),
                           nested_val_copy);
  list_add_node_by_caller (l, nv2, sizeof (struct nested_val),
                           nested_val_copy);
  list_add_node_by_caller (l, nv3, sizeof (struct nested_val),
                           nested_val_copy);

#ifdef VERBOSE
  list_print (l, nested_val_printer);
#endif

  goto cleanup;

cleanup:
  list_free_by_caller (l, free_nested_val);
  base_free_nested_val (nv1);
  base_free_nested_val (nv2);
  base_free_nested_val (nv3);
  TEST_PASSED ();
}

void
test_list_add_complex_case (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

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
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

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
  printf ("\n");
#endif

  for (i = 0; i < 3; i++)
    {
      tn = list_get_top_node (l);
      list_move_node_to_tail (l, tn);

#ifdef VERBOSE
      list_print (l, int_printer);
      printf ("\n");
#endif
    }

  goto cleanup;

cleanup:
  list_free (l);
  TEST_PASSED ();
}

void
test_list_move_node_to_tail_with_one_node (void)
{
#ifdef VERBOSE
  TEST_BEGIN ();
#endif

  LIST *l;
  LIST_NODE *tn;
  int i, a;

  l = list_init ();

  a = 3;

  list_add_node (l, &a, sizeof (int));

#ifdef VERBOSE
  list_print (l, int_printer);
  printf ("\n");
#endif

  for (i = 0; i < 3; i++)
    {
      tn = list_get_top_node (l);
      list_move_node_to_tail (l, tn);

#ifdef VERBOSE
      list_print (l, int_printer);
      printf ("\n");
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
  printf ("\nRUNNING %s:\n\n", __FILE__);

  test_list_init_and_free ();
  test_list_init_and_free_by_caller ();
  test_list_add_basic ();
  test_list_add_by_caller_basic ();
  test_list_add_by_caller_with_nested_node_val ();
  test_list_add_complex_case ();
  test_list_move_node_to_tail ();
  test_list_move_node_to_tail_with_one_node ();

  printf ("\nTESTS ARE COMPLETED\n");
#endif

  return 0;
}
