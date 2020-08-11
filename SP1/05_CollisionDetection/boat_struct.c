/* zcc +zx -vn -clib=sdcc_iy -startup=31 setup_sp1.c boat_struct.c boat_sprite.asm -o boat -create-app */

#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>

extern unsigned char boat_col1[];
extern unsigned char boat_col2[];
struct
{
  unsigned char *col1;
  unsigned char *col2;

  unsigned char hotspot_x;
  unsigned char hotspot_y;
}
boat_gfx_data = { boat_col1, boat_col2, 15, 10 };

void setup_sp1(void);
extern struct sp1_Rect full_screen;

int main()
{
  setup_sp1();

  struct sp1_ss  *boat_sprite;

  boat_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, 0, 0);
  sp1_AddColSpr(boat_sprite, SP1_DRAW_LOAD1, SP1_TYPE_1BYTE, boat_gfx_data.col2-boat_gfx_data.col1, 0);
  sp1_AddColSpr(boat_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);
    sp1_UpdateNow();

  zx_border(INK_BLUE);
  
  /* Green for land */
  unsigned char *att_addr = zx_cxy2aaddr(24, 23);
  for( unsigned char i=24; i<32; i++ ) {
    *att_addr = PAPER_GREEN;
    att_addr = zx_aaddrcright(att_addr);
  }

  unsigned char boat_x_pos = 0;
  unsigned char boat_y_pos = 176;

  unsigned char hotspot_attribute;
  do {
    sp1_MoveSprPix(boat_sprite, &full_screen, boat_col1, boat_x_pos, boat_y_pos);
    sp1_UpdateNow();
    intrinsic_halt();

    boat_x_pos++;
    hotspot_attribute = *(zx_pxy2aaddr(boat_x_pos+boat_gfx_data.hotspot_x, boat_y_pos+boat_gfx_data.hotspot_y));
  }
  while( hotspot_attribute != PAPER_GREEN );
  boat_x_pos--;
  
  while(1);
}
