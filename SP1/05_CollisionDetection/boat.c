#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>

extern unsigned char boat_col1[];
extern unsigned char boat_col2[];

void setup_sp1(void);
extern struct sp1_Rect full_screen;

int main()
{
  setup_sp1();

  struct sp1_ss  *boat_sprite;

  boat_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, 0, 0);
  sp1_AddColSpr(boat_sprite, SP1_DRAW_LOAD1, SP1_TYPE_1BYTE, boat_col2-boat_col1, 0);
  sp1_AddColSpr(boat_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);
    sp1_UpdateNow();

  zx_border(INK_BLUE);
  
  /* Green for land */
  unsigned char *att_addr = zx_cxy2aaddr(24, 23);
  for( unsigned char i=24; i<32; i++ ) {
    *att_addr = PAPER_GREEN;
    att_addr = zx_aaddrcright(att_addr);
  }


  const unsigned char HOTSPOT_X = 15;
  const unsigned char HOTSPOT_Y = 10;

  unsigned char boat_x_pos = 0;
  unsigned char boat_y_pos = 176;

  unsigned char hotspot_attribute;
  do {
    sp1_MoveSprPix(boat_sprite, &full_screen, boat_col1, boat_x_pos, boat_y_pos);
    sp1_UpdateNow();
    intrinsic_halt();

    boat_x_pos++;
    hotspot_attribute = *(zx_pxy2aaddr(boat_x_pos+HOTSPOT_X, boat_y_pos+HOTSPOT_Y));
  }
  while( hotspot_attribute != PAPER_GREEN );
  boat_x_pos--;
  
  while(1);
}
