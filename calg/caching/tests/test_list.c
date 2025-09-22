#include <stdlib.h>

#include "list.h"
#include "utils.h"

#define TESTING

void test_list_init_and_free(void)
{
  LIST *l;
  l = list_init();
  list_free(l);
}

void test_list_add_basic(void)
{
}

void test_list_add_complex_case(void)
{}
void test_delete_node(void)
{}

void test_delete_first_node(void)
{}

void test_delete_last_node(void)
{}

void test_list_move_node_to_tail (void)
{}


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
