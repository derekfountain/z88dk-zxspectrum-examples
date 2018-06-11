SECTION rodata_user

PUBLIC _arrow_up
PUBLIC _arrow_down
PUBLIC _arrow_left
PUBLIC _arrow_right

	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000

._arrow_up
	defb @00011000
	defb @00100100
	defb @01000010
	defb @10100101
	defb @00100100
	defb @00100100
	defb @00100100
	defb @00111100

	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	
._arrow_down
	defb @00111100
	defb @00100100
	defb @00100100
	defb @00100100
	defb @10100101
	defb @01000010
	defb @00100100
	defb @00011000

	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	
._arrow_left
	defb @00010000
	defb @00100000
	defb @01011111
	defb @10000001
	defb @10000001
	defb @01011111
	defb @00100000
	defb @00010000

	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	
._arrow_right
	defb @00001000
	defb @00000100
	defb @11111010
	defb @10000001
	defb @10000001
	defb @11111010
	defb @00000100
	defb @00001000

	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000
	defb @00000000	

	
	