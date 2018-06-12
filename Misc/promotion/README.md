# promotion

Test to show how the SDCC compiler handles implicit variable promotion.

The question is, if I have a unsigned char value, and I set it to 255,
then add 1, does it compare to zero? The answer is, yes, but only
if you're careful.