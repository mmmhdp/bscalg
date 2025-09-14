void
swap (int *arr, int is, int id)
{
  int tmp;
  tmp = arr[is], arr[is] = arr[id], arr[id] = tmp;
}

int
foo (int *arr, int li, int ri)
{
  if (li >= ri)
    return ri;

  while (li < ri)
    {
      if (arr[li] <= arr[ri])
        {
          ri -= 1;
        }
      else
        {
          swap (arr, li, ri);
          ri -= 1;
          li += 1;
        }
    }

  return ri;
}

int
main (void)
{
}
