/* zcc +zx -vn -startup=1 -clib=sdcc_iy dollar_scancode.c -o dollar_scancode -create-app */ 

#include <stdio.h>
#include <input.h>
#include <arch/zx.h>

int main( void )
{
  uint16_t dollar_scancode = in_key_scancode('$');

  zx_cls(PAPER_WHITE);
  while( 1 ) {

    printf("\x16\x01\x01");

    printf("Scancode for '$' is 0x%04X\n\n", dollar_scancode);
    printf("Scan for $ returns 0x%04X\n",   in_key_pressed( dollar_scancode ));
  }
}
