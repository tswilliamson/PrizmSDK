#ifndef _FXCG_MINICOMPAT
# error Included app.h without minicompat. Use <fxcg/app.h>.
#else

// No guards, whatever. It's "legacy".

void APP_FINANCE( int, int );
void APP_SYSTEM_BATTERY( int opt );
void APP_SYSTEM_DISPLAY( int );
void APP_SYSTEM_LANGUAGE( int );
void APP_SYSTEM_POWER( int opt );
void APP_SYSTEM_RESET( void );
void APP_SYSTEM_VERSION( void );
void APP_SYSTEM( void );

void ResetAllDialog( void );
unsigned char*GetAppName( unsigned char*name );

#endif
