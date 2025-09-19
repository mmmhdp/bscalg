#include <stdlib.h>

#include "llst.h"
#include "utils.h"

#define TESTING

void test_list_init_and_free(void)
{
  int len;
  LLST *tmp_list, *top;

  tmp_list = llst_init();

  top = llst_get_top(tmp_list);
  len = llst_get_len(top);

  if (len != 0)
  {
    TEST_FAILED("List is in incomplete state!");
    goto cleanup;
  }

  if (tmp_list == NULL)
  {
    TEST_FAILED("Top node is NULL after initialization\n");
    goto cleanup;
  }

  TEST_PASSED();
cleanup: 
  llst_free(tmp_list);
}

void test_list_add_basic(void)
{
  LLST *lst, *top;
  int *arr, len;

  lst = llst_init();
  top = llst_get_top(lst);

  llst_add_node(top, 10);
  len = llst_get_len(top);
  arr = llst_convert_to_arr(top);

  if (len != 1)
  {
    TEST_FAILED("List has incorrect lenght after addition of the new element");
    goto cleanup;
  }
  
  if (arr[0] != 10)
  {
    TEST_FAILED("List is in incomplete state!");
    goto cleanup;
  }


  TEST_PASSED();

cleanup: 
  free(arr);
  llst_free(lst);
}

void test_list_add_complex_case(void)
{
  LLST *lst, *top;
  int *arr, len;

  lst = llst_init();
  top = llst_get_top(lst);

  llst_add_node(top, 3);
  llst_add_node(top, 4);

  len = llst_get_len(top);
  arr = llst_convert_to_arr(top);
  for (int i = 0; i < len; i++)
  {
    if (arr[i] != (i + 3))
    {
      TEST_FAILED("List is in incomplete state!");
      goto cleanup;
    }
  }
  free(arr);

  llst_add_node(top, 5);
  len = llst_get_len(top);
  arr = llst_convert_to_arr(top);
  
  for (int i = 0; i < len; i++)
  {
    if (arr[i] != (i + 3))
    {
      TEST_FAILED("List is in incomplete state!");
      goto cleanup;
    }
  }

  TEST_PASSED();

cleanup:
  free(arr);
  llst_free(lst);
}

void test_delete_node(void)
{
  LLST *lst, *top, *node;
  int *arr;

  lst = llst_init();
  top = llst_get_top(lst);

  llst_add_node(top, 3);
  llst_add_node(top, 4);
  llst_add_node(top, 5);

  node = llst_get_next(top);
  node = llst_get_next(node);
  
  llst_delete_node(node);

  arr = llst_convert_to_arr(top);

  if (arr[0] != 3 || arr[1] != 5)
  {
    TEST_FAILED("List is in incomplete state after deletion");
    goto cleanup;
  }

  TEST_PASSED();

cleanup:
  free(arr);
  llst_free(lst);
}

void test_delete_first_node(void)
{
  LLST *lst, *top, *node;
  int *arr;

  lst = llst_init();
  top = llst_get_top(lst);

  llst_add_node(top, 3);
  llst_add_node(top, 4);
  llst_add_node(top, 5);

  node = llst_get_next(top);
  
  llst_delete_node(node);

  arr = llst_convert_to_arr(top);

  if (arr[0] != 4 || arr[1] != 5)
  {
    TEST_FAILED("List is in incomplete state after deletion");
    goto cleanup;
  }

  TEST_PASSED();

cleanup:
  free(arr);
  llst_free(lst);
}

void test_delete_last_node(void)
{
  LLST *lst, *top, *node;
  int *arr;

  lst = llst_init();
  top = llst_get_top(lst);

  llst_add_node(top, 3);
  llst_add_node(top, 4);
  llst_add_node(top, 5);

  node = llst_get_next(top);
  node = llst_get_next(node);
  node = llst_get_next(node);
  
  llst_delete_node(node);

  arr = llst_convert_to_arr(top);

  if (arr[0] != 3 || arr[1] != 4)
  {
    TEST_FAILED("List is in incomplete state after deletion");
    goto cleanup;
  }

  TEST_PASSED();

cleanup:
  free(arr);
  llst_free(lst);
}

void test_list_move_node_to_tail (void)
{
  LLST *lst, *top, *nxt_node;
  int i, *arr;

  arr = NULL;

  lst = llst_init();
  top = llst_get_top(lst);

  llst_add_node(top, 3);
  llst_add_node(top, 4);
  llst_add_node(top, 5);

  for (i = 0; i < 3; i++)
  {
    nxt_node = llst_get_next(top);
    llst_move_node_to_tail(nxt_node);

    free(arr);
    arr = llst_convert_to_arr(top);

    switch (i)
    {
      case 0:
        {
          if (arr[0] != 4 || arr[1] != 5 || arr[2] != 3)
          {
            TEST_FAILED("List is in incomplete state after rotation to tail");
            goto cleanup;
          }
          break;
        }
      case 1:
        {
          if (arr[0] != 5 || arr[1] != 3 || arr[2] != 4)
          {
            TEST_FAILED("List is in incomplete state after rotation to tail");
            goto cleanup;
          }
          break;
        }
      case 2:
        {
          if (arr[0] != 3 || arr[1] != 4 || arr[2] != 5)
          {
            TEST_FAILED("List is in incomplete state after rotation to tail");
            goto cleanup;
          }
          break;
        }
    }
  }

  TEST_PASSED();

cleanup:
  free(arr);
  llst_free(lst);
}

int
main(void)
{
#ifdef TESTING
  printf("\nRUNNING TESTS:\n\n");

  test_list_init_and_free();
  test_list_add_basic();
  test_list_add_complex_case();
  test_delete_node();
  test_delete_last_node();
  test_delete_first_node();
  test_list_move_node_to_tail();

  printf("\nTESTS ARE COMPLETED\n");
#endif

  return 0;
}
