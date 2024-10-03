/*
 * zcc +zx -vn -m -startup=4 -clib=sdcc_iy modulo.c -o modulo -create-app
 */

#include <arch/zx.h>
#include <string.h>
#include <z80.h>
#include <intrinsic.h>
#include <im2.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ITERATIONS 50

unsigned char tick = 0;
unsigned int  dropped_frames = 0;
unsigned int  drop_values[MAX_ITERATIONS][3];

IM2_DEFINE_ISR_8080(isr)
{
  ++tick;
}

void setup_int(void)
{
   im2_init((void *)0xd000);
   memset((void *)0xd000, 0xd1, 257);
   
   z80_bpoke(0xd1d1, 0xc3);
   z80_wpoke(0xd1d2, (unsigned int)isr);
}

int main( void )
{
  unsigned int iterations;
  unsigned int i;

  unsigned char post_halt_tick;

  setup_int();
  intrinsic_ei();

  for(iterations=0; iterations<MAX_ITERATIONS; iterations++)
  {
    unsigned char toptick = tick;
    unsigned int counter1 = 0;
    
    /*
     * Setting the counter to a hard coded value around 1475
     * produces a delay long enough to miss the next interrupt.
     *
     * More interestingly, introducing the modulo operator
     * causes a delay of a not-immediately-obvious period.
     * I never really thought about it, but modulo, without
     * hardware support, can be very quick (modulo 1 is just
     * a check of bit 0) or very slow.
     */
    unsigned int rand_delay = rand() % 2048;
    for(counter1=0; counter1<rand_delay; counter1++);

    intrinsic_halt();
    post_halt_tick = tick;
    if( post_halt_tick != (unsigned char)(toptick+1) )
    {
      drop_values[dropped_frames][0] = rand_delay;
      drop_values[dropped_frames][1] = post_halt_tick;
      drop_values[dropped_frames++][2] = (unsigned char)(toptick+1);
    }
  }

  printf("Dropped frames: %u\n", dropped_frames);
  for( i=0; i<dropped_frames; i++ )
  {
    printf("Dropped with delay: %u tick of %u != toptick+1 of %u\n",
           drop_values[i][0],
           drop_values[i][1],
           drop_values[i][2]);
  }

  return 0;
}
