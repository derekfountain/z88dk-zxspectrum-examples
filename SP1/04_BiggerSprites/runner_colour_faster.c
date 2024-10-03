/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy runner_colour_faster.c ../03_AnimatedSprites/runner_sprite.asm -o runner_colour_faster -create-app
 *
 * This is an approach I found detailed here:
 *
 * https://www.z88dk.org/forum/viewtopic.php?f=2&t=3209
 *
 * sp1_IterateSprChar() can be used to write colour (attribute) information into the
 * sprite structure, but it's a slow thing to do in a game loop. This approach uses
 * that function to set the intial colours, but then uses the "get sprite colour"
 * and "put sprite colour" routines to update the colours in the loop. I haven't
 * timed it, but looking at the SP1 sources shows this has got to be a lot faster
 * (which is why the library code was written).
 */

#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <z80.h>
#include <arch/zx/sp1.h>

extern unsigned char runner_f1[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

/* Sprite is 8x8 pixels, which is 2x2 char tiles, which is 4 cells as far as SP1 is concerned */
struct sp1_ap *colour_addresses[4];

/* Colour information, initialised here, then updated in the main loop */
struct sp1_ap runner_colour[4] = { {SP1_AMASK_INK, INK_BLUE},
				   {SP1_AMASK_INK, INK_BLUE},
				   {SP1_AMASK_INK, INK_BLUE},
				   {SP1_AMASK_INK, INK_BLUE},
                                 };

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
  sp1_UpdateNow();
 
  runner_sprite = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int)runner_f1, 0);
  sp1_AddColSpr(runner_sprite, SP1_DRAW_LOAD1RB, SP1_TYPE_1BYTE, 0, 0);

  /*
   * Fetch the addresses of the sprite's tile's attribute descriptors.
   * The fact this takes a unit8_t* instead of a sp1_ap* is surely a bug
   * in the SP1 API? Nevermind, just cast it.
   */
  sp1_GetSprClrAddr(runner_sprite, (uint8_t*)&colour_addresses[0]);

  /* Copy the initial sprite colours (statically defined, above) into the sprite */
  sp1_PutSprClr((uint8_t*)&colour_addresses[0], &runner_colour[0], 4);

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

    /*
     * This reads the attribute information of each of the sprite's tiles into the
     * runner_colour array. For this example this is actually necessary as the
     * sprite is in sync with this array.
     */
    sp1_GetSprClr((uint8_t*)&colour_addresses[0], &runner_colour[0], 4);
    
    /* Update the colour information in each tile. Cycle the ink colour. */
    uint8_t i;
    for(i=0;i<4;i++)
    {
      if( ++runner_colour[i].attr == INK_WHITE )
	runner_colour[i].attr = INK_BLUE;
    }

    /* Now write the updated colour information back into the sprite */
    sp1_PutSprClr((uint8_t*)&colour_addresses[0], &runner_colour[0], 4);

    z80_delay_ms(50);
    sp1_UpdateNow();
  }
}
