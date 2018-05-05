/*
 * zcc +zx -vn -startup=1 -clib=sdcc_iy question.c -o question -create-app
 */

#include <arch/zx.h>
#include <stdio.h>

int main()
{
  unsigned char name[15];

  zx_cls(PAPER_WHITE);

  printf("What is your name? ");
  fgets(name,15,stdin);
  printf("Hello, %s", name);

  getchar();

  return 0;
}
