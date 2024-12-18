#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct point_t{
  int x,y;
}point;

typedef struct triangle_t{
  struct point_t pts [3];
}triangle;

int abs_2d_vector_product(int x_1, int y_1, int x_2, int y_2){ 
  int product; 
  product = abs((x_1*y_2) - (x_2*y_1));
  return product;
}

int double_area(const triangle * tr){
  int area = abs_2d_vector_product(
  tr->pts[0].x - tr->pts[1].x,
  tr->pts[0].y - tr->pts[1].y,
  tr->pts[0].x - tr->pts[2].x,
  tr->pts[0].y - tr->pts[2].y
  );
  return area;
}


void test_1(){
  int res;

  triangle tr = {
    .pts = {
      {1,1},
      {3,1},
      {3,4},
    }
  };

  res = double_area(&tr);
  assert(res == 6);
  printf("%d\n", res);
}


void test_2(){
  int res;

  triangle tr = {
    .pts = {
      {-1,1},
      {-3,1},
      {-3,4},
    }
  };

  res = double_area(&tr);
  assert(res == 6);
  printf("%d\n", res);
}


void test_3(){
  int res;

  triangle tr = {
    .pts = {
      {-1,-1},
      {-3,-1},
      {-3,-4},
    }
  };

  res = double_area(&tr);
  assert(res == 6);
  printf("%d\n", res);
}

void test_4(){
  int res;

  triangle tr = {
    .pts = {
      {1,-1},
      {3,-1},
      {3,-4},
    }
  };

  res = double_area(&tr);
  assert(res == 6);
  printf("%d\n", res);
}

#if 0
int main(){
  test_1();
  test_2();
  test_3();
  test_4();

  return 0;
}
#else
int main() {
  int res;
  triangle tr;
  res = scanf("%d%d%d%d%d%d",
                &tr.pts[0].x,
                &tr.pts[0].y,
                &tr.pts[1].x,
                &tr.pts[1].y,
                &tr.pts[2].x,
                &tr.pts[2].y);
  assert(res == 6);
  res = double_area(&tr);
  printf("%d\n", res);
}
#endif


