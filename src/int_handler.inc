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