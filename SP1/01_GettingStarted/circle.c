#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <arch/zx/sp1.h>

extern unsigned char circle[];

int main()
{
  struct sp1_Rect full_screen = {0, 0, 32, 24};
  struct sp1_ss  *circle_sprite;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  ' ' );
  sp1_Invalidate(&full_screen);
 
  circle_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int)circle, 0);

  sp1_AddColSpr(circle_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, (int)circle, 0);

  sp1_MoveSprAbs(circle_sprite, &full_screen, 0, 0, 0, 0, 0);

  sp1_UpdateNow();

  while(1);
}
