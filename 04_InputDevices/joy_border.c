/* zcc +zx -vn -startup=31 -clib=sdcc_iy joy_border.c -o joy_border -create-app */
 
#include <input.h>
#include <arch/zx.h>

int main( void )
{
  uint16_t kempston_input;

  zx_cls(PAPER_WHITE);
  zx_border(INK_WHITE);
  while( 1 ) {

    kempston_input = in_stick_kempston();

    if( kempston_input & IN_STICK_UP )
      zx_border(INK_BLACK);

    if( kempston_input & IN_STICK_DOWN )
      zx_border(INK_BLUE);

    if( kempston_input & IN_STICK_LEFT )
      zx_border(INK_RED);

    if( kempston_input & IN_STICK_RIGHT )
      zx_border(INK_MAGENTA);

    if( kempston_input & IN_STICK_FIRE )
      zx_border(INK_GREEN);
  }
}
