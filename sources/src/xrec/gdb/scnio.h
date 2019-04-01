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
#include <sys/types.h>

 __BEGIN_DECLS

#define SCN_NATIVE 0
#define SCN_CLIENT 1
#define SCN_SERVER 2

typedef  int64_t  fileref_t ;

 extern void *     scn_box  ( fileref_t fref, int w, int h, int s,
                                      int x, int y, int wx, int hy );
 extern void       scn_close( fileref_t fref  );
 extern int        scn_fgets( void *addr, int  nbyte, fileref_t  fref   );
 extern fileref_t  scn_open ( void *addr, char *mode, int *sz     );
 extern int        scn_read ( void *addr, int  nbyte, fileref_t  fref   );
 extern void       scn_seek ( fileref_t fref, int offset, int  whence );
 extern int        scn_skrd ( fileref_t fref, int  n, int sz, int64_t *offs, void *buf );
 extern void       scn_type ( int   type, char *str );

 __END_DECLS

#endif /* include_SCNIO */
