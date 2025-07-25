#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2

void
mcopy (unsigned (*MDist)[N], unsigned const (*MSrc)[N])
{
  int i, j;

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
          MDist[i][j] = MSrc[i][j];
        }
    }
}

void
mshow (unsigned const (*M)[N])
{
  int i, j;
  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
          printf ("%u ", M[i][j]);
        }
      printf ("\n");
    }
}

void
miniteig (unsigned (*M)[N])
{
  int i, j;

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
          M[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void
mtranspose (unsigned (*tM)[N], unsigned const (*M)[N])
{
  int i, j;

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
          tM[i][j] = (i == j) ? M[i][j] : M[j][i];
        }
    }
}

unsigned
vmultmod (unsigned const a[N], unsigned const b[N], unsigned const m)
{
  int i;
  unsigned res;

  res = 0;
  for (i = 0; i < N; i++)
    res = (res + ((a[i] % m) * (b[i] % m)) % m) % m;

  return res;
}

void
mmultmod (unsigned (*C)[N], const unsigned (*A)[N], const unsigned (*B)[N],
          unsigned m)
{
  int i, j;

  unsigned (*tB)[N] = calloc (N, sizeof (unsigned[N]));
  mtranspose (tB, B);

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
          C[i][j] = vmultmod (A[i], tB[j], m);
        }
    }

  free (tB);
}

void
powNxN (unsigned (*A)[N], unsigned k, unsigned m)
{
  unsigned (*TmpM)[N] = calloc (N, sizeof (unsigned[N]));
  unsigned (*Accum)[N] = calloc (N, sizeof (unsigned[N]));
  unsigned (*Res)[N] = calloc (N, sizeof (unsigned[N]));

  miniteig (Res);
  mcopy (Accum, A);

  while (k > 0)
    {
      if (k % 2 == 1)
        {
          mmultmod (TmpM, Res, Accum, m);
          mcopy (Res, TmpM);
          k -= 1;
        }
      else
        {
          mmultmod (TmpM, Accum, Accum, m);
          mcopy (Accum, TmpM);
          k /= 2;
        }
    }

  mcopy (A, Res);

  free (TmpM);
  free (Accum);
  free (Res);
}

int
main (void)
{
  int nitems, i, j, arr[N][N], k, m;

  nitems = scanf ("%d", &k);
  assert (nitems == 1);

  nitems = scanf ("%d", &m);
  assert (nitems == 1);

  for (i = 0; i < N; ++i)
    {
      for (j = 0; j < N; ++j)
        {
          nitems = scanf ("%d", &(arr[i][j]));
          assert (nitems == 1);
        }
    }

  powNxN (arr, k, m);

  for (i = 0; i < N; ++i)
    {
      for (j = 0; j < N; ++j)
        {
          printf ("%d ", arr[i][j]);
        }
      printf ("\n");
    }
}
