#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "list.h"
#include "utils.h"

static void
reset_tests_cnt (void)
{
  TESTS_CNT = 0;
  SUCC_TESTS_CNT = 0;
}

int
test_list_init_and_free (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;

  l = list_init ();
  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

void
free_val (NODE_DATA *d)
{
  void *v = list_node_data_get_value (d);
  free (v);
}

int
test_list_init_and_free_by_caller (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;

  l = list_init ();

  list_free_by_caller (l, free_val);

  TEST_PASSED_EXT ();
  return TRUE;
}

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

int
test_list_add_basic (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  int a;

  l = list_init ();

  a = 3;

  list_add_node (l, &a, sizeof (int));

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

void
val_copy (void **dst, void *v, size_t v_sz)
{
  *dst = malloc (v_sz);
  memcpy (*dst, v, v_sz);
}

int
test_list_add_by_caller_basic (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  int a;

  l = list_init ();

  a = 3;

  list_add_node_by_caller (l, &a, sizeof (int), val_copy);

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  list_free_by_caller (l, free_val);
  TEST_PASSED_EXT ();
  return TRUE;
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
  struct nested_val *v;

  v = (struct nested_val *)list_node_data_get_value (d);

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

int
test_list_add_by_caller_with_nested_node_val (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  int i;
  struct nested_val *tmp;

  l = list_init ();

  for (i = 3; i < 6; i++)
    {
      tmp = create_nested_val (i);
      list_add_node_by_caller (l, tmp, sizeof (struct nested_val),
                               nested_val_copy);
      base_free_nested_val (tmp);
    }

#ifdef VERBOSE
  list_print (l, nested_val_printer);
#endif

  list_free_by_caller (l, free_nested_val);

  TEST_PASSED_EXT ();
  return TRUE;
}

int
test_list_add_complex_case (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  int tmp;

  l = list_init ();

  for (tmp = 3; tmp < 6; tmp++)
    list_add_node (l, &tmp, sizeof (int));

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

int
test_list_move_node_to_tail (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  LIST_NODE *tn;
  int i, tmp;

  l = list_init ();

  for (tmp = 3; tmp < 6; tmp++)
    list_add_node (l, &tmp, sizeof (int));

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

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

int
test_list_move_node_to_tail_with_one_node (void)
{
  TEST_BEGIN_EXT ();

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

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

int
test_list_move_tail_node_to_tail (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  LIST_NODE *tn;
  int tmp;

  l = list_init ();

  for (tmp = 3; tmp < 6; tmp++)
    list_add_node (l, &tmp, sizeof (int));

#ifdef VERBOSE
  list_print (l, int_printer);
  printf ("\n");
#endif

  tn = list_get_tail_node (l);
  list_move_node_to_tail (l, tn);

#ifdef VERBOSE
  list_print (l, int_printer);
  printf ("\n");
#endif

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

int
test_delete_top_node (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  int tmp;

  l = list_init ();

  for (tmp = 3; tmp < 6; tmp++)
    list_add_node (l, &tmp, sizeof (int));

#ifdef VERBOSE
  LIST_NODE *ln;

  printf ("Before:\n");
  list_print (l, int_printer);
  ln = list_get_top_node (l);
  tmp = *((int *)list_node_data_get_value (list_node_get_data (ln)));

  list_delete_node (l, ln);

  printf ("\nAfter deletion of top node with val %d:\n", tmp);
  list_print (l, int_printer);
#endif

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

int
test_delete_tail_node (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  int tmp;

  l = list_init ();

  for (tmp = 3; tmp < 6; tmp++)
    list_add_node (l, &tmp, sizeof (int));

#ifdef VERBOSE
  printf ("Before:\n");
  list_print (l, int_printer);

  LIST_NODE *ln;

  ln = list_get_tail_node (l);
  tmp = *((int *)list_node_data_get_value (list_node_get_data (ln)));

  list_delete_node (l, ln);

  printf ("\nAfter deletion of tail node with val %d:\n", tmp);
  list_print (l, int_printer);
#endif

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

int
test_delete_some_node (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  int tmp;

  l = list_init ();

  tmp = 0;
  for (; tmp < 10; tmp++)
    list_add_node (l, &tmp, sizeof (int));

#ifdef VERBOSE
  LIST_NODE *ln;

  printf ("Before:\n");
  list_print (l, int_printer);
  ln = list_get_top_node (l);

  for (tmp = 0; tmp < 2; tmp++)
    ln = list_node_get_next (ln);

  tmp = *((int *)list_node_data_get_value (list_node_get_data (ln)));

  list_delete_node (l, ln);

  printf ("\nAfter deletion of tail node with val %d:\n", tmp);
  list_print (l, int_printer);
#endif

  list_free (l);

  TEST_PASSED_EXT ();
  return TRUE;
}

static int
are_ints_equal (void *lhs, void *rhs)
{
  int *ltmp, *rtmp;

  ltmp = lhs;
  rtmp = rhs;

  return ((*ltmp - *rtmp) == 0) ? TRUE : FALSE;
}

void
test_list_arr_init_and_free (void)
{
  TEST_BEGIN_EXT ();

  LIST *l;
  LIST_ARR *la, *test_against_arr;
  int tmp, is_succ;

  l = list_init ();

  for (tmp = 3; tmp < 6; tmp++)
    list_add_node (l, &tmp, sizeof (int));

  la = list_arr_init (l);

  for (int i = 0; i < (int)la->arr_sz; i++)
    {
      int *pa = (int *)la->arr;
      int curr = pa[i];
      if (i == 0)
        printf ("(Top node %d)->", curr);
      else
        printf ("(Node %d)->", curr);
    }
  printf ("(Null node)\n");

  test_against_arr = &(LIST_ARR){
    .arr = (int[]){ 3, 4, 5 },
    .el_szs = (size_t[]){ 4, 4, 4 },
    .arr_sz = 3,
  };

  is_succ = list_arr_is_equal (la, test_against_arr, are_ints_equal);
  if (!is_succ)
    TEST_FAILED ();

#ifdef VERBOSE
  list_print (l, int_printer);
#endif

  list_arr_free (la);
  list_free (l);

  TEST_PASSED_EXT ();
}

int
main (void)
{
#ifdef TESTING
  printf ("\nRUNNING %s:\n\n", __FILE__);

  reset_tests_cnt ();
#if 1
  test_list_init_and_free ();
  test_list_init_and_free_by_caller ();

  test_list_add_basic ();

  test_list_add_by_caller_basic ();
  test_list_add_by_caller_with_nested_node_val ();
  test_list_add_complex_case ();

  test_delete_top_node ();
  test_delete_tail_node ();
  test_delete_some_node ();

  test_list_move_node_to_tail ();
  test_list_move_node_to_tail_with_one_node ();
  test_list_move_tail_node_to_tail ();
#endif
  test_list_arr_init_and_free ();

  if (SUCC_TESTS_CNT != TESTS_CNT)
    printf ("\nONLY %d TESTS FROM %d ARE COMPLETED SUCCESSFULLY!\n",
            SUCC_TESTS_CNT, TESTS_CNT);
  else
    printf ("\nALL %d TESTS ARE COMPLETED SUCCESSFULLY!\n", TESTS_CNT);
#endif

  return 0;
}
