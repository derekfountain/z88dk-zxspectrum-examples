_main::
        push    ix
        ld      ix,#0
        add     ix,sp
        dec     sp
        ld      -1 (ix), #0xff
        ld      bc,#0x00ff
        inc     bc
        ld      a, b
        or      a,c
        jr      NZ,l_main_00102$
        ld      hl, #___str_0
        push    hl
        call    _printf
        pop     af
l_main_00102$:
        ld      -1 (ix), #0xff
        ld      a, #0xff
        inc     a
        or      a, a
        jr      NZ,l_main_00104$
        ld      hl, #___str_1
        push    hl
        call    _printf
        pop     af
l_main_00104$:
        ld      -1 (ix), #0xff
        ld      bc,#0x00ff
        inc     bc
        ld      a, b
        or      a,c
        jr      NZ,l_main_00106$
        ld      hl, #___str_2
        push    hl
        call    _printf
        pop     af
l_main_00106$:
        ld      hl, #0x0000
        inc     sp
        pop     ix
        ret
