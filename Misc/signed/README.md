# signed

This one's about the handling of signed vs. unsigned numbers. I saw something
about SDCC producing worse code for signed numbers so I thought I'd look.
It's all about the compiled code, this one.

The two addition functions in the C code compile to this:

```
_unsigned_addition:
        push    ix
        ld      ix,0
        add     ix,sp
;signed.c:14: result=ten+input;
        ld      a,(ix+4)
        add     a,0x0a
;signed.c:16: return result;
        ld      l, a
        pop     ix
        ret

_signed_addition:
        push    ix
        ld      ix,0
        add     ix,sp
;signed.c:25: result=ten+input;
        ld      a,(ix+4)
        add     a,0x0a
;signed.c:27: return result;
        ld      l, a
        pop     ix
        ret

```

i.e. they're identical. So the Z80 doesn't care whether the C compiler
thinks a value is signed or unsigned. Addition is addition, regardless.

The *printf()* calls are more interesting:

```
;signed.c:32: printf("unsigned_addition() = %u\n", unsigned_addition(10));
        ld      a,0x0a
        push    af
        inc     sp
        call    _unsigned_addition
        inc     sp
        ld      c, l
        ld      b,0x00
        push    bc
        ld      hl,___str_0
        push    hl
        call    _printf
        ...
        ...
;signed.c:33: printf("  signed_addition() = %d\n", signed_addition(-10));
        ld      h,0xf6
        ex      (sp),hl
        inc     sp
        call    _signed_addition
        inc     sp
        ld      c, l
        ld      a, c
        rla
        sbc     a, a
        ld      b, a
        push    bc
        ld      hl,___str_1
        push    hl
        call    _printf
        ...
        ...
```

So that's where the difference is. The unsigned value handling is simple,
passed into printf as an 8 bit value in the C register. The signed
handling is more complex, rotating through carry then subtracting with
carry to load the top bits of B to get the two's complement.

So simple signed maths, where an 8 bit signed value is subtracted from
some other value, isn't likely to be slower. Calculating a screen position,
for example, shouldn't be a problem. The slow bit is where the value
needs to be in two's complement in order to be interpreted correctly.

I looked at the compiled code I'm using elsewhere for screen
positioning, which adds a signed (and potentially negative) offset to
an 8 bit value.  The compiled assembler doesn't do anything unusual,
no rotations or anything. So other than unintended side effects,
there's no apparent reason to avoid signed values in C.
