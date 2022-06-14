/*
 *
 *  file      :  GDB.H
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
 *               NEEDED FOR THE SMC GIS DATABASES
 *
 *
 */

#ifndef include_GDB
#define include_GDB

#include "declare.h"

 __BEGIN_DECLS

/*
 *  fill types
 */

 enum GDB_FILS { GDB_FIL_MAX  = 4,      /* Nb of entries     */
                 GDB_FIL_LAND = 1,      /* land              */
                 GDB_FIL_LAKE,          /* lake              */
                 GDB_FIL_LAND2,         /* island            */
                 GDB_FIL_LAKE2          /* lake in island    */
               };

/*
 *  line types
 */

 enum GDB_LINS { GDB_LIN_MAX   = 16,    /* Nb of entries     */
                 GDB_LIN_ADMIN = 1,     /* internal politics */
                 GDB_LIN_CANAL,         /* canal / aqueduc   */
                 GDB_LIN_CITY,          /* city              */
                 GDB_LIN_COAST,         /* coast             */
                 GDB_LIN_CULT,          /* cultural          */
                 GDB_LIN_DATE,          /* date line         */
                 GDB_LIN_ICE,           /* ice, glacier...   */
                 GDB_LIN_LAKE,          /* lake              */
                 GDB_LIN_PHYS,          /* physiography      */
                 GDB_LIN_POLIT,         /* political         */
                 GDB_LIN_RAIL,          /* railroad          */
                 GDB_LIN_ROAD,          /* road              */
                 GDB_LIN_RIVER,         /* river             */
                 GDB_LIN_SALIN,         /* saline            */
                 GDB_LIN_SEA,           /* sea features      */
                 GDB_LIN_UTIL           /* utility           */
               };

/*
 *  map types
 */

 enum GDB_MAPS { GDB_MAP_MAX = 5,       /* Nb of entries     */
                 GDB_MAP_BAT = 1,       /* bathymetry        */
                 GDB_MAP_DEM,           /* elevation         */
                 GDB_MAP_MSK,           /* land/sea mask     */
                 GDB_MAP_TER,           /* terrain type      */
                 GDB_MAP_TEX            /* texture           */
               };

/*
 *  point types
 */

 enum GDB_PNTS { GDB_PNT_MAX   = 3,     /* Nb of entries     */
                 GDB_PNT_ISLND = 1,     /* islands           */
                 GDB_PNT_LAKE,          /* lake              */
                 GDB_PNT_SEA            /* sea feature       */
               };

/*
 *  text types
 */


 enum GDB_TXTS { GDB_TXT_MAX  = 8,      /* Nb of entries     */
                 GDB_TXT_AIRP = 1,      /* airport           */
                 GDB_TXT_CULT,          /* cultural          */
                 GDB_TXT_CITY,          /* cily              */
                 GDB_TXT_DRAIN,         /* drainage          */
                 GDB_TXT_ISLND,         /* island (contry)   */
                 GDB_TXT_POLIT,         /* political         */
                 GDB_TXT_RAIL,          /* railroad          */
                 GDB_TXT_ROAD           /* road              */
               };


/*
 *  transmitters
 */

 typedef void (*GDB_LINE )( int type, int     n, 
                            float mnlat, float mnlon, float mxlat, float mxlon,
                            float *ll );
 typedef void (*GDB_POINT)( int type, float lat, float lon );
 typedef void (*GDB_TEXT )( int type, float lat, float lon, char *str );

/*
 *  initialisation, set up  and log
 */

 extern int  gdb_init ( );
 extern int  gdb_limit( float bottom, float left, float top, float right );
 extern void gdb_log  ( );

/*
 *  selecting data 
 */

 extern void gdb_fill ( int reso, int typ, GDB_LINE  module );
 extern void gdb_line ( int reso, int typ, GDB_LINE  module );
 extern void gdb_point( int reso, int typ, GDB_POINT module );
 extern void gdb_text ( int reso, int typ, GDB_TEXT  module );

 extern void gdb_map  ( int reso, int typ, char **map, int *w, int *h, int *d );

/*
 *  asking for rmn stuff
 */

 extern void gdb_rmn  ( int reso, char *typ,  int *ni,  int *nj,
                                  int *ig1,   int *ig2, int *ig3, int *ig4,
                                  char *vtyp, float **xdef, float **ydef );

/*
 *  specific map stuff 
 */

 extern void   gdb_mapclose( int imfp );
 extern char * gdb_mapget  ( int imfp, float lat, float lon, char *buf );
 extern char * gdb_mapgetv ( int imfp, int n, float *ll, char *buf );
 extern int    gdb_mapopen ( int reso, int type, int *d );

 __END_DECLS

#endif /* include_GDB */
