/* zcc +zx -vn -clib=sdcc_iy -startup=31 setup_sp1.c chain.c gem_sprite.asm -o chain -create-app */

#pragma output REGISTER_SP = 0xD000

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>
#include <stdlib.h>

extern unsigned char gem_col1[];
extern unsigned char gem_col2[];

void setup_sp1(void);
extern struct sp1_Rect full_screen;

#define GEM_HEIGHT   ((uint8_t)14)
#define GEM_WIDTH    ((uint8_t)12)
#define CHAIN_HEIGHT ((uint8_t)9)

/*
 * A 16x9 chain graphic. SP1 tiles, should be displayed as:
 *
 *  121212...
 *  343434...
 *
 * Tiles are 8x8, so this is 4 tiles in 16x16 formation, with
 * the bottom 7 rows (of tiles 3 and 4) empty.
 */
const uint8_t tile1[] = {0xf0, 0x0f, 0x10, 0xe0, 0x27, 0xfa, 0x02, 0x07};
const uint8_t tile2[] = {0x0f, 0xf0, 0x04, 0x03, 0xf2, 0x2c, 0x20, 0xf0};
const uint8_t tile3[] = {0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t tile4[] = {0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*
 * In this example the gem comes up from underneath the chain, so the
 * hotspot is on the leading tip.
 */
#define GEM_HOTSPOT_X ((uint8_t)7)
#define GEM_HOTSPOT_Y ((uint8_t)1)

/* This is used to define the bounding box of the length of chain graphic */
struct rect {
   uint8_t x1;
   uint8_t y1;
   uint8_t x2;
   uint8_t y2;
};

int main()
{
  setup_sp1();

  struct sp1_ss  *gem_sprite;

  gem_sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, 0, 0);
  sp1_AddColSpr(gem_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, gem_col2-gem_col1, 0);
  sp1_AddColSpr(gem_sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);
  sp1_UpdateNow();

  zx_border(INK_BLUE);

  /* Create the tiles in SP1 */
  sp1_TileEntry('a', tile1); sp1_TileEntry('b', tile2);
  sp1_TileEntry('c', tile3); sp1_TileEntry('d', tile4);

  while(1)
  {
    /* Place the chain at a random height from the top two 3rds of the screen */
    uint8_t chain_top_y_pos = (uint8_t)(((long)(16) * rand()) / (long)(RAND_MAX));

    uint8_t i;
    for(i=0; i<32; i+=2)
    {
      sp1_PrintAt(chain_top_y_pos,   i,   INK_BLACK | PAPER_WHITE, 'a');
      sp1_PrintAt(chain_top_y_pos,   i+1, INK_BLACK | PAPER_WHITE, 'b');
      sp1_PrintAt(chain_top_y_pos+1, i,   INK_BLACK | PAPER_WHITE, 'c');
      sp1_PrintAt(chain_top_y_pos+1, i+1, INK_BLACK | PAPER_WHITE, 'd');
    }
    sp1_Invalidate( &full_screen );
    sp1_UpdateNow();

    /*
     * This is the box, in pixels, which defines the chain's boundary.
     * If the hotspot of the sprite enters this box it's a collision
     */
    struct rect chain_box = { 0,   chain_top_y_pos*8,
                              255, chain_top_y_pos*8+CHAIN_HEIGHT };

    /* Pick a start point along the bottom */
    uint8_t xpos = (uint8_t)(((long)(255-GEM_WIDTH+2) * rand()) / (long)(RAND_MAX));
    uint8_t ypos = (uint8_t)((191-GEM_HEIGHT)+1);

    while(ypos)
    {
      sp1_MoveSprPix(gem_sprite, &full_screen, gem_col1, xpos, ypos);
      intrinsic_halt();
      sp1_UpdateNow();
      
      /*
       * Because the gem fires up from the bottom, the hotspot can
       * only enter the chain's bounding box from the underside.  And
       * because the chain stretches across the screen the collision
       * detection for this scenario can be optimised to a single
       * check.
       *
       * If the scenario calls for checking that the hotspot is inside
       * any edge of the bounding box the second piece of code is
       * required, but it's slower.
       */
#define OPTIMAL 1
#if OPTIMAL
      if( ypos+GEM_HOTSPOT_Y <= chain_box.y2 )
	break;
#else
      uint8_t hotspot_xpos = xpos+GEM_HOTSPOT_X;
      uint8_t hotspot_ypos = ypos+GEM_HOTSPOT_Y;
      if( hotspot_xpos >= chain_box.x1 && hotspot_xpos <= chain_box.x2
	  &&
          hotspot_ypos >= chain_box.y1 && hotspot_ypos <= chain_box.y2 )
	break;
#endif

      ypos--;
    }

    /* Remove the chain and sprite and just keep running the example */
    sp1_MoveSprPix(gem_sprite, &full_screen, (void*)0, 255, 255);
    sp1_ClearRect(&full_screen, INK_BLACK | PAPER_WHITE, ' ', SP1_RFLAG_TILE);
  }

}
