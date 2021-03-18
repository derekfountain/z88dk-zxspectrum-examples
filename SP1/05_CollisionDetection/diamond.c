/*
 * zcc +zx -vn -clib=sdcc_iy -startup=31 setup_sp1.c diamond.c diamond_sprites.asm -o diamond -create-app
 *
 * This animates a solid diamond which is controlled by QAOP.
 * There are 6 hollow diamonds to collect.
 * 
 * Bounding diamonds are used to check for collisions.
 */

#pragma output REGISTER_SP = 0xD000

#include <input.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>
#include <stdlib.h>

extern unsigned char diamond_solid_col1[];
extern unsigned char diamond_solid_col2[];
extern unsigned char diamond_hollow_col1[];
extern unsigned char diamond_hollow_col2[];

void setup_sp1(void);
extern struct sp1_Rect full_screen;

struct
{
  struct sp1_ss  *sprite;
  uint8_t         x;
  uint8_t         y;
} diamond_solid;

struct
{
  struct sp1_ss  *sprite;
  uint8_t         x;
  uint8_t         y;
  uint8_t         collected;
} diamond_hollow[6];

int main()
{
  uint8_t i;

  setup_sp1();

  /*
   * Create the solid diamond, starting at the top of the screen
   */
  diamond_solid.sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 0);
  sp1_AddColSpr(diamond_solid.sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, diamond_solid_col2-diamond_solid_col1, 0);
  sp1_AddColSpr(diamond_solid.sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);
  diamond_solid.x = 120;
  diamond_solid.y = 0;

  /*
   * Scatter the hollow diamonds about (non random).
   */
  for( i=0; i<6; i++ )
  {
    diamond_hollow[i].sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 10);
    sp1_AddColSpr(diamond_hollow[i].sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, diamond_hollow_col2-diamond_hollow_col1, 10);
    sp1_AddColSpr(diamond_hollow[i].sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 10);

    diamond_hollow[i].x = (uint8_t)i*175+25;
    diamond_hollow[i].y = (uint8_t)i*30+10;
    sp1_MoveSprPix(diamond_hollow[i].sprite, &full_screen, diamond_hollow_col1, diamond_hollow[i].x,
		                                                                diamond_hollow[i].y);
    diamond_hollow[i].collected = 0;
  }

  zx_border( INK_BLUE );

  while(1)
  {
    /*
     * Read the keyboard, move the diamond
     * Don't let it leave the screen area.
     */
    if( in_key_pressed( IN_KEY_SCANCODE_o ) )
    {
      if( diamond_solid.x )
	diamond_solid.x--;
    }
    else if( in_key_pressed( IN_KEY_SCANCODE_p ) )
    {
      if( diamond_solid.x < 240 )
	diamond_solid.x++;
    }
    if( in_key_pressed( IN_KEY_SCANCODE_q ) )
    {
      if( diamond_solid.y > 0 )
	diamond_solid.y--;
    }
    else if( in_key_pressed( IN_KEY_SCANCODE_a ) )
    {
      if( diamond_solid.y < 176 )
	diamond_solid.y++;
    }

    /*
     * Update the solid diamond's screen position.
     */
    sp1_MoveSprPix(diamond_solid.sprite, &full_screen, diamond_solid_col1, diamond_solid.x, diamond_solid.y);

    /*
     * Loop over the hollow diamond, check each one to see if the
     * solid diamond has collided with it. If so move it off 
     * screen to make it disappear
     */
    uint8_t diamond_solid_centre_x = diamond_solid.x+7;
    uint8_t diamond_solid_centre_y = diamond_solid.y+7;
    for( i=0; i<6; i++ )
    {
      if( diamond_hollow[i].collected )
	continue;

      uint8_t diamond_hollow_centre_x = diamond_hollow[i].x+7;
      uint8_t diamond_hollow_centre_y = diamond_hollow[i].y+7;

#define COLLISION_THRESHOLD 16
      if( ( abs(diamond_solid_centre_x - diamond_hollow_centre_x)
	    +
	    abs(diamond_solid_centre_y - diamond_hollow_centre_y) ) < COLLISION_THRESHOLD )
      {
	diamond_hollow[i].collected = 1;
	sp1_MoveSprPix(diamond_hollow[i].sprite, &full_screen, diamond_hollow_col1, 255, 255);
	break;
      }
    }

    intrinsic_halt();
    sp1_UpdateNow();
  }
}
