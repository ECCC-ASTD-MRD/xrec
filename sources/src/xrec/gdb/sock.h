/*
 *
 *  file      :  SOCK.H
 *
 *  author    :  MICHEL GRENIER
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THIS FILE CONTAINS ALL THE DECLARATIONS
 *               NEEDED FOR SOCKETS
 *
 */

#ifndef include_SOCK
#define include_SOCK

#include "declare.h"

 __BEGIN_DECLS

 extern void   sclose( int sock );
 extern int    sopen ( char *hostport, char *mode );
 extern int    sread ( int sock, char *buf, size_t nbyte);
 extern int    swrite( int sock, char *buf, size_t nbyte);

 __END_DECLS

#endif /* include_SOCK */
