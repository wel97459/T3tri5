    sep RRET
    align 32
_shiftLeft8:
    ;get the value to shift
    dec SP
    ldi 0
    phi retVal
    glo regArg1
    plo retVal

    ;and get by how much
    ghi retVal
    phi memAddr
    glo regArg2
    plo memAddr
    bz + ;it's zero

-   glo retVal
    shl
    plo retVal
    dec memAddr
    glo memAddr
    bnz -
+   inc SP
    sep RRET

    align 32 
_shiftRight8:
    ;get the value to shift
    dec SP
    ldi 0
    phi retVal
    glo regArg1
    plo retVal
    
    ;and get by how much
    ghi retVal ;get zero
    phi memAddr
    glo regArg2
    plo memAddr
    bz +;it's zero

-   glo retVal
    shr
    plo retVal
    dec memAddr
    glo memAddr
    bnz -
+   inc SP
    sep RRET

    align 32
_shiftLeft16:
    dec SP
    ;get the value to shift
    ghi regArg1
    phi retVal
    glo regArg1
    plo retVal
    
    ;and get by how much
    ghi regArg2
    phi memAddr
    str SP

    glo regArg2
    plo memAddr
    OR
    bz + ;it's zero

-   glo R15
    shl
    plo R15
    ghi R15
    shlc
    phi R15
    dec R14
    glo R14
    bnz -
+   inc SP
    sep RRET

    align 32 
_shiftRight16:
    dec SP
    ;and get by how much
    ghi regArg1
    phi retVal
    glo regArg1
    plo retVal
 
    ;get the value to shift
    ghi regArg2
    phi memAddr
    str SP

    glo regArg2
    plo memAddr
    OR
    bz + ;it's zero

-   ghi retVal
    shr
    phi retVal
    glo retVal
    shrc
    plo retVal
    dec memAddr
    glo memAddr
    bnz -
+   inc SP
    sep RRET

_blink:
    BNQ $$SET
    BQ $$UNSET
$$SET:
    SEQ
    sep RRET
$$UNSET:
    REQ
    sep RRET

_getSP:
    cpy2 retVal, SP
    sep RRET

_setBit:
    cpy2 R8, regArg1
    cpy2 R9, regArg2

    ldi 0
    phi regArg1
    ldi 1
    plo regArg1
    Ccall _shiftLeft8

    sex R8
    glo retVal
    or
    str R8

    sep RRET

_clearBit:
    cpy2 R8, regArg1
    cpy2 R9, regArg2

    ldi 0
    phi regArg1
    ldi 1
    plo regArg1

    Ccall _shiftLeft8

    sex R8
    glo retVal  ;get the shifted value
    xri 255     ;not the the value in D
    and         ;and with the value in pointer R8    
    str R8

    sep RRET

_checkBit:
    cpy2 R8, regArg1
    cpy2 R9, regArg2

    ldi 0
    phi regArg1
    ldi 1
    plo regArg1

    Ccall _shiftLeft8

    sex R8
    glo retVal  ;get the shifted value
    and
    plo retVal

    ldi 0
    phi retVal

    sep RRET
