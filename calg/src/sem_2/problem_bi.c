#include <stdio.h>
#include <stdlib.h>

#define ENDL printf ("\n");

int
main (void)
{
  int n, target_bit;
  int *nums;

  scanf ("%d", &n);
  nums = malloc (sizeof (int) * n);

  for (int i = 0; i < n; i++)
    {
      scanf ("%d", &(nums[i]));
    }

  scanf ("%d", &target_bit);

  int tidx = target_bit / 8;
  int ridx = target_bit % 8;
  nums[tidx] = nums[tidx] ^ (1 << ridx);
  for (int i = 0; i < n; i++)
    {
      printf ("%d ", nums[i]);
    }

#if 0
  printf("NUMBER OF NUMS IS %d", n);
  ENDL
  for (int i = 0; i < n ; i++){
    printf("%d ",nums[i]);
  }
  ENDL
  printf("TARGET BIT IS %d", target_bit);
  ENDL

  int tidx = target_bit / 8;
  int ridx = target_bit % 8;
  nums[tidx] = nums[tidx] ^ (1 << ridx);

  
  printf("UPDATED VERSION");
  ENDL
  for (int i = 0; i < n ; i++){
    printf("%d ",nums[i]);
  }
  ENDL
#endif
}
