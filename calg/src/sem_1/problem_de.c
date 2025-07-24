#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef enum
{
  VALID,
  INVALID,
} Solution_Status;

typedef struct
{
  int arg1;
  int arg2;
  Solution_Status solution_status;
} Solution_Pair;
int
gcd (int a, int b, int *x, int *y)
{
  int x_hat, y_hat, d;

  if (b == 0)
    {
      *x = 1;
      *y = 0;
      return a;
    }

  d = gcd (b, a % b, &x_hat, &y_hat);

  *x = y_hat;
  *y = x_hat - (a / b) * y_hat;

  return d;
}

void
find_de_solution (int *a, int *b, int *c, Solution_Pair *solution_pair)
{
  int gcd_a_b = gcd (*a, *b, &(solution_pair->arg1), &(solution_pair->arg2));

  if (!(gcd_a_b != 0 && ((*c) % gcd_a_b == 0)))
    {
      solution_pair->solution_status = INVALID;
      return;
    }

  assert (gcd_a_b != 0);

  solution_pair->solution_status = VALID;
  solution_pair->arg1 = solution_pair->arg1 * (*c / gcd_a_b);
  solution_pair->arg2 = solution_pair->arg2 * (*c / gcd_a_b);
}

int
main ()
{
  int a, b, c, nitems;
  Solution_Pair de_solution;

  nitems = scanf ("%d%d%d", &a, &b, &c);

  if (nitems != 3)
    {
      printf ("Invalid input\n");
      exit (1);
    }

  find_de_solution (&a, &b, &c, &de_solution);

  switch (de_solution.solution_status)
    {
    case INVALID:
      printf ("NONE\n");
      break;
    case VALID:
      printf ("%d %d\n", de_solution.arg1, de_solution.arg2);
      break;
    default:
      printf ("Undefined solution status. Unreacheble state is triggered.\n");
      break;
    }

  return 0;
}
