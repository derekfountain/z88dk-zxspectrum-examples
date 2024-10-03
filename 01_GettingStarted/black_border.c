/*
 * Build with the newer zsdcc compiler and libraries with:
 *
 * zcc +zx -vn -clib=sdcc_iy -startup=31 black_border.c -o black_border -create-app
 */

#include <arch/zx.h>

int main( void )
{
  zx_border(INK_BLACK);
  return 0;
}
