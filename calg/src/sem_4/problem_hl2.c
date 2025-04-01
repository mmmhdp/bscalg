#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  struct node_t *next;
  int data;
};

struct node_t *list_is_a_loop(struct node_t *top) {
  struct node_t *tortoise = NULL;
  struct node_t *hare = NULL;

  if (!top)
    return 0;
  if (!top->next)
    return 0;

  tortoise = top;
  hare = top->next;

  for (;;) {
    if (tortoise == hare)
      return hare;
    if (!hare->next)
      break;
    if (!hare->next->next)
      break;

    tortoise = tortoise->next;
    hare = hare->next->next;
  }

  return NULL;
}

int loop_len(struct node_t *top) {
  int loop_len_cnt = 0;
  struct node_t *pivot_node = NULL;
  struct node_t *forward = NULL;
  pivot_node = list_is_a_loop(top);

  if (!pivot_node)
    return 0;

  forward = pivot_node;

  for (;;) {
    loop_len_cnt++;
    if ((forward = forward->next) == pivot_node)
      return loop_len_cnt;
  }

  assert(0);
  return -1;
}

void free_list(struct node_t *top) {
  if (!top)
    return;

  struct node_t *tortoise = top, *hare = top;
  int hasCycle = 0;
  while (hare && hare->next) {
    tortoise = tortoise->next;
    hare = hare->next->next;
    if (tortoise == hare) {
      hasCycle = 1;
      break;
    }
  }

  if (hasCycle) {
    tortoise = top;
    while (tortoise != hare) {
      tortoise = tortoise->next;
      hare = hare->next;
    }
    struct node_t *prev = tortoise;
    while (prev->next != tortoise)
      prev = prev->next;
    prev->next = NULL;
  }

  while (top != NULL) {
    struct node_t *tmp = top;
    top = top->next;
    free(tmp);
  }
}

int main(void) {
  int i, nnodes, looppos, res, outcome;
  struct node_t *top = NULL, *cur = NULL, *loopelem = NULL;

  res = scanf("%d%d", &nnodes, &looppos);
  assert(res == 2);
  assert(nnodes >= 0);
  assert(looppos > -2);
  assert(looppos < nnodes);

  if (nnodes > 0) {
    top = calloc(1, sizeof(struct node_t));
    cur = top;
    if (looppos == 0)
      loopelem = cur;
  }

  for (i = 1; i < nnodes; ++i) {
    cur->next = calloc(1, sizeof(struct node_t));
    if (looppos == i)
      loopelem = cur;
    cur = cur->next;
  }

  if (looppos != -1)
    cur->next = loopelem;

  outcome = loop_len(top);
  printf("%d\n", outcome);

  free_list(top);
}
