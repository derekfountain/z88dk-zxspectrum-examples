/* zcc +zx -vn -startup=31 -clib=sdcc_iy key_press.c -o key_press -create-app */ 

#include <arch/zx.h>
#include <input.h>

int main()
{

  while( 1 )
  {
    zx_border(INK_WHITE);

    in_wait_key();

    zx_border(INK_BLUE);

    in_wait_nokey();
  }
}
