/* zcc +zx -vn -startup=1 -clib=sdcc_iy scancodes.c -o scancodes -create-app */
 
#include <stdio.h>
#include <input.h>
#include <arch/zx.h>

int main()
{
  zx_cls(PAPER_WHITE);
  while( 1 ) {

    printf("\x16\x01\x01");

    printf("Scan for q returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_q ));
    printf("Scan for a returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_a ));
    printf("Scan for o returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_o ));
    printf("Scan for p returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_p ));
    printf("Scan for <sp> returns 0x%04X\n\n", in_key_pressed( IN_KEY_SCANCODE_SPACE ));
  }
}
