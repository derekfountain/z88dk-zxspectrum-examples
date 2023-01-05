/*
 * zcc +zx -vn -startup=31 -clib=sdcc_iy bifrost_02.c ctile.asm -o bifrost_02
 * z88dk-appmake +zx -b bifrost_02_CODE.bin -o bifrost_02_code.tap --noloader --org 32768 --blockname bifrost_02_code
 * z88dk-appmake +zx -b bifrost_02_BIFROSTH.bin -o bifrosth.tap --noloader --org 57047 --blockname bifrosth
 * cat bifrost_loader.tap bifrost_02_code.tap bifrosth.tap > bifrost_02.tap
 */

#pragma output REGISTER_SP  = -1
#pragma output CLIB_MALLOC_HEAP_SIZE = 0

#include <string.h>
#include <arch/zx.h>
#include <arch/zx/bifrost_h.h>

extern unsigned char ctiles[];

int main()
{
  unsigned char line, col;

  BIFROSTH_resetTileImages(_ctiles);

  memset(BIFROSTH_tilemap, BIFROSTH_DISABLED, 81);

  zx_cls(PAPER_WHITE);

  for(line = 0; line <=160; line++)
    for(col = 0; col <= 18; col++)
      BIFROSTH_fillTileAttrH(line, col, INK_WHITE+(8*INK_WHITE));

  BIFROSTH_setTile(4, 4, 0+BIFROSTH_STATIC);

  BIFROSTH_start();

  line = 0;
  col = 1;
  while(1) {
    BIFROSTH_halt();

    if( line != 16 )
      line++;

    BIFROSTH_drawTileH(line,     col, 0);
    BIFROSTH_drawTileH(160-line, col, 0);
  }

}
