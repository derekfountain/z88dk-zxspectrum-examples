/*
 * zcc +zx -vn -clib=sdcc_iy -startup=4 signed.c -o signed -create-app -no-cleanup --c-code-in-asm
 */

#include <stdio.h>
#include <stdint.h>

uint8_t unsigned_addition(uint8_t input)
{
  uint8_t ten;
  uint8_t result;

  ten=10;
  result=ten+input;

  return result;
}

int8_t signed_addition(int8_t input)
{
  int8_t ten;
  int8_t result;

  ten=10;
  result=ten+input;

  return result;
}

int main( void )
{
  printf("unsigned_addition() = %u\n", unsigned_addition(10));
  printf("  signed_addition() = %d\n", signed_addition(-10));

  return 0;
}
