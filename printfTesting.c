#include <stdio.h>

/* This results in a compilation warning and a segmentation fault when run. */

int main(int argc, char** argv)
{
  printf("Testing the wrong stuff %s\n", 10);
  return 0;
}
