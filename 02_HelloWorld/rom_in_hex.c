/*
 * zcc +zx -vn -startup=4 -clib=sdcc_iy rom_in_hex.c -o rom_in_hex -create-app
 */

#include <arch/zx.h>
#include <stdio.h>

int main()
{
  unsigned char * rom_addr = 0;
  int i, j;

  zx_cls(PAPER_WHITE);

  for( i=0; i<50; i++ )
  {
    printf("0x%04X: ", rom_addr);

    for( j=0; j<8; j++ )
    {
      printf("%02X ", *(rom_addr+j));
    }
    printf("   ");
    for( j=0; j<8; j++ )
    {
      printf("%c ",
             (*rom_addr>=32 && *rom_addr<=127) ? *rom_addr : '.');
      rom_addr++;
    }

    printf("\n");
  }
  
  return 0;
}
