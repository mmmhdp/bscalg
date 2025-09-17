#include "llst.h"
#include "utils.h"

void test_list_init_and_free(void)
{
  LLST *tmp_list;
  tmp_list = llst_init();

  if (tmp_list == NULL)
  {
    TEST_FAILED("Top node is NULL after initialization\n");
    return;
  }

  llst_free(tmp_list);
  TEST_PASSED();
}

int
main(void)
{
  test_list_init_and_free();

  return 0;
}
