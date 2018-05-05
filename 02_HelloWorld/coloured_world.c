/*
 * zcc +zx -vn -startup=1 -clib=sdcc_iy coloured_world.c -o coloured_world -create-app
 */

#include <arch/zx.h>
#include <stdio.h>

int main()
{
  zx_cls(PAPER_WHITE);

  puts("\x10\x33\x16\x0A\x0C\x12\x01Hello\x12\x00, world!");

  return 0;
}
