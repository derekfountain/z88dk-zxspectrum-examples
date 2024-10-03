/*
 * zcc +zx -vn -startup=31 -clib=sdcc_iy runner.c runner_sprite.asm -o runner -create-app
 */
#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <z80.h>
#include <arch/zx/sp1.h>

extern unsigned char runner_f1[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

int main( void )
{
  struct sp1_ss  *runner_sprite;
  unsigned char   x;
  unsigned char   animation_offset;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  ' ' );
  sp1_Invalidate(&full_screen);
 
  runner_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int)runner_f1, 0);

  sp1_AddColSpr(runner_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);

  x=0;
  animation_offset=0;
  while(1) {
    runner_sprite->frame = (void*)animation_offset;
    sp1_MoveSprPix(runner_sprite, &full_screen, 0, x++, 80);

    animation_offset+=16;
    if( animation_offset == 128 )
    {
      animation_offset = 0;
    }

    z80_delay_ms(50);
    sp1_UpdateNow();
  }
}
