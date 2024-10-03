/*
 * zcc +zx -vn -clib=sdcc_iy -startup=1 keypress.c -o keypress -SO3 --max-allocs-per-node200000 -create-app
 */

#pragma output REGISTER_SP = 0xFF58
#pragma output CLIB_MALLOC_HEAP_SIZE = -0xFBFA

#include <inttypes.h>
#include <z80.h>
#include <string.h>
#include <intrinsic.h>
#include <im2.h>
#include <arch/zx.h>
#include <input.h>
#include <stdio.h>

IM2_DEFINE_ISR(isr)
{
}

#define TABLE_HIGH_BYTE        ((unsigned int)0xfc)
#define JUMP_POINT_HIGH_BYTE   ((unsigned int)0xfb)

#define UI_256                 ((unsigned int)256)

#define TABLE_ADDR             ((void*)(TABLE_HIGH_BYTE*UI_256))
#define JUMP_POINT             ((unsigned char*)( (unsigned int)(JUMP_POINT_HIGH_BYTE*UI_256) + JUMP_POINT_HIGH_BYTE ))

int main( void )
{
  uint16_t key_held_cycles = 0;
  uint8_t  key_was_pressed = 0;

  memset( TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257 );

  z80_bpoke( JUMP_POINT,   195 );
  z80_wpoke( JUMP_POINT+1, (unsigned int)isr );

  im2_init( TABLE_ADDR );

  intrinsic_ei();

  zx_cls( INK_BLACK | PAPER_WHITE );

  key_held_cycles = 0;
  key_was_pressed = 0;
  while(1)
  {
    uint8_t key_is_pressed = (in_key_pressed( IN_KEY_SCANCODE_SPACE ) != 0);

    if( key_was_pressed && !key_is_pressed ) {

      /* Key has been released */
      printf("Key was held for %u cycles\n", key_held_cycles);
      key_held_cycles = 0;
    }

    intrinsic_halt();

    /* If it's still down it's been held a complete cycle */
    if( key_is_pressed && (in_key_pressed( IN_KEY_SCANCODE_SPACE ) != 0) )
       key_held_cycles++;

    key_was_pressed = key_is_pressed;
  }
}
