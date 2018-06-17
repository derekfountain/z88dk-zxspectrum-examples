# keypress

Question: is it possible for a player to tap a key on the Spectrum
keyboard and do it quickly enough that the keypress isn't registered?
At 50fps with a keyboard scan at the top of the loop they'd have to
press and release the key within 20ms. In a 25fps game they'd have
40ms.

Cue a quick program to test how many cycles a quick key tap actually
takes.