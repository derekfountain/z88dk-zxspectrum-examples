/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy arrow_sprite.c arrow_sprite.asm -o arrow_sprite -create-app
 */
#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <z80.h>
#include <arch/zx/sp1.h>
#include <input.h>

extern unsigned char arrow_left[];
extern unsigned char arrow_right[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

struct
{
  unsigned int   scan_code;
  unsigned char *graphic;
  signed   char  x_delta;
}
arrow_state[] = { {IN_KEY_SCANCODE_o, arrow_left,  -1},
                  {IN_KEY_SCANCODE_p, arrow_right, +1} };

int main()
{
  struct sp1_ss  *arrow_sprite;

  unsigned char   x     = 128;
  unsigned char   state = 0;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  ' ' );
  sp1_Invalidate(&full_screen);
 
  arrow_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, 0, 0);
  sp1_AddColSpr(arrow_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);

  while( 1 ) {
    unsigned char i;

    for( i=0; i<2; i++ )
    {
      if( in_key_pressed( arrow_state[i].scan_code ) == 0xFFFF ) {
        state = i;
        break;
      }
    }

    x += arrow_state[state].x_delta;

    sp1_MoveSprPix(arrow_sprite, &full_screen, arrow_state[state].graphic, x, 80);
    z80_delay_ms(10);

    sp1_UpdateNow();    
  }
}
 
