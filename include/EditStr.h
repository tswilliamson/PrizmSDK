typedef struct{
	int xpos;
	int ypos;
	int height;
	unsigned char*buffer;
} TEditStr;

int EditStr_Execute( unsigned char*hint, TEditStr*ed ) {
	int key, iresult=0, edpos=0, icontinue=1, idigit;
	MsgBoxPush( (*ed).height );
	locate_OS( (*ed).xpos, (*ed).ypos );
	PrintLine( hint, 19 );
	if ( InputAny( (*ed).buffer, (*ed).xpos, (*ed).ypos+1, 8, edpos, emHEX ) == KEY_CTRL_EXIT ) iresult = 0;
	else iresult = 1;

	MsgBoxPop();
	return iresult;
}