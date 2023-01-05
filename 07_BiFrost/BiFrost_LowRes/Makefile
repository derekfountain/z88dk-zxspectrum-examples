all: bifrost_01.tap

bifrost_01_CODE.bin: bifrost_01.c ctile.asm coloured_ball.ctile
	zcc +zx -vn -startup=31 -clib=sdcc_iy bifrost_01.c ctile.asm -o bifrost_01

bifrost_01_code.tap: bifrost_01_CODE.bin
	z88dk-appmake +zx -b bifrost_01_CODE.bin -o bifrost_01_code.tap --noloader --org 32768 --blockname bifrost_01_code
	z88dk-appmake +zx -b bifrost_01_BIFROSTL.bin -o bifrostl.tap --noloader --org 58625 --blockname bifrostl

bifrost_01.tap: bifrost_01_code.tap
	cat bifrost_loader.tap bifrost_01_code.tap bifrostl.tap > bifrost_01.tap