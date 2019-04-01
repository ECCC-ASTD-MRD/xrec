/*
 *
 *  file      :  dbinit.c
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
 *               MANAGING THE INITIALIZATION OF THE
 *               GEOGRAPHICAL DATABASE
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbinit.h"
#include "desc.h"

/*
 *  DEFAULT server and port 
 */

    int  gdb_client      = 0;
    int  gdb_port        = 1377;
    char gdb_server[256] = "foo";
    char gdb_strsrv[256] = "foo:1377";

/*
 *  DEFAULT directories 
 */

    char gdb_dbase[256]  = "/data/cmds/afsm/gis_le";
    char gdb_dreso[8][4] = { "001", "002", "004", "008",
                             "016", "032", "064", "128" };
/*
 *  DEFAULT level descriptions
 *                          bottom   left   top   right    w      h     t  */

    Desc gdb_desc[8] = { {  -90.0, -180.0, 90.0, 180.0,   361,   181, 360 },
                         {  -90.0, -180.0, 90.0, 180.0,   721,   361, 360 },
                         {  -90.0, -180.0, 90.0, 180.0,  1441,   721, 360 },
                         {  -90.0, -180.0, 90.0, 180.0,  2881,  1441,  90 },
                         {  -90.0, -180.0, 90.0, 180.0,  5761,  2881,  45 },
                         {  -90.0, -180.0, 90.0, 180.0, 11521,  5761,  20 },
                         {  -90.0, -180.0, 90.0, 180.0, 23041, 11521,  10 },
                         {  -90.0, -180.0, 90.0, 180.0, 46081, 23041,  10 }
                       } ;



/*
 *
 *  module    :  DBINIT_DESC
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  
 *               dbinit_desc();
 *
 *  object    :  INITIALIZATION OF LEVEL DESCRIPTIONS
 *               OVERWRITING DEFAULT IF EVERYTING GOES WELL
 */

 extern int
 dbinit_desc( )
    {
    char  path[512];
    int   i,ierr;
    Desc *d;

    printf("Using GDB: %s\n", gdb_dbase );

    for( i=0; i<8; ++i )
       {
       strcpy(path,gdb_dbase);
       strcat(path,"/");
       strcat(path,gdb_dreso[i]);
       strcat(path,"/");
       strcat(path,"desc");
       ierr = dsc_read(path,(char **)&d);
       if( ierr < 0 )
         {
         printf(" Error gdb_init_desc : reading %s\n",path);
         return(1);
         }

       gdb_desc[i].bottom = d->bottom;
       gdb_desc[i].left   = d->left;
       gdb_desc[i].top    = d->top;
       gdb_desc[i].right  = d->right;
       gdb_desc[i].w      = d->w;
       gdb_desc[i].h      = d->h;
       gdb_desc[i].t      = d->t;

       free(d);
       }

    return(0);
    }


/*
 *
 *  module    :  DBINIT_PATH
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  
 *               dbinit_path();
 *
 *  object    :  INITIALIZATION OF THE DBASE server:port/path
 *               USER GIVES IT PREFERENCE ON THE GIS SERVICE
 *               THROUGH THE GDB_PATH ENVIRONMENT VARIABLE
 */

 extern int
 dbinit_path( )
    {
    char *env;
    char *ptr;
    int   i;

/*
 *  get GDB_PATH : if not given uses defaults
 */
    env = getenv("GDB_PATH");
    if( env == NULL ) return(0);

/*
 *  get dbase path
 */
    ptr = strchr(env,'/');
    if( ptr == NULL ) { printf(" Error GDB_PATH : no path\n"); return(-1); } 
    strcpy(gdb_dbase,ptr);

/*
 *  no server port
 */
    if( ptr == env )
      {
      gdb_strsrv[0] = '\0';
      gdb_client    =   0;
      return(0);
      }

/*
 *  is there a port
 */
    ptr[0] = '\0';
    ptr = strchr( env,':' );
    if( ptr == NULL ) { printf(" Error GDB_PATH : no port\n"); return(-2); } 

/*
 *  get server name or number
 */
    ptr[0] = '\0';
    if( ptr != env ) strcpy( gdb_server, env );

/*
 *  get port number
 */
    i = sscanf( &ptr[1], "%d", &gdb_port );
    if( i != 1 ) { printf(" Error GDB_PATH : bad port\n"); return(-3); } 

/*
 *  set the server-port string
 */
    ptr[0] = ':';
    strcpy( gdb_strsrv, env );
    gdb_client = 1;

    return(0);
    }
