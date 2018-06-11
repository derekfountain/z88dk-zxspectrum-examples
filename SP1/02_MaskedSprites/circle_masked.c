/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy circle_masked.c circle_sprite_masked.asm -o circle_masked -create-app
 */


#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>
#include <z80.h>
#include <im2.h>
#include <string.h>

IM2_DEFINE_ISR(isr) {}
#define TABLE_HIGH_BYTE        ((unsigned int)0xD0)
#define JUMP_POINT_HIGH_BYTE   ((unsigned int)0xD1)

#define UI_256                 ((unsigned int)256)
#define TABLE_ADDR             ((void*)(TABLE_HIGH_BYTE*UI_256))
#define JUMP_POINT             ((unsigned char*)( (unsigned int)(JUMP_POINT_HIGH_BYTE*UI_256) + JUMP_POINT_HIGH_BYTE ))

extern unsigned char circle_masked[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

int main()
{
  struct sp1_ss  *circle_sprite;
  unsigned char x;

  memset( TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257 );
  z80_bpoke( JUMP_POINT,   195 );
  z80_wpoke( JUMP_POINT+1, (unsigned int)isr );
  im2_init( TABLE_ADDR );
  intrinsic_ei();

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  'X' );
  sp1_Invalidate(&full_screen);
 
  circle_sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 2, (int)circle_masked, 0);

  sp1_AddColSpr(circle_sprite, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);

  x=0;
  while(1) {
    sp1_MoveSprPix(circle_sprite, &full_screen, 0, x++, 4);
    intrinsic_halt();
    sp1_UpdateNow();
  }
}
