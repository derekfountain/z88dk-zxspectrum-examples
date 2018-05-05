/* zcc +zx -vn -startup=31 -clib=sdcc_iy line.c -o line -create-app */

#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>

void plot(unsigned char x, unsigned char y)
{
   *zx_pxy2saddr(x,y) |= zx_px2bitmask(x);
}

void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
   unsigned char dx  = abs(x1-x0);
   unsigned char dy  = abs(y1-y0);
   signed   char sx  = x0<x1 ? 1 : -1;
   signed   char sy  = y0<y1 ? 1 : -1;
   int           err = (dx>dy ? dx : -dy)/2;
   int           e2;

   while (1)
   {
      plot(x0,y0);
      if (x0==x1 && y0==y1) break;
    
      e2 = err;
      if (e2 >-dx) { err -= dy; x0 += sx; }
      if (e2 < dy) { err += dx; y0 += sy; }
   }
}

int main(void)
{
  unsigned char i;

  zx_cls(PAPER_WHITE);
 
  for( i=0; i<15; i++ )
  {
    line(rand()%256, rand()%192, rand()%256, rand()%192);
  }
  return 0;
} 
