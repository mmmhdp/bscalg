#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int size;
  char *table;
} Sieve;

Sieve *init_sieve(Sieve *siev) {
  int basic_size = 2;
  char *psiev_table;
  siev->size = basic_size;
  psiev_table = calloc(basic_size, sizeof(char));
  if (!psiev_table) {
    printf("Invalid allocation for sieve");
    abort();
  }
  siev->table = psiev_table;
  siev->table[0] = 1;
  siev->table[1] = 1;

  return siev;
}

Sieve *expand_sieve(Sieve *siev, int new_size) {
  char *new_table =
      realloc(siev->table, (siev->size + new_size) * sizeof(char));
  if (!new_table) {
    printf("Invalid allocation for sieve");
    abort();
  }

  siev->table = new_table;
  siev->size = new_size;

  return siev;
}

void free_sieve(Sieve *siev) { free(siev->table); }

int main() {
  int nitems = {0}, counter = {0}, i_n = {0};
  Sieve siv;

  nitems = scanf("%d", &counter);

  if (nitems != 1) {
    printf("Invalid argument.");
    abort();
  }

  init_sieve(&siv);
  expand_sieve(&siv, counter * (int)log2(counter) + 10);
  for (int i = 0;; i++) {
    if (siv.table[i] == 1)
      continue;
    else {
      counter -= 1;
      if (counter == 0) {
        i_n = i;
        goto final;
      }
    }
    for (int t = i; t < siv.size; t += i) {
      siv.table[t] = 1;
    }
  }

final:
  printf("%d", i_n);
  free_sieve(&siv);
}
