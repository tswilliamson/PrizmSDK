#ifndef _INPUT_H
#define _INPUT_H

#include <display_syscalls.h>
#include <display.h>
#include <keyboard_syscalls.h>
#include <keyboard.hpp>

#define KEY_HELP KEY_CTRL_F1
#define KEY_LEFT KEY_CTRL_LEFT
#define KEY_RIGHT KEY_CTRL_RIGHT
#define KEY_0 KEY_CHAR_0
#define KEY_1 KEY_CHAR_1
#define KEY_2 KEY_CHAR_2
#define KEY_3 KEY_CHAR_3
#define KEY_4 KEY_CHAR_4
#define KEY_5 KEY_CHAR_5
#define KEY_6 KEY_CHAR_6
#define KEY_7 KEY_CHAR_7
#define KEY_8 KEY_CHAR_8
#define KEY_9 KEY_CHAR_9
#define KEY_A KEY_CTRL_XTT
#define KEY_B KEY_CHAR_LOG
#define KEY_C KEY_CHAR_LN
#define KEY_D KEY_CHAR_SIN
#define KEY_E KEY_CHAR_COS
#define KEY_F KEY_CHAR_TAN
#define KEY_EXIT KEY_CTRL_EXIT
#define KEY_EXE KEY_CTRL_EXE

typedef struct{
	int xpos;
	int ypos;
	int height;
	int maxLen;
	unsigned char*buffer;
} TEditStr;

// Edit a string only allowing numbers.
int EditStr_Execute_Decimal(unsigned char* hint, TEditStr* ed) {
	int key, iresult=0, edpos=0, icontinue=1, idigit;
	MsgBoxPush(ed->height);
	Cursor_SetFlashOn(0);
	locate_OS(ed->xpos, ed->ypos);
	PrintLine(hint, 19);
	while (icontinue) {
		locate_OS(ed->xpos, ed->ypos+1);
		PrintLine(ed->buffer, 19);
		locate_OS(ed->xpos+edpos, ed->ypos+1);
		GetKey(&key);
		idigit = 0;
		switch (key) {
			case KEY_0:
			case KEY_1:
			case KEY_2:
			case KEY_3:
			case KEY_4:
			case KEY_5:
			case KEY_6:
			case KEY_7:
			case KEY_8:
			case KEY_9:
				idigit = key;
				break;
			case KEY_CTRL_RIGHT: 
				if (edpos < ed->maxLen-1) 
					edpos++; 
				break;
			case KEY_CTRL_LEFT: 
				if (edpos > 0) 
					edpos--;
				break;
			case KEY_CTRL_EXIT: 
				icontinue = 0; 
				iresult = 0; 
				break;
			case KEY_CTRL_EXE: 
				icontinue = 0; 
				iresult = 1; 
				break;
		}

		if (idigit && edpos < ed->maxLen) {
			ed->buffer[edpos] = idigit;
			edpos++;
		}
	}
	MsgBoxPop();
	return iresult;
}

#endif