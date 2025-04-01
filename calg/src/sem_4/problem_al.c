#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  struct node_t *next;
  int data;
};

struct node_t *read_list(FILE *inp) {
  int nitems, tmp;

  struct node_t *tmp_node;
  struct node_t *even_head = NULL;
  struct node_t *even_tail = NULL;
  struct node_t *odd_head = NULL;
  struct node_t *odd_tail = NULL;

  while ((nitems = fscanf(inp, "%d", &tmp)) == 1) {
    tmp_node = calloc(1, sizeof(struct node_t));
    tmp_node->data = tmp;
    tmp_node->next = NULL;

    if (tmp % 2) {
      if (!odd_head)
        odd_head = odd_tail = tmp_node;
      else {
        odd_tail->next = tmp_node;
        odd_tail = odd_tail->next;
      }
    } else {
      if (!even_head)
        even_head = even_tail = tmp_node;
      else {
        even_tail->next = tmp_node;
        even_tail = even_tail->next;
      }
    }
  }

  if (even_head)
    even_tail->next = odd_head;
  else
    even_head = odd_head;

  return even_head;
}

void delete_list(struct node_t *top) {
  if (!top) {
    free(top);
    return;
  }
  delete_list(top->next);

  free(top);
}

int main(void) {
  struct node_t *top = read_list(stdin);
  struct node_t *cur = top;

  while (cur) {
    printf("%d ", cur->data);
    cur = cur->next;
  }
  printf("\n");
  printf("\n");

  delete_list(top);
}
