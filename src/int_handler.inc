 align 16
INT_Exit:
    popr MEMADDR
    LDXA
    RET
Int_Handler:
    DEC SP
	SAV
	DEC SP
	STR SP

    pushr MEMADDR
    ldireg MEMADDR, _IntHandlerValue
    ldn MEMADDR

    B1 $$EF1
		ORI 0x1
$$EF1:

;    BN2 $$EF2
;		ORI 0x2
;$$EF2:

;    BN3 $$EF3
;		ORI 0x4
;$$EF3:
;
;    BN4 $$EF4
;		ORI 0x8
;$$EF4:

    str MEMADDR

    BR INT_Exit

_int_handler_init:   
    ldireg R1, Int_Handler
    Cretn 

 align 16
_int_handler_check:
    ldaD R15, 0;
    bn1 + 
    ldireg MEMADDR, _IntHandlerValue
    ldn MEMADDR
    xri 0x1 ;unset it
    str MEMADDR
    xri 0x1 ;reset it
    ani 0x1 ;check it
    bz +
    ldaD R15,1;
+
 align 32
    Cretn
_check_vsync:
    ldaD R15, 0;
    ldireg MEMADDR, _IntHandlerValue
    bn1 + 
    ldn MEMADDR
    bnz set_R15
    ldi 1
    plo R15
    str MEMADDR
    Cretn
+
    ldi 0
    str MEMADDR
set_R15:
    Cretn

_display_n5_status equ 0x41c3

_load_shape:
    pushr MEMADDR
    ldireg MEMADDR, _display_n5_status
    lda MEMADDR
    ori 01 ;Set Cmem access
    plo retVal
    ldn MEMADDR
    phi retVal
    sex retVal
    out 5

    ldi 0xc0
    plo MEMADDR
    ldn MEMADDR
    ori 10
    sex SP
    str SP
    out 3 ;disable display
    dec SP

    ldi 0xcb
    plo MEMADDR
    ldn MEMADDR
    str SP

    ldi 0xfb
    phi MEMADDR
    ldi 0xc0
    plo MEMADDR
    sex MEMADDR
    out 6 ;Page Address Register
    dec MEMADDR

    glo regArg2
    phi regArg2
    sex SP
load_next_loop:

    ldi 0x41
    phi MEMADDR
    ldi 0xca ; get screen mode NTSC/PAL
    plo MEMADDR
    ldn MEMADDR
    plo regArg2

    ldi 0xfb
    phi MEMADDR
    ldi 0xc0
    plo MEMADDR
    glo regArg1
-
    b1 - 
    str MEMADDR
    inc regArg1

    ldi 0xf4
    phi MEMADDR
    ldi 0
    plo MEMADDR

load_loop:
    lda R11 ;Get char data
-
    b1 -
    str MEMADDR
    inc MEMADDR
    dec regArg2
    glo regArg2
    bnz load_loop

    ghi regArg2
    smi 0x01
    phi regArg2
    bnz load_next_loop

    ldi 0
    phi retVal

    ldireg MEMADDR, _display_n5_status
    lda MEMADDR
    plo retVal
    ldn MEMADDR
    phi retVal
    sex retVal
    out 5

    ldi 0xc0
    plo MEMADDR
    ldn MEMADDR
    sex SP
    str SP
    out 3 ;enable display
    dec SP

    popr MEMADDR
    Cretn