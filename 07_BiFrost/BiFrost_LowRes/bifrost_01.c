/* 
 * zcc +zx -vn -startup=31 -clib=sdcc_iy bifrost_01.c ctile.asm -o bifrost_01
 * z88dk-appmake +zx -b bifrost_01_CODE.bin -o bifrost_01_code.tap --noloader --org 32768 --blockname bifrost_01_code
 * z88dk-appmake +zx -b bifrost_01_BIFROSTL.bin -o bifrostl.tap --noloader --org 58625 --blockname bifrostl
 * cat bifrost_loader.tap bifrost_01_code.tap bifrostl.tap > bifrost_01.tap
 */

#pragma output REGISTER_SP  = -1
#pragma output CLIB_MALLOC_HEAP_SIZE = 0

#include <arch/zx.h>
#include <arch/zx/bifrost_l.h>

extern unsigned char ctiles[];

int main()
{
  unsigned char blank_tile_index;
  unsigned char row, col;

  BIFROSTL_resetTileImages(_ctiles);

  for(blank_tile_index = 0; blank_tile_index < 81; blank_tile_index++) {
    BIFROSTL_tilemap[blank_tile_index] = BIFROSTL_DISABLED;
  }

  zx_cls(PAPER_WHITE);

  for(row = 1; row <=17; row++)
    for(col = 1; col <= 17; col+=2)
      BIFROSTL_fillTileAttrL(row, col, INK_WHITE+(8*INK_WHITE));

  BIFROSTL_setTile(0, 0, 0+BIFROSTL_STATIC);

  BIFROSTL_start();

  while(1); 
}
