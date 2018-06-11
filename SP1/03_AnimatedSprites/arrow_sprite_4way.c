/*
 * zcc +zx -vn -m -startup=1 -clib=sdcc_iy arrow_sprite_4way.c arrow_sprite_4way.asm -o arrow_sprite_4way -create-app
 */

#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <z80.h>
#include <arch/zx/sp1.h>
#include <input.h>

extern unsigned char arrow_up[];
extern unsigned char arrow_down[];
extern unsigned char arrow_left[];
extern unsigned char arrow_right[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

struct
{
  unsigned int   scan_code;
  unsigned char *graphic;
  signed   char  x_delta;
  signed   char  y_delta;
}
arrow_state[4] = { {IN_KEY_SCANCODE_q, arrow_up,     0, -1},
                   {IN_KEY_SCANCODE_a, arrow_down,   0, +1},
                   {IN_KEY_SCANCODE_o, arrow_left,  -1,  0},
                   {IN_KEY_SCANCODE_p, arrow_right, +1,  0} };

int main()
{
  struct sp1_ss  *arrow_sprite;

  unsigned char   x     = 128;
  unsigned char   y     = 87;
  unsigned char   state = 0;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  ' ' );
 
  arrow_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, 0, 0);
  sp1_AddColSpr(arrow_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);
  sp1_MoveSprPix(arrow_sprite, &full_screen, arrow_state[state].graphic, x, y);
  sp1_Invalidate(&full_screen);

  while( 1 ) {
    unsigned char i;

    for( i=0; i<4; i++ )
    {
      if( in_key_pressed( arrow_state[i].scan_code ) == 0xFFFF ) {
        state = i;
        break;
      }
    }

    x += arrow_state[state].x_delta;
    y += arrow_state[state].y_delta;
    if( y == 175+8 )
      y = 0;
    else if( y == 0 )
      y = 175+8;

    sp1_MoveSprPix(arrow_sprite, &full_screen, arrow_state[state].graphic, x, y);
    z80_delay_ms(20);

    sp1_UpdateNow();    
  }
}
