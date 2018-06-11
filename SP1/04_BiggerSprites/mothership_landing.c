/*
 * zcc +zx -vn -m -startup=31 -clib=sdcc_iy mothership_landing.c mothership_sprite.asm -o mothership_landing -create-app
 */

#pragma output REGISTER_SP = 0xD000

#include <z80.h>
#include <intrinsic.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

extern unsigned char mothership_col1[];
extern unsigned char mothership_col2[];
extern unsigned char mothership_col3[];
extern unsigned char mothership_col4[];
extern unsigned char mothership_col5[];
extern unsigned char mothership_col6[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

void initialiseColour(unsigned int count, struct sp1_cs *c)
{

  if( count == 2 || count == 3 || count == 4 )
  {
    c->attr_mask = SP1_AMASK_INK;
    c->attr      = INK_BLUE;
  }
}

void changeAntennacolour(unsigned int count, struct sp1_cs *c)
{

  if( count == 2 || count == 3 || count == 4 )
  {
    unsigned char *p;

    if( ++c->attr == INK_WHITE )
      c->attr = INK_BLUE;

    if (*(p = (unsigned char *)c->update))
    {
      sp1_InvUpdateStruct((void*)intrinsic_swap_endian_16((unsigned int)p));
    }
  }
}

void invalidateAntenna(unsigned int count, struct sp1_update *u)
{
  if( count == 2 || count == 3 || count == 4 )
  {
    sp1_InvUpdateStruct(u);
  }
}

struct sp1_ss  *mothership_sprite;

int main()
{
  unsigned char y;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  ' ' );
 
  mothership_sprite = sp1_CreateSpr(SP1_DRAW_LOAD2LB, SP1_TYPE_2BYTE, 4, 0, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_LOAD2,    SP1_TYPE_2BYTE, (int)mothership_col2-mothership_col1, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_LOAD2,    SP1_TYPE_2BYTE, (int)mothership_col3-mothership_col1, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_LOAD2,    SP1_TYPE_2BYTE, (int)mothership_col4-mothership_col1, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_LOAD2,    SP1_TYPE_2BYTE, (int)mothership_col5-mothership_col1, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_LOAD2,    SP1_TYPE_2BYTE, (int)mothership_col6-mothership_col1, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_LOAD2RB,  SP1_TYPE_2BYTE, 0, 0);

  sp1_IterateSprChar(mothership_sprite, initialiseColour);
  sp1_Invalidate(&full_screen);

  y=0;
  while(1)
  {
    sp1_IterateSprChar(mothership_sprite, changeAntennacolour);

    if( y <= 192-24+2 )
    {
      sp1_MoveSprPix(mothership_sprite, &full_screen, mothership_col1, 104, y++);
    }
    else
    {
//      sp1_IterateUpdateSpr(mothership_sprite,invalidateAntenna);
    }

    z80_delay_ms(20);
    sp1_UpdateNow();    
  }
}
