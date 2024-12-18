#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int size;
  char *table;
  int *primes;
  int nprimes;
} Sieve;

Sieve *init_sieve(Sieve *siev) {
  int basic_size = 2;
  char *psiev_table;
  int *pprimes;
  siev->size = basic_size;
  siev->nprimes = 0;
  psiev_table = calloc(basic_size, sizeof(char));
  pprimes = calloc(basic_size, sizeof(char));
  if (!psiev_table || !pprimes) {
    printf("Invalid allocation for sieve");
    abort();
  }
  siev->table = psiev_table;
  siev->primes = pprimes;
  siev->table[0] = 1;
  siev->table[1] = 1;

  return siev;
}

Sieve *expand_sieve(Sieve *siev, int new_size) {
  char *new_table =
      realloc(siev->table, (siev->size + new_size) * sizeof(char));
  int *new_primes =
      realloc(siev->primes, (siev->size + new_size) * sizeof(int));
  if (!new_table || !new_primes) {
    printf("Invalid allocation for sieve");
    abort();
  }

  siev->primes = new_primes;
  siev->table = new_table;
  siev->size = new_size;

  return siev;
}

void free_sieve(Sieve *siev) {
  free(siev->table);
  free(siev->primes);
  siev->size = 0;
  siev->nprimes = 0;
}

int main() {
  int nitems = {0}, nprimes = {0}, counter = {0};
  Sieve siv;

  nitems = scanf("%d", &counter);

  if (nitems != 1) {
    printf("Invalid argument.");
    abort();
  }

  init_sieve(&siv);
  expand_sieve(&siv, counter * (int)log2(counter) + 10);
  for (int i = 0; i <= counter; i++) {
    if (siv.table[i] == 1)
      continue;
    else {
      siv.primes[nprimes] = i;
      nprimes += 1;
      siv.nprimes = nprimes;
    }
    for (int t = i; t < siv.size; t += i) {
      siv.table[t] = 1;
    }
  }

#ifdef DEBUG
  for (int i = 0; i < nprimes; i++) {
    printf("%d\n", siv.primes[i]);
  }
#endif
  printf("%d\n", siv.nprimes);
  free_sieve(&siv);
}
