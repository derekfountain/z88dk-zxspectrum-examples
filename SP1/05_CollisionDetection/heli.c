/*
 * zcc +zx -vn -clib=sdcc_iy -startup=31 setup_sp1.c heli.c heli_sprites.asm -o heli -create-app -SO3 --max-allocs-per-node200000
 *
 * This animates a helicopter which is controlled by QAOP, plus 2
 * deadly bombs which move up and down. There are 6 moneybags
 * the helicopter can collect.
 * 
 * Bounding boxes are used to check for collisions. The moneybags
 * don't move, so their bounding boxes are calculated when they're
 * placed on screen. The bombs and helicopter have their bounding
 * boxes recalculated every loop.
 */

#pragma output REGISTER_SP = 0xD000

#include <input.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>

extern unsigned char heli_left_col1[];
extern unsigned char heli_left_col2[];
extern unsigned char heli_right_col1[];
extern unsigned char heli_right_col2[];
extern unsigned char moneybag_col1[];
extern unsigned char moneybag_col2[];
extern unsigned char bomb_col1[];
extern unsigned char bomb_col2[];

void setup_sp1(void);
extern struct sp1_Rect full_screen;

enum {
  FACING_LEFT,
  FACING_RIGHT
};

enum {
  MOVING_UP,
  MOVING_DOWN
};

typedef struct
{
  uint8_t         x1;
  uint8_t         y1;
  uint8_t         x2;
  uint8_t         y2;
} Rect;

/*
 * Characters, one helicopter, 6 moneybags and 2 bombs.
 */
struct
{
  struct sp1_ss  *sprite;
  Rect            bounding_box;
  uint8_t         facing;
} heli;

struct
{
  struct sp1_ss  *sprite;
  Rect            bounding_box;
  uint8_t         collected;
} moneybag[6];

struct
{
  struct sp1_ss  *sprite;
  Rect            bounding_box;
  uint8_t         moving;
} bomb[2];

int main()
{
  uint8_t i;

  setup_sp1();

  /*
   * Create the helicopter, starting at the top of the screen
   */
  heli.sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 0);
  sp1_AddColSpr(heli.sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, heli_left_col2-heli_left_col1, 0);
  sp1_AddColSpr(heli.sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);
  heli.bounding_box.x1 = 120;
  heli.bounding_box.y1 = 0;
  heli.facing = FACING_LEFT;

  /*
   * Scatter the moneybags about (non random). They don't move so these
   * could be SP1 tiles. The bounding box for each 14x16 sprite is
   * calculated and stored.
   */
  for( i=0; i<6; i++ )
  {
    moneybag[i].sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 10);
    sp1_AddColSpr(moneybag[i].sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, moneybag_col2-moneybag_col1, 10);
    sp1_AddColSpr(moneybag[i].sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 10);

    moneybag[i].bounding_box.x1 = (uint8_t)i*175+25; moneybag[i].bounding_box.x2=moneybag[i].bounding_box.x1+14;
    moneybag[i].bounding_box.y1 = (uint8_t)i*30+10;  moneybag[i].bounding_box.y2=moneybag[i].bounding_box.y1+16;
    sp1_MoveSprPix(moneybag[i].sprite, &full_screen, moneybag_col1, moneybag[i].bounding_box.x1,
		                                                    moneybag[i].bounding_box.y1);
    moneybag[i].collected = 0;
  }

  /*
   * The two bombs, one going one way, one going the other
   */
  bomb[0].sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 10);
  sp1_AddColSpr(bomb[0].sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, bomb_col2-bomb_col1, 0);
  sp1_AddColSpr(bomb[0].sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  bomb[0].bounding_box.x1 = (uint8_t)65; bomb[0].bounding_box.x2=bomb[0].bounding_box.x1+16;
  bomb[0].bounding_box.y1 = (uint8_t)0;  bomb[0].bounding_box.y2=bomb[0].bounding_box.y1+16;
  sp1_MoveSprPix(bomb[0].sprite, &full_screen, bomb_col1, bomb[0].bounding_box.x1,
		                                          bomb[0].bounding_box.y1);
  bomb[0].moving = MOVING_DOWN;

  bomb[1].sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 10);
  sp1_AddColSpr(bomb[1].sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, bomb_col2-bomb_col1, 0);
  sp1_AddColSpr(bomb[1].sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  bomb[1].bounding_box.x1 = (uint8_t)175; bomb[1].bounding_box.x2=bomb[1].bounding_box.x1+16;
  bomb[1].bounding_box.y1 = (uint8_t)175; bomb[1].bounding_box.y2=bomb[1].bounding_box.y1+16;
  sp1_MoveSprPix(bomb[1].sprite, &full_screen, bomb_col1, bomb[1].bounding_box.x1,
		                                          bomb[1].bounding_box.y1);
  bomb[1].moving = MOVING_UP;

  zx_border( INK_BLUE );

  while(1)
  {
    /*
     * Read the keyboard, turn or move the helicopter.
     * Don't let it leave the screen area.
     */
    if( in_key_pressed( IN_KEY_SCANCODE_o ) )
    {
      if( heli.facing == FACING_LEFT )
      {	
	if( heli.bounding_box.x1 )
	  heli.bounding_box.x1--;
      }
      else
      {
	heli.facing = FACING_LEFT;
      }
    }
    else if( in_key_pressed( IN_KEY_SCANCODE_p ) )
    {
      if( heli.facing == FACING_RIGHT )	
      {
	if( heli.bounding_box.x1 < 240 )
	  heli.bounding_box.x1++;
      }
      else
      {
	heli.facing = FACING_RIGHT;
      }
    }
    if( in_key_pressed( IN_KEY_SCANCODE_q ) )
    {
      if( heli.bounding_box.y1 > 0 )
	heli.bounding_box.y1--;
    }
    else if( in_key_pressed( IN_KEY_SCANCODE_a ) )
    {
      if( heli.bounding_box.y1 < 176 )
	heli.bounding_box.y1++;
    }

    /*
     * Update the helicopter's screen position, recalculate
     * it's bounding box. The sprite is 16x16.
     */
    sp1_MoveSprPix(heli.sprite, &full_screen, (heli.facing==FACING_LEFT) ? heli_left_col1 : heli_right_col1,
		   heli.bounding_box.x1, heli.bounding_box.y1);
    heli.bounding_box.x2 = heli.bounding_box.x1+16;
    heli.bounding_box.y2 = heli.bounding_box.y1+16;

    /*
     * Move the bombs up or down, recalculate each one's
     * bounding box
     */
    for( i=0; i<2; i++)
    {
      if( bomb[i].moving == MOVING_UP )
      {
	if( bomb[i].bounding_box.y1 == 0 )
	  bomb[i].moving=MOVING_DOWN;
	else
	  bomb[i].bounding_box.y1--;
      }
      else
      {
	if( bomb[i].bounding_box.y1 == 176 )
	  bomb[i].moving=MOVING_UP;
	else
	  bomb[i].bounding_box.y1++;
      }
      sp1_MoveSprPix(bomb[i].sprite, &full_screen, bomb_col1, bomb[i].bounding_box.x1, bomb[i].bounding_box.y1);
      bomb[i].bounding_box.x2 = bomb[i].bounding_box.x1+16;
      bomb[i].bounding_box.y2 = bomb[i].bounding_box.y1+16;
    }

    /*
     * Loop over the moneybags, check each one to see if the
     * helicopter has collided with it. If so move it off 
     * screen to make it disappear
     */
    for( i=0; i<6; i++ )
    {
      if( moneybag[i].collected )
	continue;

      if( ! (moneybag[i].bounding_box.x1 >= heli.bounding_box.x2 || moneybag[i].bounding_box.x2 <= heli.bounding_box.x1
	     ||
	     moneybag[i].bounding_box.y1 >= heli.bounding_box.y2 || moneybag[i].bounding_box.y2 <= heli.bounding_box.y1) )
      {
	moneybag[i].collected = 1;
	sp1_MoveSprPix(moneybag[i].sprite, &full_screen, moneybag_col1, 255, 255);
	break;
      }
    }

    /*
     * Loop over the bombs. A collision with the helicopter ends
     * the example
     */
    for( i=0; i<2; i++ )
    {
      if( ! (bomb[i].bounding_box.x1 >= heli.bounding_box.x2 || bomb[i].bounding_box.x2 <= heli.bounding_box.x1
	     ||
	     bomb[i].bounding_box.y1 >= heli.bounding_box.y2 || bomb[i].bounding_box.y2 <= heli.bounding_box.y1) )
      {
	sp1_UpdateNow();
	while(1) for(i=0;i<8;i++) zx_border(i);
      }
    }

    sp1_UpdateNow();
  }
}
