#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#define FRANCAIS 0
#define ENGLISH  1


typedef struct
{
   XtAppContext contexte;
   Widget topLevel;
   } SuperWidgetStruct;

