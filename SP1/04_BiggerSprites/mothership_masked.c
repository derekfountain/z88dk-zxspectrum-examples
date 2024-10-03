/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy mothership_masked.c mothership_sprite.asm -o mothership_masked -create-app
 */

#pragma output REGISTER_SP = 0xD000

#include <z80.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

extern unsigned char mothership_col1[];
extern unsigned char mothership_col2[];
extern unsigned char mothership_col3[];
extern unsigned char mothership_col4[];
extern unsigned char mothership_col5[];
extern unsigned char mothership_col6[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

int main( void )
{
  struct sp1_ss  *mothership_sprite;
  unsigned char x;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  'X' );
  sp1_Invalidate(&full_screen);
 
  mothership_sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 4, (int)mothership_col1, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col2, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col3, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col4, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col5, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col6, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  x=0;
  while(1)
  {
    sp1_MoveSprPix(mothership_sprite, &full_screen, 0, x++, 0);

    z80_delay_ms(10);
    sp1_UpdateNow();    
  }
}
