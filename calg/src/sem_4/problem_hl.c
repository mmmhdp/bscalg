#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  struct node_t *next;
  int data;
};

int list_is_a_loop(struct node_t *top) {
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
      return 1;
    if (!hare->next)
      break;
    if (!hare->next->next)
      break;

    tortoise = tortoise->next;
    hare = hare->next->next;
  }
  return 0;
}

void free_list(struct node_t *top) {
  if (!top)
    return;

  // Detect cycle using Floyd's cycle-finding algorithm.
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

  // If a cycle is found, find the start of the loop.
  if (hasCycle) {
    tortoise = top;
    while (tortoise != hare) {
      tortoise = tortoise->next;
      hare = hare->next;
    }
    // Now 'tortoise' (or 'hare') is at the start of the cycle.
    // Find the node just before the start of the loop.
    struct node_t *prev = tortoise;
    while (prev->next != tortoise)
      prev = prev->next;
    // Break the loop.
    prev->next = NULL;
  }

  // Free the list safely.
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

  outcome = list_is_a_loop(top);
  printf("%d\n", outcome);

  free_list(top);
}
