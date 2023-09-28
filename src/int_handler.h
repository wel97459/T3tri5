#ifndef _INT_HANDLER_H
#define _INT_HANDLER_H
    uint8_t IntHandlerValue;

    void int_handler_includer(){
		asm(" include int_handler.inc\n");
	}

    void int_handler_init();
    uint8_t int_handler_check();
    uint8_t check_vsync();
    void load_shape(uint8_t ch, uint8_t len, const unsigned char *data);

#endif //_INT_HANDLER_H
