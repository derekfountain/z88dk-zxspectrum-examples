/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy bifrost_03.c ctile.asm -o bifrost_03
 * z88dk-appmake +glue -b bifrost_03 --filler 0 --clean
 * z88dk-appmake +zx -b bifrost_03__.bin --org 32768 --blockname bifrost_03 -o bifrost_03.tap
 */

#pragma output REGISTER_SP  = -1
#pragma output CLIB_MALLOC_HEAP_SIZE = 0

#include <string.h>
#include <arch/zx.h>
#include <arch/zx/bifrost2.h>

extern unsigned char ctiles[];

int main( void )
{
  unsigned char line, col;

  zx_border(INK_BLACK);

  BIFROST2_install();

  BIFROST2_resetTileImages(_ctiles);

  memset(BIFROST2_tilemap, BIFROST2_DISABLED, 110);

  zx_cls(PAPER_WHITE);

  for(line = 0; line <=192; line++) {
    for(col = 0; col <= 20; col++) {
      BIFROST2_fillTileAttrH(line, col, INK_WHITE+PAPER_WHITE);
    }
  }

  BIFROST2_start();

  col = 0;

  while(1) {
    for( line=0; line<=192; line++ ) {

      BIFROST2_halt();

      BIFROST2_fillTileAttrH(line-1,10,INK_WHITE+(8*INK_WHITE));
      BIFROST2_drawTileH(line, 10, 0);

      BIFROST2_fillTileAttrH(80,col-1,INK_WHITE+(8*INK_WHITE));
      BIFROST2_drawTileH(80, col, 0);
      if( ++col == 21 )
        col = 0;
    }  
  }
}
