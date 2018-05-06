all: bifrost_02.tap

bifrost_02_CODE.bin: bifrost_02.c ctile.asm coloured_ball.ctile
	zcc +zx -vn -startup=31 -clib=sdcc_iy bifrost_02.c ctile.asm -o bifrost_02

bifrost_02_code.tap: bifrost_02_CODE.bin
	appmake +zx -b bifrost_02_CODE.bin -o bifrost_02_code.tap --noloader --org 32768 --blockname bifrost_02_code
	appmake +zx -b bifrost_02_BIFROSTH.bin -o bifrosth.tap --noloader --org 57047 --blockname bifrosth

bifrost_02.tap: bifrost_02_code.tap
	cat bifrost_loader.tap bifrost_02_code.tap bifrosth.tap > bifrost_02.tap