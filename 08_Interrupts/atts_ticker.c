/* zcc +zx -vn -clib=sdcc_iy -startup=31 atts_ticker.c -o atts_ticker -create-app */

/* Ensure IM2 is left at exit */
#pragma output CRT_INTERRUPT_MODE_EXIT = 2

#include <z80.h>
#include <string.h>
#include <im2.h>
#include <arch/zx.h>


static unsigned char  ticker_string[] = "Hello, world! ";
static unsigned char* current_char_ptr;

/* Address in ROM of the character being scrolled into view */
static unsigned char* rom_address;

/* Bit, left to right, of the character to scroll into view next. Goes 128, 64, 32...1 */
static unsigned char  bit;

/*
 * Off-screen buffer to put the display into. This is blitted into the screen, replacing
 * whatever the user's program happens to have put there. A "merge" would be friendlier. :)
 */
static unsigned char  off_screen_buffer[32*8];

IM2_DEFINE_ISR_WITH_BASIC(isr)
{
  unsigned char* buffer_address;
  unsigned char  i;

  /*
   * Scroll off-screen display buffer data leftwards one byte. This is just a memory move downwards by one. 
   */
  memcpy((unsigned char*)off_screen_buffer, (unsigned char*)off_screen_buffer+1, sizeof(off_screen_buffer)-1);

  /*
   * For each of the 8 lines (top to bottom) of the character we're displaying, pick out
   * the current bit (left to right). If it's a 1, set the rightmost attribute cell to
   * colour, otherwise set the attribute cell white. This is done in the off-screen buffer.
   */
  buffer_address = (unsigned char*)&off_screen_buffer+0x1f;
  for( i=0; i<8; i++ )
  {
    unsigned char attribute_value;

    attribute_value = ( *rom_address & bit ) ? PAPER_MAGENTA : PAPER_WHITE;

    *buffer_address = attribute_value;
    buffer_address += 0x20;

    rom_address++;
  }

  /*
   * If that was the rightmost bit of the character, that character's done with. Move to the
   * next character in the display string and start again at its left side (bit 128).
   * Otherwise keep with the same character and get ready for the next bit.
   */
  if( bit == 1 )
  {
    current_char_ptr++;
    if( *current_char_ptr == '\0' )
      current_char_ptr = ticker_string;

    rom_address = ((*current_char_ptr-0x20)*8)+(unsigned char*)0x3D00;

    bit = 128;
  }
  else
  {
    bit = bit/2;

    /* Still on the same character, so move back to the start of its data in ROM */
    rom_address -= 8;
  }

  /* Copy the off-screen buffer into the display */
  memcpy( (unsigned char*)0x5800, off_screen_buffer, sizeof(off_screen_buffer) );
}


#define TABLE_HIGH_BYTE        ((unsigned int)0xfc)
#define JUMP_POINT_HIGH_BYTE   ((unsigned int)0xfb)

#define UI_256                 ((unsigned int)256)

#define TABLE_ADDR             ((void*)(TABLE_HIGH_BYTE*UI_256))
#define JUMP_POINT             ((unsigned char*)( (unsigned int)(JUMP_POINT_HIGH_BYTE*UI_256) + JUMP_POINT_HIGH_BYTE ))

int main( void )
{
  /*
   * Initialise the ticker and its buffer
   */
  memset( off_screen_buffer, PAPER_WHITE+INK_WHITE, sizeof(off_screen_buffer) );

  current_char_ptr = ticker_string;
  rom_address      = ((*current_char_ptr-0x20)*8)+(unsigned char*)0x3D00;
  bit              = 128;

  /* Set up the interrupt vector table */
  im2_init( TABLE_ADDR );

  memset( TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257 );

  z80_bpoke( JUMP_POINT,   195 );
  z80_wpoke( JUMP_POINT+1, (unsigned int)isr );

  return 0;
}
