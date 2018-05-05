/*
 * zcc +zx -vn -startup=1 -clib=sdcc_iy hello_world.c -o hello_world -create-app
 */

#include <arch/zx.h>
#include <stdio.h>

int main()
{
  zx_cls(PAPER_WHITE);

  puts("Hello, world!");

  return 0;
}
