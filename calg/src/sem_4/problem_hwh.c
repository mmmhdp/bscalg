#define _GNU_SOURCE

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define HSH_DBG

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__)          \
    || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__)
#define get16bits(d) (*((const uint16_t *)(d)))
#endif

#if !defined(get16bits)
#define get16bits(d)                                                          \
  ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)                             \
   + (uint32_t)(((const uint8_t *)(d))[0]))
#endif

uint32_t
SuperFastHash (const char *data, int len)
{
  uint32_t hash = len, tmp;
  int rem;

  if (len <= 0 || data == NULL)
    return 0;

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (; len > 0; len--)
    {
      hash += get16bits (data);
      tmp = (get16bits (data + 2) << 11) ^ hash;
      hash = (hash << 16) ^ tmp;
      data += 2 * sizeof (uint16_t);
      hash += hash >> 11;
    }

  /* Handle end cases */
  switch (rem)
    {
    case 3:
      hash += get16bits (data);
      hash ^= hash << 16;
      hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
      hash += hash >> 11;
      break;
    case 2:
      hash += get16bits (data);
      hash ^= hash << 11;
      hash += hash >> 17;
      break;
    case 1:
      hash += (signed char)*data;
      hash ^= hash << 10;
      hash += hash >> 1;
    }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

#define HSH_TB_CP 1048576
#define HSH_P 9973
#define HSH_A 123
#define HSH_B 23

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
lstnd_show (LstNd *top)
{
  if (!top)
    {
      printf ("||");
      return;
    }
  printf ("->%s->", top->v);
  lstnd_show (top->nxt);
}

void
lstnd_free (LstNd *top)
{
  if (!top)
    {
      free (top);
      return;
    }

  lstnd_free (top->nxt);

  free (top->v);
  free (top);
}

void
lstnd_update (LstNd **root, char *key)
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

  lstnd_update (&top->nxt, key);
}

int
lstnd_cnt (LstNd *top)
{
  int cnt = 0;
  while (top)
    {
      cnt++;
      top = top->nxt;
    }

  return cnt;
}

uint32_t
ht_hsf (char *key)
{
  uint32_t hs;
  hs = SuperFastHash (key, strlen (key)) % HSH_TB_CP;

#ifdef HSH_DBG
  printf ("Hash for |%s| is equal to |%lu|\n", key, hs);
#endif

  return hs;
}

unsigned long
ht_hsf_naive (char *key)
{
  int i;
  unsigned long hs;
  char c;

  if (strlen (key) == 0)
    return 0;

  assert (strlen (key) != 0);
  for (i = 0; i < (int)strlen (key); i++)
    {
      c = key[i];
      hs = ((HSH_A * c + HSH_B) % HSH_P) % HSH_TB_CP;
    }

#ifdef HSH_DBG
  printf ("Hash for |%s| is equal to |%lu|\n", key, hs);
#endif

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
  uint32_t i;
  for (i = 0; i < HSH_TB_CP; i++)
    {
      lstnd_free (ht->tb[i]);
    }

  free (ht);
}

void
ht_add (Ht *ht, char *key)
{
  uint32_t hs;

  hs = ht_hsf (key);
  lstnd_update (&ht->tb[hs], key);
}

int
ht_cnt (Ht *ht, char *key)
{
  uint32_t hs;
  int cnt;

  hs = ht_hsf (key);
  if (!(ht->tb[hs]))
    return 0;
  cnt = lstnd_cnt (ht->tb[hs]);

  return cnt;
}

void
ht_show (Ht *ht)
{
  uint32_t i;
  for (i = 0; i < HSH_TB_CP; i++)
    if (ht->tb[i])
      {
        printf ("Key %d is presented\n", i);
        printf ("()");
        lstnd_show (ht->tb[i]);
        printf ("\n");
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
  Ht *ht;

  ht = ht_init ();
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
      ht_add (ht, w);
      free (w);
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
      cnt = ht_cnt (ht, w);
      printf ("%d ", cnt);
      free (w);
    }
  printf ("\n");

#ifdef HSH_DBG
  ht_show (ht);
#endif

  fclose (sqd);
  free (sq);

  ht_free (ht);
}
#endif
