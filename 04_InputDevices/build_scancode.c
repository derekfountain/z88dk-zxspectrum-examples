/* zcc +zx -vn -startup=1 -clib=sdcc_iy build_scancode.c -o build_scancode -create-app */
 
#include <stdio.h>
#include <input.h>
#include <arch/zx.h>

int main()
{
  /* Space, plus top bit set means CAPS */
  uint16_t break_scancode = IN_KEY_SCANCODE_SPACE | 0x8000;

  zx_cls(PAPER_WHITE);
  while( 1 ) {

    printf("\x16\x01\x01");

    printf("Scan for <break> returns 0x%04X\n",  in_key_pressed( break_scancode ));
  }
}
