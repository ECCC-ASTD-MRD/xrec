/*
 *
 *  file      :  DBINIT.H
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
 *               NEEDED FOR THE GEOGRAPHICAL DATABASE
 *
 */

#ifndef include_DBINIT
#define include_DBINIT

#include "desc.h"
#include "declare.h"

 __BEGIN_DECLS

/*
 *  server port
 */

 extern int  gdb_client;
 extern int  gdb_port;
 extern char gdb_server[256];
 extern char gdb_strsrv[256];

/*
 *  directories 
 */

 extern char gdb_dbase[256];
 extern char gdb_dreso[8][4];

/*
 *  level descriptions
 */

 extern Desc gdb_desc[8];

/*
 *  initialization modules
 */

 extern int dbinit_desc( );
 extern int dbinit_path( );

 __END_DECLS

#endif /* include_DBINIT */
