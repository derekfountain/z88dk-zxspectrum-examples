/*
 * Graphics aren't ported to the new libs in Z88DK as yet.
 * This uses the classic libraries, which aren't covered by
 * the getting started guide.
 *
 * zcc +zx -vn -lndos plot_classic_lib.c -o plot_classic_lib -create-app
 */

#include <graphics.h>

int main()
{
  register unsigned char i;

  for(i=1; i<=175; i+=2)
    {
      plot(0,i);
      drawr(255,0);
    }
}
