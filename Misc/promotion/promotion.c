/*
 * zcc +zx -vn -m -startup=1 -clib=sdcc_iy promotion.c -o promotion -create-app
 * gcc -o promotion promotion.c
 */

#include <stdio.h>

/*
 * It's the + operator which triggers the compiler's
 * promotion. It promotes the char to an int to do
 * the addition, so it's the casting back which
 * keeps the result at 8 bits. Hence example 2 below
 * the one which behaves as expected.
 */

int main( void )
{
  volatile unsigned char test;

  test = 255;

  /*
   * The SDCC compiler uses BC for 'test' here:
   *
   *  ld      bc,#0x00ff
   *  inc     bc
   *  ld      a, b
   *  or      a,c
   *  jr      NZ,l_main_00102$
   *
   * It does the same with the last example too.
   *
   * But for the middle example, which behaves how I wanted,
   * it uses:
   *
   *  ld      a, #0xff
   *  inc     a
   *  or      a, a
   *  jr      NZ,l_main_00104$
   *
   * It must recognise when the value will be promoted
   * and hence use a 16bit compare. Which makes sense but
   * it was still a lesson worth learning; just using an
   * unsigned char doesn't guarantee 8 bit code.
   */

  if( test+1 == 0 )
    printf("Rolled over 1\n");

  test = 255;

  if( (unsigned char)(test+1) == 0 )
    printf("Rolled over 2\n");

  test = 255;

  if( (unsigned char)(test)+(unsigned char)(1) == (unsigned char)(0) )
    printf("Rolled over 3\n");

  return 0;
}
