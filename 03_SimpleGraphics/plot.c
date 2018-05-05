/* zcc +zx -vn -startup=31 -clib=sdcc_iy plot.c -o plot -create-app */

#include <arch/zx.h>
#include <stdlib.h>

void plot(unsigned char x, unsigned char y)
{
   *zx_pxy2saddr(x,y) |= zx_px2bitmask(x);
}

int main(void)
{
   unsigned char i;

   zx_cls(PAPER_WHITE);
   
   for( i=0; i<15; i++ )
   {
      plot(rand()%256, rand()%192);      
   }
   return 0;
}
