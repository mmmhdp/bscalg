#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HSH_TB_CP 100
#define HSH_P 17
#define HSH_A 3
#define HSH_B 30

typedef struct lstnd
{
  struct lstnd *nxt;
  char *v;
} LstNd;

typedef struct ht
{
  LstNd *tb[HSH_TB_CP];
} Ht;

void
lstnd_show(LstNd * top)
{
  if (!top)
  {
    printf("||");
    return;
  }
  printf("->%s->", top->v);
  lstnd_show (top->nxt);
}

void
lstnd_free (LstNd *top)
{
  if (!top)
  {
    free(top);
    return;
  }

  lstnd_free (top->nxt);

  free (top->v);
  free (top);
}

void
lstnd_update(LstNd **root, char * key)
{
  LstNd *top = *root;
  if (!top)
    {
      top = calloc (1, sizeof (LstNd));
      top->v = malloc (strlen (key) + 1);

      strcpy (top->v, key);

      *root = top;
      return;
    }

  if (!(top->nxt))
    {
      top->nxt = calloc (1, sizeof (LstNd));
      top->nxt->v = malloc (strlen (key) + 1);

      strcpy (top->nxt->v, key);
      return;
    }

  lstnd_update(&top->nxt, key);
}

int 
lstnd_cnt(LstNd *top)
{
  int cnt = 0;
  while (top)
  {
    cnt++;
    top = top->nxt;
  }

  return cnt;
}

unsigned long
ht_hsf (char *key)
{
  int i;
  unsigned long hs;
  char c;

  if (strlen(key) == 0)
    return 0;

  assert (strlen (key) != 0);
  for (i = 0; i < (int)strlen (key); i++)
    {
      c = key[i];
      hs = ((HSH_A * c + HSH_B) % HSH_P) % HSH_TB_CP;
    }

  return hs;
}

Ht *
ht_init (void)
{
  Ht *h = calloc (1, sizeof (Ht));
  return h;
}

void
ht_free (Ht *ht)
{
  int i;
  for (i = 0; i < HSH_TB_CP; i++)
    {
      lstnd_free (ht->tb[i]);
    }

  free (ht);
}

void
ht_add (Ht *ht, char *key)
{
  unsigned hs;

  hs = ht_hsf (key);
  lstnd_update(&ht->tb[hs], key);
}

int
ht_cnt (Ht * ht, char *key)
{
  int hs;
  int cnt;

  hs = ht_hsf(key);
  if (!(ht->tb[hs]))
    return 0;
  cnt = lstnd_cnt(ht->tb[hs]);

  return cnt;
}

void
ht_show(Ht *ht)
{
  int i;
  for (i = 0; i < HSH_TB_CP; i++)
    if (ht->tb[i])
    {
      printf("Key %d is presented\n", i);
      printf("()");
      lstnd_show(ht->tb[i]);
      printf("\n");
    }

}

char *
trim (char *s)
{
  char *ns;
  int i, j, k, len, nssz;

  len = strlen (s);

  i = len - 1;
  while (i >= 0)
    if (isspace (s[i]))
      i--;
    else
      break;

  j = 0;
  while (j < i)
    if (isspace (s[j]))
      j++;
    else
      break;

  nssz = i - j + 1 + 1;
  ns = malloc (nssz);

  for (k = 0; k < nssz; k++)
    ns[k] = s[j++];
  ns[nssz - 1] = '\0';

  return ns;
}

char *
read_key (FILE *fd)
{
  int tmp;
  unsigned long wsize;
  char *raw_w, *w;

  raw_w = NULL;
  w = NULL;
  wsize = 0;

  tmp = getdelim (&raw_w, &wsize, ' ', fd);

  if (tmp == -1)
    {
      free (raw_w);
      return w;
    }

  w = trim (raw_w);
  free (raw_w);

  return w;
}

#if 0
int
main (void)
{
  Ht *h = ht_init ();

  ht_add(h, "Hello");
  ht_add(h, "World");
  ht_add(h, "Hello");
  ht_add(h, "Hello");

  ht_show(h);

  ht_free (h);
}

#else

int
main (void)
{
  int nq, cnt;
  unsigned long sbfsz, sqbfsz, slen, sqlen;
  char *s, *sq, *w;
  Ht * ht;

  ht = ht_init();
  s = sq = NULL;

  scanf ("%d ", &nq);
  scanf ("%lu ", &sbfsz);
  slen = getline (&s, &sbfsz, stdin);

  FILE *sd = fmemopen (s, slen, "r");
  for (;;)
    {
      w = read_key (sd);
      if (!w)
        break;
      ht_add(ht, w);
    }

  fclose (sd);
  free (s);

  scanf ("%lu ", &sqbfsz);
  sqlen = getline (&sq, &sqbfsz, stdin);

  FILE *sqd = fmemopen (sq, sqlen, "r");
  for (;;)
    {
      w = read_key (sqd);
      if (!w)
        break;
      cnt = ht_cnt(ht, w);
      printf ("%d\n", cnt);
      free (w);
    }

#ifdef HSH_DBG
  ht_show(ht);
#endif

  fclose (sqd);
  free (sq);

  ht_free(ht);
}
#endif
