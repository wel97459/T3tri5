#ifndef _FUNC_H
#define _FUNC_H




uint8_t shiftLeft8(uint8_t v, uint8_t a);
uint8_t shiftRight8(uint8_t v, uint8_t a);

uint16_t shiftLeft16(uint16_t v, uint8_t a);
uint16_t shiftRight16(uint16_t v, uint8_t a);

void blink();
void setBit(uint8_t *v, uint8_t b);
void clearBit(uint8_t *v, uint8_t b);
uint8_t checkBit(uint8_t *v, uint8_t b);
uint16_t getSP();

void func_includer()
{
    asm(" include func.inc\n");
}

#endif //_FUNC_H
