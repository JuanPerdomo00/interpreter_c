#include "utils/vector.h"
#include <assert.h>
#include <stdio.h>

// proving that can hold the required values
void test1() {
    printf("proving that can hold the values as an static array\n");
    int arr[] = {1,2,3,4,5,6,7,8,9,10};

    Vector v = vector_create(sizeof(int),10);
    for(int i = 1; i < 11; ++i) {
      vector_push(&v, &i);
    }

  bool flag = true;
  for(int i = 0; i < 10; ++i) {
    if(*(int*)vector_get(&v,i) != arr[i]) flag = false;
  }

  assert(flag);
}


int main() {

  test1();
  printf("test1 completed\n");

  return 0;
}

