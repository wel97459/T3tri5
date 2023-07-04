/*
    move_shape example by Marcel van Tongoren 
*/

#include <stdint.h>

#include "devkit/video/vis_video.h"
#include "devkit/input/keyboard.h"
#include "devkit/system/system.h"
#include "devkit/sound/vis_sound.h"

#include "int_handler.h"
#include "func.h"
#include "Tetris.h"
#include "shapes.h"

Tetris tetris;
uint8_t tetris_board[100];
uint16_t vidmem_last[4];
uint16_t vidmem_next_last[4];
uint8_t s = 0, m = 0, h = 0;

#define TETRIS_TOP 0xF850
#define TETRIS_BOT 0xFB98

unsigned char Block_Color(uint8_t c){
	if(c == 0){
		return GREEN_BLOCK;
	}
	if(c == 1){
		return GREEN_BLOCK + 128;
	}
	if(c == 2){
		return BLACK_BLOCK + 128;
	}
	if(c == 3){
		return BLUE_BLOCK;
	}
	if(c == 4){
		return BLUE_BLOCK + 128;
	}
	if(c == 5){
		return CYAN_BLOCK;
	}
	if(c == 6){
		return CYAN_BLOCK + 128;
	}
	return CYAN_BLOCK;
}

unsigned short int loc_add;
void putc(char c){

	vidchar (loc_add, c);
	loc_add++;
}

void printstr(char *ptr){
    while(*ptr){
		putc(*ptr++); //jan 29
	}
}

char * itoa(int s, char *buffer){ //convert an integer to printable ascii in a buffer supplied by the caller
	unsigned int r,k,n;
	unsigned int flag=0;
	char * bptr; bptr=buffer;
	if (s<0){
		*bptr='-';bptr++;
		n=-s;
	} else{
		n=s;
	}
	k=10000;
	while(k>0){
		for(r=0;k<=n;r++,n-=k); // was r=n/k
		if (flag || r>0||k==1){
			*bptr=('0'+r);bptr++;
			flag='y';
		}
		//n=n-r*k;
		k=k/10;
	}

	*bptr='\0';
	return buffer;
}

void printint(int s, unsigned char nZreos){ //print an integer
	char buffer[8];
	itoa(s,buffer);
	printstr(buffer);
}

void Clear(Tetris *t)
{
	unsigned short int vidmem;
	vidclr(0xf800, X_SIZE*Y_SIZE);
	
	shapechar(shapes, 1, 12); // Shape 9 characters as defined above in 'shapes'

	vidchar (TETRIS_TOP+14, TOP_LEFT);
	vidchar (TETRIS_TOP+25, TOP_RIGHT);
	for (vidmem=TETRIS_TOP+15; vidmem<TETRIS_TOP+25; vidmem++){
		vidchar (vidmem, TOP_LINE);
	}
	for (vidmem=TETRIS_BOT+15; vidmem<TETRIS_BOT+25; vidmem++){
		vidchar (vidmem, BOT_LINE);
	}
	vidchar (TETRIS_BOT+14, BOT_LEFT);
	vidchar (TETRIS_BOT+25, BOT_RIGHT);
	for (vidmem=TETRIS_TOP+40; vidmem<TETRIS_BOT; vidmem+=40){
		vidchar (vidmem+14, LEFT_LINE);
		vidchar (vidmem+25, RIGHT_LINE);
	}

	vidchar (Y2VidMem[10]+30, TOP_LEFT);
	vidchar (Y2VidMem[10]+35, TOP_RIGHT);
	vidchar (Y2VidMem[15]+30, BOT_LEFT);
	vidchar (Y2VidMem[15]+35, BOT_RIGHT);

	for (vidmem=Y2VidMem[11]; vidmem<Y2VidMem[15]; vidmem+=40){
		vidchar (vidmem+30, LEFT_LINE);
		vidchar (vidmem+35, RIGHT_LINE);
	}
	for (vidmem=Y2VidMem[10]+31; vidmem<Y2VidMem[10]+35; vidmem++){
		vidchar (vidmem, TOP_LINE);
	}
	for (vidmem=Y2VidMem[15]+31; vidmem<Y2VidMem[15]+35; vidmem++){
		vidchar (vidmem, BOT_LINE);
	}

	textcolor(COLOR_WHITE); 
	loc_add = Y2VidMem[9]+31;
	printstr("NEXT");
}

void BuildSpritePieces(Tetris *t)
{
}


void MovePiece(Tetris *t)
{
    uint8_t i, x, y, l;
	uint16_t vidmem;
	l = vidmem_last[0] != 0;
    x = tetris.piece_x+22;
    y = tetris.piece_y+10;

    for(i=0; i<4; i++)
    {
        if(l) vidchar (vidmem_last[i], ' ');
	}

    for(i=0; i<4; i++)
    {
	    vidmem = Y2VidMem[((T.piece.y[i]>>3) + y)-8] + (((T.piece.x[i]>>3) + x)-8);
        vidchar (vidmem, Block_Color(T.piece.color));
		vidmem_last[i] = vidmem;
    }
}

void ShowNext(Tetris *t)
{
    uint8_t i, x, y, l;
	uint16_t vidmem;
    x = 39;
    y = 19;
	l = vidmem_next_last[0] != 0;
    for(i=0; i<4; i++)
    {
       if(l)	vidchar (vidmem_next_last[i], ' ');
	}

	for(i=0; i<4; i++)
    {
	    vidmem = Y2VidMem[((T.pieceNext.y[i]>>3) + y)-8] + (((T.pieceNext.x[i]>>3) + x)-8);
		vidchar (vidmem, Block_Color(T.pieceNext.color));
		vidmem_next_last[i] = vidmem;
    }
}

void UpdateBoard(Tetris *t)
{
	uint8_t ix, iy, j, n, x, y;
    uint16_t t16;
    n=mul5lookup[T.board_min_y-1];
	t16 = Y2VidMem[2+T.board_min_y];
    for(iy=T.board_min_y-1;iy<20;iy++)
    {

        for (ix = 0; ix < 10; ix += 2)
        {
            j = T.board[n++];
			if((j & 0xf0) == 0x10){
				vidchar (t16+15+ix, ' ');
			}else if(j & 0xf0){
				vidchar (t16+15+ix, Block_Color(((j & 0xf0)>>4)-3));
			}

			if((j & 0x0f) == 0x01){
				vidchar (t16+16+ix, ' ');
			}else if(j & 0x0f){
				vidchar (t16+16+ix, Block_Color((j & 0x0f)-3));
			}
        }
		t16 += 40;
    }
	vidmem_last[0] = 0;
}

void UpdateBoardBlock(Tetris *t, uint8_t xx, uint8_t yy, uint8_t p)
{
}

void RemoveLine(Tetris *t, uint8_t yy)
{
}

void UpdateInfo(Tetris *t)
{ 
	loc_add = Y2VidMem[2];
	textcolor(COLOR_CYAN); printstr("SCORE: "); 
	textcolor(COLOR_WHITE); printint(T.score, 0);
	loc_add = Y2VidMem[3];
	textcolor(COLOR_CYAN); printstr("LINES: "); 
	textcolor(COLOR_WHITE); printint(T.linesRemoved, 0);
	loc_add = Y2VidMem[4];
	textcolor(COLOR_CYAN); printstr("LEVEL: ");
	textcolor(COLOR_WHITE); printint(T.level, 0);
	// loc_add = Y2VidMem[5];
	// textcolor(COLOR_CYAN); printstr("MIN:    ");
	// loc_add -= 3;
	// textcolor(COLOR_WHITE); printint(T.board_min_y, 0);

	if(T.gameover){
		loc_add = Y2VidMem[6];
	    printstr("GAME OVER!");
	}
}

void PlaySound(Tetris *t, uint8_t s)
{
	if(s == 1){
		uint8_t i;
		uint16_t ii;
		for(i=0;i<8;i++){
			generatenoise(i, 2);
			for(ii=0;ii<1200;ii++){
				asm(" nop\n");
			}

		}
		generatenoise(1, 0);
	}
	if(s == 2){
		uint8_t i;
		uint16_t ii;
		for(i=0;i<8;i++){
			generatenoise(i, 2);
			for(ii=0;ii<200;ii++){
				asm(" nop\n");
			}

		}
		generatenoise(1, 0);
	}
	if(s == 3){
		uint8_t i;
		uint16_t ii;
		for(i=0;i<8;i++){
			generatenoise(7-i, 2);
			for(ii=0;ii<200;ii++){
				asm(" nop\n");
			}

		}
		generatenoise(1, 0);
	}
}

void init_game()
{
    tetris.Clear = Clear;
    tetris.BuildPiece = BuildSpritePieces; 
    tetris.MovePiece = MovePiece;
    tetris.ShowNext = ShowNext;
    tetris.UpdateBoard = UpdateBoard;
    tetris.UpdateBoardBlock = UpdateBoardBlock;
    tetris.RemoveLine = RemoveLine;
	tetris.UpdateInfo = UpdateInfo;
	tetris.PlaySound = PlaySound;
    tetris_init(&tetris, tetris_board);
}

void main()
{
	int x, y, c;
    unsigned char key;

	disableinterrupt();
    initvideo();                            // Initialize VIS video output
    setvideobase(0xf800);                   // Set video base address to 0xF800, needed for vidstrcpy routines

	init_game();

    textcolor(COLOR_WHITE);              // Set text color to white
    vidstrcpyxy(9,1,"T3TRI5 BY WEL97459 2023");

	x = X_SIZE>>1;
    y = Y_SIZE>>1;               // Set x and y to middle of screen

    // int_handler_init();
	// enableinterrupt();
	while (1){
		if(check_vsync()){
			if(kbhit()){
				key = cgetc();
				// loc_add = Y2VidMem[7];
				// printstr("KEY:     "); 
				// loc_add -= 4;
				// printint(key, 0);
			}else{
				key=0;
			}
			tetris_tick(&tetris);
			tetris.move = 0;
			if(key == 0x77 || key == 0x82){ //Up
				setBit(&tetris.move, 3);
			}else if(key == 0x64 || key == 0x83) { //Right
				setBit(&tetris.move, 1);
			}else if(key == 0x61 || key == 0x84) { //Left
				setBit(&tetris.move, 2);
			}else if(key == 0x73 || key == 0x85) { //Down
				setBit(&tetris.move, 0);
			}else if(key == 0x70){ //P - Pause
				setBit(&tetris.move, 4);
				loc_add = Y2VidMem[6];
				if(tetris.pause){
					printstr("       ");
				} else {
					printstr("PAUSED.");
				}
			}
			if(key == 0x63 || tetris.gameover & key != 0){ //C - Reset
				init_game();
			}
			
		}	
    }
}

#include "TinyTetris/Tetris.c"
