/* zcc +zx -vn -startup=1 -clib=sdcc_iy joy_input.c -o joy_input -create-app */
 
#include <stdio.h>
#include <input.h>
#include <arch/zx.h>

int main( void )
{
  uint16_t kempston_input;

  zx_cls(PAPER_WHITE);
  while( 1 ) {

    kempston_input = in_stick_kempston();

    printf("\x16\x01\x01");

    printf("Joystick input value is 0x%04X\n", kempston_input);
  }
}
