/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy bubble_masked.c bubble_masked_sprite.asm -o bubble_masked -create-app
 */

#pragma output REGISTER_SP = 0xD000

#include <z80.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

extern unsigned char bubble_col1[];
extern unsigned char bubble_col2[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

int main()
{
  struct sp1_ss *bubble_sprite;
  unsigned char x,y;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  'X' );
  sp1_Invalidate(&full_screen);
 
  bubble_sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, (int)bubble_col1, 0);
  sp1_AddColSpr(bubble_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)bubble_col2, 0);
  sp1_AddColSpr(bubble_sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  x=0;
  y=80;
  while(1)
  {
    sp1_MoveSprPix(bubble_sprite, &full_screen, 0, x++, y++);

    z80_delay_ms(25);
    sp1_UpdateNow();    
  }
}
