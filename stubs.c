
#include <canlib.h>

void canInitializeLibrary   (   void        ){

}


int canOpenChannel  (   int     channel, int     flags ){

}


canStatus canSetBusParams   (   const int   hnd,
long    freq,
unsigned int    tseg1,
unsigned int    tseg2,
unsigned int    sjw,
unsigned int    noSamp,
unsigned int    syncmode 
)  {}


canStatus canSetBusOutputControl    (   const int   hnd,
const unsigned int  drivertype 
)   {}


canStatus canBusOn  (   const int   hnd ){}

canStatus canBusOff (   const int   hnd ){}

canStatus canRead   (   const int   hnd,
long *  id,
void *  msg,
unsigned int *  dlc,
unsigned int *  flag,
unsigned long *     time 
)   {}

canStatus canClose  (   const int   hnd )   {}

canStatus canWriteWait  (   const int   hnd,
long    id,
void *  msg,
unsigned int    dlc,
unsigned int    flag,
unsigned long   timeout 
)   {}


canStatus canGetErrorText   (   canStatus   err,
char *  buf,
unsigned int    bufsiz 
)   {}