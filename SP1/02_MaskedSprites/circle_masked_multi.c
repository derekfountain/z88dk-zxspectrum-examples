/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 circle_masked_multi.c circle_sprite_masked.asm -o circle_masked_multi -create-app
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

typedef struct
{
  struct sp1_ss  *sprite;
  unsigned char   x_pos;
  unsigned char   y_pos;
} CIRCLE_SPRITE;

#define NUM_SPRITES  3

CIRCLE_SPRITE circle_sprites[NUM_SPRITES];

int main( void )
{
  unsigned char i;

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
 
  for( i=0; i<NUM_SPRITES; i++ )
  {
    circle_sprites[i].sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 2, (int)circle_masked, 0);
    sp1_AddColSpr(circle_sprites[i].sprite, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);

    circle_sprites[i].x_pos = i*10;
    circle_sprites[i].y_pos = i*10;
  }

  while(1)
  {
    for( i=0; i<NUM_SPRITES; i++ )
    {
      sp1_MoveSprPix(circle_sprites[i].sprite, &full_screen, 0, circle_sprites[i].x_pos++, circle_sprites[i].y_pos);
    }
    intrinsic_halt();
    sp1_UpdateNow();
  }
}
