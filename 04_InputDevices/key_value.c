/* zcc +zx -vn -startup=0 -clib=sdcc_iy key_value.c -o key_value -create-app */

#include <stdio.h>
#include <input.h>

int main()
{
  unsigned char c;

  while( 1 )
  {
    in_wait_key();
    c = in_inkey();
    in_wait_nokey();

    printf("Key pressed is %c (0x%02X)\n", c, c);
  }
}
