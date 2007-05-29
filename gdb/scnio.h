/*
 *
 *  file      :  SCNIO.H
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THIS FILE CONTAINS ALL THE MODULES
 *               FOR SERVER-CLIENT AND NATIVE FILE IO
 *
 */

#ifndef include_SCNIO
#define include_SCNIO

#include "declare.h"

 __BEGIN_DECLS

#define SCN_NATIVE 0
#define SCN_CLIENT 1
#define SCN_SERVER 2

 extern void * scn_box  ( int   fref, int w, int h, int s,
                                      int x, int y, int wx, int hy );
 extern void   scn_close( int   fref  );
 extern int    scn_fgets( void *addr, int  nbyte, int  fref   );
 extern int    scn_open ( char *path, char *mode, int *sz     );
 extern int    scn_read ( void *addr, int  nbyte, int  fref   );
 extern void   scn_seek ( int   fref, int offset, int  whence );
 extern int    scn_skrd ( int   fref, int  n, int sz, int *offs, void *buf );
 extern void   scn_type ( int   type, char *str );

 __END_DECLS

#endif /* include_SCNIO */
