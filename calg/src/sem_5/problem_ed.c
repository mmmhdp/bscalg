#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
show_table (int **t, int n, int m)
{
  int i, j;
  for (i = 0; i < n; i++)
    {
      for (j = 0; j < m; j++)
        {
          printf ("%3d ", t[i][j]);
        }
      printf ("\n");
    }
}

int
min (int a, int b)
{
  return (a - b < 0) ? a : b;
}

int
up_cmp (char *s1, char *s2, int i, int j, int up)
{
  return (s1[i] == s2[j]) ? 0 : up;
}

int
calc_ed (char *s1, char *s2, int add, int del, int up)
{
  int i, j, **V, ed;

  V = malloc (sizeof (int *) * (strlen (s1) + 1));
  for (i = 0; i < (int)strlen (s1) + 1; i++)
    V[i] = malloc (sizeof (int) * (strlen (s2) + 1));

  for (i = 0; i < (int)strlen (s1) + 1; i++)
    for (j = 0; j < (int)strlen (s2) + 1; j++)
      {
        if (i == 0 || j == 0)
          V[i][j] = (j == 0) ? i : j;
        else
          {
            V[i][j] = min (
                V[i - 1][j] + del,
                min (V[i][j - 1] + add,
                     V[i - 1][j - 1] + up_cmp (s1, s2, i - 1, j - 1, up)));
          }
      }

  ed = V[strlen (s1)][strlen (s2)];

  for (i = 0; i < (int)strlen (s1) + 1; i++)
    free (V[i]);
  free (V);

  return ed;
}

int
main (void)
{
  int add_cost, delete_cost, update_cost, s1_len, s2_len, edit_range;
  char *s1, *s2;

  scanf ("%d", &add_cost);
  scanf ("%d", &delete_cost);
  scanf ("%d", &update_cost);

  scanf ("%d", &s1_len);
  s1 = malloc (s1_len + 1);
  scanf ("%s", s1);

  scanf ("%d", &s2_len);
  s2 = malloc (s2_len + 1);
  scanf ("%s", s2);

  edit_range = calc_ed (s1, s2, add_cost, delete_cost, update_cost);
  printf ("%d\n", edit_range);

  free (s1);
  free (s2);
}
