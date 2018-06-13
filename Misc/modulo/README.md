# modulo

The objective here was to find a way to count the number of iterations of a game
loop which take longer than 20ms to complete. A game will run at 50fps as long as
the main processing in the loop completes between consequtive interrupts. i.e.
in less than 1/50th second. The idea was to update a ticker in the ISR, then take
a note of that ticker at the start of the loop, then again after a HALT. The ticker
would be expected to increment immediately after the HALT, so it should end up one
greater than the value seen at the start of the loop. However, if the work in the
loop takes more than 20ms the ticker will be incremented during it, then incremented
again once the HALT completes, hence it ends up 2 greater, which means a dropped
frame.

Simple, but I couldn't get the thing to work properly. All sorts of weird things
happened when I starting introducing a workload in the loop which tied up the CPU
for a random period. I eventually worked out that restricting the workload using

```
rand() % 2048
```

was expensive in an inconsistent way. It's not the rand(), it's the modulo operator.
For some values, that produces quite a significant chunk of work which causes a
dropped frame. For other values it's very fast.

Lesson: don't use modulo inside a game loop!