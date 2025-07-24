#include <stdio.h>

// a, b, c
static const int STATES[3][5]
    = { { 1, 1, 5, 5, 5 }, { 2, 2, 4, 4, 5 }, { 5, 3, 3, 3, 5 } };

static const int ACCEPTING_STATES[5] = { 0, 1, 1, 1, 0 };

int
main (void)
{
  int state = 1;

  char c;
  int ic;
  while (scanf ("%c", &c) == 1)
    {
      ic = c - 'a';
      if (c == 'a' || c == 'b' || c == 'c')
        state = STATES[ic][state - 1];
    }

  printf ("%d\n", ACCEPTING_STATES[state - 1]);
}
