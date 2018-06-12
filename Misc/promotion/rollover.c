/*
 * zcc +zx -vn -m -startup=1 -clib=sdcc_iy rollover.c -o rollover -create-app
 */

#include <stdio.h>
#include <stdint.h>

int main()
{
  uint8_t   test;

  test = 0;
  while( 1 )
    {
      /*
       * Demonstrate that a ticker about to wrap at MAX
       * can be compared to zero to see the wrap. The casting
       * is required because the compiler will implicitly
       * type promote to unsigned int for the +, and 256
       * isn't 0. :)
       */

      if( (unsigned char)(test+1) == 0 ) {
        printf("Rolled over at %u\n", test);
	break;
      }

      test++;
    }

  return 0;
}
 
