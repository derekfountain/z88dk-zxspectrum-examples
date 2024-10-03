/* zcc +zx -vn -startup=0 -clib=sdcc_iy text_main.c text_data.c -o text -create-app */
/* zcc +zx -vn -startup=0 -clib=sdcc_iy @text.lst -o text -create-app  */
/* zcc +zx -vn -startup=0 -clib=sdcc_iy text_main.c text_data.asm -o text_asm -create-app */

#include <stdio.h>

extern unsigned char message[];

int main( void )
{
  printf("Message is: \"%s\"\n", message);

  return 0;
}
