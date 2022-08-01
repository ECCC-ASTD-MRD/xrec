/*
 *
 *  module    :  GDB
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THE GDB PACKAGE FROM THE OUTSIDE
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbinit.h"
#include "gdb.h"
#include "map.h"
#include "pln.h"
#include "pol.h"
#include "rgis.h"
#include "scnio.h"
#include "swp.h"

#define  gdb_chkreso(res,reso) switch( reso )                    \
                                     {                           \
                                     case   1 : res =  0; break; \
                                     case   2 : res =  1; break; \
                                     case   4 : res =  2; break; \
                                     case   8 : res =  3; break; \
                                     case  16 : res =  4; break; \
                                     case  32 : res =  5; break; \
                                     case  64 : res =  6; break; \
                                     case 128 : res =  7; break; \
                                     default  : res = -1;        \
                                     }

 static float usr_bottom =  -90.0;
 static float usr_left   = -180.0;
 static float usr_top    =   90.0;
 static float usr_right  =  180.0;

/*
 * fill/line  out layer
 */

 static GDB_LINE out_module;
 static int      out_type;

 extern void
 gdb_out( int n, float minx, float miny, float maxx, float maxy, float *v )
    {
    (*out_module)( out_type, n, minx, miny, maxx, maxy, v );
    }


/*
 *
 *  module    :  GDB_CHKMAP
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  Desc *desc;
 *               int   reso;
 *               int  *x, *y, *w, *h;
 *               gdb_chkmap(desc,reso,x,y,w,h);
 *
 *  object    :  CHECK POSITION AND DIMENSION OF MAP
 *               FOR USER LIMIT AND RESOLUTION
 */

 static int
 gdb_chkmap( Desc *desc, int reso, int *x, int *y, int *w, int *h )
    {
    int  x2, y2;

    if( usr_left   > desc->right  ) return(-1);
    if( usr_right  < desc->left   ) return(-1);
    if( usr_bottom > desc->top    ) return(-1);
    if( usr_top    < desc->bottom ) return(-1);

    *x = (int)(( usr_left  - desc->left ) * reso );
    *y = (int)(( desc->top - usr_top    ) * reso );
    x2 = (int)(( usr_right - desc->left ) * reso );
    y2 = (int)(( desc->top - usr_bottom ) * reso );

    if( *x  <       0 ) *x = 0;
    if( *y  <       0 ) *y = 0;
    if( x2 >= desc->w ) x2 = desc->w - 1;
    if( y2 >= desc->h ) y2 = desc->h - 1;

    if( x2 < *x || y2 < *y ) return(-1);

    *w = x2 - *x + 1;
    *h = y2 - *y + 1;

    return(0);
    }


/*
 *
 *  module    :  GDB_CHKTILES
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  Desc *desc;
 *               int  *lat0,*lat1;
 *               int  *lon0,*lon1;
 *               gdb_chktiles(desc,lat0,lat1,lon0,lon1);
 *
 *  object    :  CHECK POSITION AND DIMENSION OF MAP
 *               FOR USER LIMIT AND RESOLUTION
 */

 static void
 gdb_chktiles( Desc *desc, int *lat0, int *lat1, int *lon0, int *lon1)
    {
    int ilat,ilon;
    int lat,lon;

    ilat = desc->t;
    ilon = desc->t;

    if( ilon == 360 )
      {
      *lat0 = -90;
      *lat1 = -90;
      *lon0 = -180;
      *lon1 = -180;
      return;
      }

    lat = (int)usr_bottom;
    if( lat % ilat != 0 ) lat = lat/ilat * ilat;
    if( lat < 0  ||
      ( lat == 0 && usr_bottom < 0.0 ) ) lat -= ilat;
   *lat0 = lat;

    lat = (int)usr_top;
    if( lat % ilat != 0 ) lat = lat/ilat * ilat;
    if( lat <  0 ||
      ( lat == 0 && usr_top < 0.0 ) ) lat -= ilat;
   *lat1 = lat;

    lon = (int)usr_left;
    if( lon % ilon != 0 ) lon = lon/ilon * ilon;
    if( lon <  0 ||
      ( lon == 0 && usr_left < 0.0 ) ) lon -= ilon;
   *lon0 = lon;

    lon = (int)usr_right;
    if( lon % ilon != 0 ) lon = lon/ilon * ilon;
    if( lon <  0 ||
      ( lon == 0 && usr_right < 0.0 ) ) lon -= ilon;
   *lon1 = lon;

    }


/*
 *
 *  module    :  GDB_FILL
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
 *               int       reso;
 *               int       type;
 *               GDB_LINE  module;
 *               gdb_fill( reso, type, module );
 *
 *  object    :  READ IN A LIMITED (GDB_LIMIT) SET OF LINES
 *
 */

 extern void
 gdb_fill( int reso, int type, GDB_LINE module )
    {
    char  name[25];
    char  path[256];
    char *ptr;
    int   ilat,ilon;
    int   lat, lon;
    int   lat0,lat1;
    int   lon0,lon1;
    int   i,siz;
    int   res;
    Lls  *lls;

/*
 *  check reso
 */
    gdb_chkreso(res,reso);
    if( res == -1 )
      {
      printf(" Error gdb_fill : reso  out of range\n");
      return;
      }

/*
 *  check type
 */
    switch( type )
      {
      case GDB_FIL_LAND  : strcpy(name,"pol_99_01"); break;
      case GDB_FIL_LAKE  : strcpy(name,"dnl_99_01"); break;
      case GDB_FIL_LAND2 : strcpy(name,"pol_99_02"); break;
      case GDB_FIL_LAKE2 : strcpy(name,"dnl_99_02"); break;
      default            : printf(" Error gdb_fill : type not supported\n");
                           return;
      }

/*
 *  building the path
 */
    strcpy(path,gdb_dbase);
    strcat(path,"/");
    strcat(path,gdb_dreso[res]);
    strcat(path,"/");
    ptr = &path[strlen(path)];

/*
 *  check tiles
 */
    gdb_chktiles(&gdb_desc[res],&lat0,&lat1,&lon0,&lon1);

/*
 *  setup output
 */
    out_type   = type;
    out_module = module;

/*
 *  loop on all the tiles
 */
    ilat = gdb_desc[res].t;
    ilon = gdb_desc[res].t;
    for( lon=lon0; lon<=lon1; lon+=ilon )
       {
       for( lat=lat0; lat<=lat1; lat+=ilat )
          {
          sprintf(ptr,"%.3d/%.2d/%s",lon,lat,name);
          if( rgis_reads(path,(char **)&lls,&siz) != 0 ) continue;

          for( i=0; i<lls->ns; ++i )
             {
             pol_cutBox ( lls->sl[i]-2,
                          lls->seg[i][0], lls->seg[i][1],
                          lls->seg[i][2], lls->seg[i][3],
                         &lls->seg[i][4],
                          usr_bottom, usr_left, usr_top, usr_right,
                          gdb_out );
             }

          rgis_free(lls);
          }
       }
    }


/*
 *
 *  module    :  GDB_INIT
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
 *               gdb_init();
 *
 *  object    :  STARTING INITIALIZATION
 */

 extern int
 gdb_init()
    {
    static int  done = 0;
    static int  i    = 0;

    if( done != 0 ) return i;

    (void) dbinit_path();

    if( gdb_client == 1 ) scn_type ( SCN_CLIENT, gdb_strsrv );

    i = dbinit_desc();

    done = 1;

    return i;
    }


/*
 *
 *  module    :  GDB_LIMIT
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  float bottom, left, top, right;
 *               gdb_limit( bottom, left, top, right)
 *
 *  object    :  LIMITING THE ACCESS TO THE DATABASE
 */

 extern int
 gdb_limit(float bottom, float left, float top, float right)
    {
    float tmp;

    if( bottom >=   top ) return(-1);
    if( left   >= right ) return(-1);

    usr_bottom = bottom;
    usr_left   = left;
    usr_top    = top;
    usr_right  = right;

    if( usr_top < usr_bottom )
      {
      tmp        = usr_top;
      usr_top    = usr_bottom;
      usr_bottom = tmp;
      }

    if( usr_right < usr_left )
      {
      tmp        = usr_right;
      usr_right  = usr_left;
      usr_left   = tmp;
      }

    if( usr_bottom <  -90.0 ) usr_bottom =  -90.0;
    if( usr_top    >   90.0 ) usr_top    =   90.0;
    if( usr_left   < -180.0 ) usr_left   = -180.0;
    if( usr_right  >  180.0 ) usr_right  =  180.0;

    return(0);
    }


/*
 *
 *  module    :  GDB_LINE
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
 *               int       reso;
 *               int       type;
 *               GDB_LINE  module;
 *               gdb_line( reso, type, module );
 *
 *  object    :  READ IN A LIMITED (GDB_LIMIT) SET OF LINES
 *
 */

 extern void
 gdb_line( int reso, int type, GDB_LINE module )
    {
    char  name[25];
    char  path[256];
    char *ptr;
    int   ilat,ilon;
    int   lat, lon;
    int   lat0,lat1;
    int   lon0,lon1;
    int   i,siz;
    int   res;
    Lls  *lls;

/*
 *  check reso
 */
    gdb_chkreso(res,reso);
    if( res == -1 )
      {
      printf(" Error gdb_line : reso  out of range\n");
      return;
      }

/*
 *  check type
 */
    switch( type )
      {
      case GDB_LIN_ADMIN : strcpy(name,"pol_04");
                           break;
      case GDB_LIN_CANAL : strcpy(name,"dnl_02_04");
                           if( reso < 128 ) return;
                           break;
      case GDB_LIN_CITY  : strcpy(name,"ppl_00");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_COAST : strcpy(name,"pol_10_01");
                           break;
      case GDB_LIN_CULT  : strcpy(name,"cll_00");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_DATE  : strcpy(name,"pol_13");
                           break;
      case GDB_LIN_ICE   : strcpy(name,"icl_00");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_LAKE  : strcpy(name,"dnl_02_01");
                           break;
      case GDB_LIN_PHYS  : strcpy(name,"phl_00");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_POLIT : strcpy(name,"pol_01");
                           break;
      case GDB_LIN_RAIL  : strcpy(name,"rrl_00");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_ROAD  : strcpy(name,"rdl_00");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_RIVER : strcpy(name,"dnl_01");
                           if( reso <  16 ) return;
                           break;
      case GDB_LIN_SALIN : strcpy(name,"dnl_02_04");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_SEA   : strcpy(name,"ofl_00");
                           if( reso <  64 ) return;
                           break;
      case GDB_LIN_UTIL  : strcpy(name,"utl_00");
                           if( reso <  64 ) return;
                           break;
      default            : printf(" Error gdb_line : type not supported\n");
                           return;
      }

/*
 *  building the path
 */
    strcpy(path,gdb_dbase);
    strcat(path,"/");
    strcat(path,gdb_dreso[res]);
    strcat(path,"/");
    ptr = &path[strlen(path)];

/*
 *  check tiles
 */
    gdb_chktiles(&gdb_desc[res],&lat0,&lat1,&lon0,&lon1);

/*
 *  setup output
 */
    out_type   = type;
    out_module = module;

/*
 *  loop on all the tiles
 */
    ilat = gdb_desc[res].t;
    ilon = gdb_desc[res].t;
    for( lon=lon0; lon<=lon1; lon+=ilon )
       {
       for( lat=lat0; lat<=lat1; lat+=ilat )
          {
          sprintf(ptr,"%.3d/%.2d/%s",lon,lat,name);
          if( rgis_reads(path,(char **)&lls,&siz) != 0 ) continue;

          for( i=0; i<lls->ns; ++i )
             pln_cutBox ( lls->sl[i]-2,
                          lls->seg[i][0], lls->seg[i][1],
                          lls->seg[i][2], lls->seg[i][3],
                         &lls->seg[i][4],
                          usr_bottom, usr_left, usr_top, usr_right,
                          gdb_out );
          rgis_free(lls);
          }
       }
    }


/*
 *
 *  module    :  GDB_MAP
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  READINGING IN A LIMITED (GDB_LIMIT) MAP
 */

 static void gdb_mapt128 ( int, char **, int *, int *, int * );

 extern void
 gdb_map ( int reso, int type, char **map, int *w, int *h, int *d )
    {
    char  path[256];
    char  name[25];
    int   x,y;
    int   res;

/*
 *  special case texture a resolution 128 as a split file
 */
    if( type == GDB_MAP_TEX && reso == 128 )
      {
      gdb_mapt128( reso, map, w, h, d );
      return;
      }

/*
 *  initialization
 */
    *map = (char *)NULL;
    *w   = 0;
    *h   = 0;
    *d   = 0;

/*
 *  check reso
 */
    gdb_chkreso(res,reso);
    if( res == -1 )
      {
      printf(" Error gdb_map : reso  out of range\n");
      return;
      }

/*
 *  check type
 */
    switch( type )
      {
      case GDB_MAP_BAT : strcpy(name,"BAT"); *d = 2; break;
      case GDB_MAP_DEM : strcpy(name,"DEM"); *d = 2; break;
      case GDB_MAP_MSK : strcpy(name,"MSK"); *d = 1; break;
      case GDB_MAP_TER : strcpy(name,"TER"); *d = 1; break;
      case GDB_MAP_TEX : strcpy(name,"TEX"); *d = 3; break;
      default          : printf(" Error gdb_map : type not supported\n");
                         return;
      }

/*
 *  check map
 */
    if( gdb_chkmap(&gdb_desc[res],reso,&x,&y,w,h) < 0 )
      {
      printf(" Error gdb_map : limit out of dbase range\n");
      return;
      }

/*
 *  building the path
 */
    strcpy(path,gdb_dbase);
    strcat(path,"/");
    strcat(path,gdb_dreso[res]);
    strcat(path,"/");
    strcat(path,name);

/*
 *  read in the information
 */
   *map = map_box( path, gdb_desc[res].w, gdb_desc[res].h, *d, x,y,*w,*h );

/*  if( *map == NULL )
      {
      printf(" Error gdb_map : not enough memory ???\n");
      return;
      } */

    return;
    }

 static void
 gdb_mapt128 ( int reso, char **map, int *w, int *h, int *d )
    {
    char  path[256];
    char *buf;
    char *mp;
    int   h1,h2;
    int   x,y,y2,ylimit;
    int   res;
    int   tot;

/*
 *  initialization
 */
    *map    = (char *)NULL;
    *w      = 0;
    *h      = 0;
    *d      = 0;
     res    = 7;
     ylimit = 11520;

/*
 *  check map
 */
    if( gdb_chkmap(&gdb_desc[res],reso,&x,&y,w,h) < 0 )
      {
      printf(" Error gdb_map : limit out of dbase range\n");
      return;
      }
    *d = 3;
    y2 = y + *h - 1;

/*
 *  building the path
 */
    strcpy(path,gdb_dbase);
    strcat(path,"/");
    strcat(path,gdb_dreso[res]);
    strcat(path,"/TEX");

/*
 *  a simple read from first file
 */
    if( y2 <= ylimit )
      {
      *map = map_box( path, gdb_desc[res].w, ylimit+1, *d, x,y,*w,*h );
/*    if( *map == NULL )
        {
        printf(" Error gdb_map : out of memory\n");
        return;
        } */

      return;
      }

/*
 *  a simple read from second file
 */
    if( y > ylimit )
      {
      strcat(path,"2");
      y   -= ylimit + 1;
      *map = map_box( path, gdb_desc[res].w, ylimit,   *d, x,y,*w,*h );
/*    if( *map == NULL )
        {
        printf(" Error gdb_map : out of memory\n");
        return;
        } */

      return;
      }

/*
 *  going from one file to the other; allocate space
 */
    tot = *w * *h * *d;
    mp  = (char *) malloc ( tot * sizeof(char) );
    if( mp == NULL )
      {
      printf(" Error gdb_map : not enough memory ???\n");
      return;
      }

/*
 *  reading  first block from first file
 */
    h1  = ylimit - y + 1;
    buf = (char *)map_box( path, gdb_desc[res].w, ylimit+1, *d, x,y,*w,h1 );
    if( buf == NULL )
      {
      free((char *)mp);
/*    printf(" Error gdb_map : not enough memory ???\n"); */
      return;
      }
    tot = *w *h1* *d;
    memcpy(mp, buf, tot);
    free(buf);


/*
 *  reading  first block from first file
 */
    strcat(path,"2");
    h2  = *h - h1;
    buf = (char *)map_box( path, gdb_desc[res].w, ylimit,   *d, x,0,*w,h2 );
    if( buf == NULL )
      {
      free((char *)mp);
/*    printf(" Error gdb_map : not enough memory ???\n"); */
      return;
      }
    memcpy( &mp[tot], buf, *w *h2* *d);
    free(buf);

    *map = (char *)mp;

    }


/*
 *
 *  module    :  GDB_MAPOPEN
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  OPEN FOR SEEKING A MAP
 */

 typedef struct {
                int  magic;
                int64_t  fp1;
                int64_t  fp2;
                int  type;
                int  reso;
                int  res;
                int  d;
                } Mapfp;

 extern void
 gdb_mapclose ( caddr_t imfp )
    {
    Mapfp *mfp = (Mapfp *)imfp;

    if( mfp        == NULL ) return;
    if( mfp->magic != 1234 ) return;

    scn_close(mfp->fp1);
    if( mfp->fp2 != 0 ) scn_close(mfp->fp2);

    free(mfp);
    
    return;
    }

 extern char *
 gdb_mapget ( caddr_t imfp, float lat, float lon, char *buf )
    {
    int    pos;
    int    w,h,d;
    int    x,y;
    int64_t    fp;
    Mapfp *mfp = (Mapfp *)imfp;
    int    ylimit;

    if( mfp        == NULL ) return((char *)NULL);
    if( mfp->magic != 1234 ) return((char *)NULL);

    d = mfp->d;
    w = gdb_desc[mfp->res].w;
    h = gdb_desc[mfp->res].h;

    x = (int)(( lon       - gdb_desc[mfp->res].left ) * mfp->reso );
    y = (int)(( gdb_desc[mfp->res].top - lat        ) * mfp->reso );

    if( x < 0 || x >= w ) return((char *)NULL);
    if( y < 0 || y >= h ) return((char *)NULL);

    fp = mfp->fp1;

    if( mfp->reso == 128 && mfp->type == GDB_MAP_TEX )
      {
      ylimit = 11520;
      if( y > ylimit )
        {
        fp = mfp->fp2;
        y -= ylimit + 1;
        }
      }

    pos = ( y * w + x ) * d;

    scn_seek(fp, pos, SEEK_SET );

    if( scn_read( buf, d, fp ) != d ) return( (char *)NULL );

    if( d == 2 && SWAP_IS == SWAP_LSBF )
      {
      short *sp = (short *)buf;
      swap_2(sp[0]);
      }

    return(buf);
    }

 extern char * gdb_mapgetv128( caddr_t imfp, int n, float *ll, char *buf );

 extern char *
 gdb_mapgetv( caddr_t imfp, int n, float *ll, char *buf )
    {
    int    i;
    int    w,h,d;
    int    x,y;
    int64_t fp;
    Mapfp *mfp = (Mapfp *)imfp;
    int64_t   *pos;

    unsigned char  *pbuf;

/*
 *  special case texture a resolution 128 as a split file
 */
     if( mfp->reso == 128 && mfp->type == GDB_MAP_TEX )
      {
      return(gdb_mapgetv128(imfp, n, ll, buf) );
      }

/*
 *  check validity of file pointer struct
 */
    if( mfp        == NULL ) return((char *)NULL);
    if( mfp->magic != 1234 ) return((char *)NULL);

    fp = mfp->fp1;
    d  = mfp->d;
    w  = gdb_desc[mfp->res].w;
    h  = gdb_desc[mfp->res].h;
 
    pbuf = (unsigned char *) buf;

    pos  = (int64_t *) malloc ( n * sizeof(int64_t));

    for( i=0; i<n; ++i )
       {
       x = (int)(( ll[2*i+1]       - gdb_desc[mfp->res].left ) * mfp->reso );
       y = (int)(( gdb_desc[mfp->res].top - ll[2*i]          ) * mfp->reso );

       if( x < 0 || x >= w ){ free(pos); return((char *)NULL); }
       if( y < 0 || y >= h ){ free(pos); return((char *)NULL); }

       pos[i] = ( y * w + x ) * d;
       }

    scn_skrd(fp, d, n, pos, buf );

    if( d == 2 && SWAP_IS == SWAP_LSBF )
      {
      short *sp = (short *)pbuf;
      for( i=0; i<n; ++i ) swap_2(sp[i]);
      }

    free(pos);

    return(buf);
    }

 extern char *
 gdb_mapgetv128( caddr_t imfp, int n, float *ll, char *buf )
    {
    int    i,l;
    int    w,h,d;
    int    x,y;
    Mapfp *mfp = (Mapfp *)imfp;
    int64_t   *pos;
    int64_t   *fp;

    unsigned char  *pbuf;

    static int ylimit = 11520;

    if( mfp        == NULL ) return((char *)NULL);
    if( mfp->magic != 1234 ) return((char *)NULL);

    d = mfp->d;
    w = gdb_desc[mfp->res].w;
    h = gdb_desc[mfp->res].h;
 
    pbuf = (unsigned char *) buf;

    pos  = (int64_t *) malloc ( n * sizeof(int64_t));
    fp   = (int64_t *) malloc ( n * sizeof(int64_t));

    for( i=0; i<n; ++i )
       {
       x = (int)(( ll[2*i+1]       - gdb_desc[mfp->res].left ) * mfp->reso );
       y = (int)(( gdb_desc[mfp->res].top - ll[2*i]          ) * mfp->reso );

       if( x < 0 || x >= w ){ free(pos); return((char *)NULL); }
       if( y < 0 || y >= h ){ free(pos); return((char *)NULL); }

       if( y <= ylimit )
         fp[i] = mfp->fp1;
       else
         {
         fp[i] = mfp->fp2;
         y -= ylimit + 1;
         }

       pos[i] = ( y * w + x ) * d;
       }

    l = 0;
    for( i=0; i<n; ++i )
       {
       if( fp[i] == fp[l] ) continue;
       scn_skrd(fp[l], d, i-l, &pos[l], (char *)&pbuf[l*d] );
       l = i;
       }

    scn_skrd(fp[l], d, i-l, &pos[l], (char *)&pbuf[l*d] );

    free(fp);
    free(pos);

    return(buf);
    }

 extern caddr_t
 gdb_mapopen ( int reso, int type, int *d )
    {
    char   path[256];
    char   name[25];
    int    i;
    int    res;
    int64_t    fp1;
    int64_t    fp2;
    Mapfp *mfp;

/*
 *  initialization
 */
    *d   = 0;

/*
 *  check reso
 */
    gdb_chkreso(res,reso);
    if( res == -1 )
      {
      printf(" Error gdb_mapopen : reso  out of range\n");
      return(NULL);
      }

/*
 *  check type
 */
    switch( type )
      {
      case GDB_MAP_BAT : strcpy(name,"BAT"); *d = 2; break;
      case GDB_MAP_DEM : strcpy(name,"DEM"); *d = 2; break;
      case GDB_MAP_MSK : strcpy(name,"MSK"); *d = 1; break;
      case GDB_MAP_TER : strcpy(name,"TER"); *d = 1; break;
      case GDB_MAP_TEX : strcpy(name,"TEX"); *d = 3; break;
      default          : printf(" Error gdb_mapopen : type not supported\n");
                         return(NULL);
      }

/*
 *  building the path
 */
    strcpy(path,gdb_dbase);
    strcat(path,"/");
    strcat(path,gdb_dreso[res]);
    strcat(path,"/");
    strcat(path,name);

/*
 *  open the map
 */
    fp1 = scn_open(path,"r",&i);
    if( fp1 == 0 ) return(NULL);

/*
 *  open second file if needed
 */
    fp2 = 0;
    if( reso == 128 && type == GDB_MAP_TEX )
      {
      strcat(path,"2");
      fp2 = scn_open(path,"r",&i);
      if( fp2 == 0 ){ scn_close(fp1); return(NULL); }
      }

/*
 *  allocate and return struct 
 */
    mfp = (Mapfp *) malloc ( sizeof(Mapfp) );
    if( mfp == NULL )
      {
      scn_close(fp1);
      scn_close(fp2);
      return(NULL);
      }
    mfp->magic = 1234;
    mfp->fp1   = fp1;
    mfp->fp2   = fp2;
    mfp->reso  = reso;
    mfp->type  = type;
    mfp->res   = res;
    mfp->d     = *d;
    
    return((caddr_t)mfp);
    }


/*
 *
 *  module    :  GDB_POINT
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
 *               int       reso;
 *               int       type;
 *               GDB_POINT module;
 *               gdb_point( reso, type, module );
 *
 *  object    :  READ IN A LIMITED (GDB_LIMIT) SET OF POINT
 *
 */

 extern void
 gdb_point( int reso, int type, GDB_POINT module )
    {
    char  name[25];
    char  path[256];
    char *ptr;
    int   ilat,ilon;
    int   lat, lon;
    int   lat0,lat1;
    int   lon0,lon1;
    int   i,siz;
    int   res;

    float plat,plon;
    Ll   *ll;

/*
 *  points are only available at resolution 128
 */
    if( reso != 128 ) return;

/*
 *  check reso
 */
    gdb_chkreso(res,reso);
    if( res == -1 )
      {
      printf(" Error gdb_point : reso  out of range\n");
      return;
      }

/*
 *  check type
 */
    switch( type )
      {
      case GDB_PNT_ISLND : strcpy(name,"isp_00"); break;
      case GDB_PNT_LAKE  : strcpy(name,"dnp_00"); break;
      case GDB_PNT_SEA   : strcpy(name,"ofp_00"); break;
      default            : printf(" Error gdb_point : type not supported\n");
                           return;
      }

/*
 *  building the path
 */
    strcpy(path,gdb_dbase);
    strcat(path,"/");
    strcat(path,gdb_dreso[res]);
    strcat(path,"/");
    ptr = &path[strlen(path)];

/*
 *  check tiles
 */
    gdb_chktiles(&gdb_desc[res],&lat0,&lat1,&lon0,&lon1);

/*
 *  loop on all the tiles
 */
    ilat = gdb_desc[res].t;
    ilon = gdb_desc[res].t;
    for( lon=lon0; lon<=lon1; lon+=ilon )
       {
       for( lat=lat0; lat<=lat1; lat+=ilat )
          {
          sprintf(ptr,"%.3d/%.2d/%s",lon,lat,name);
          if( rgis_reads(path,(char **)&ll,&siz) != 0 ) continue;

          for( i=0; i<ll->n; ++i )
             {
             plat = ll->ll[2*i];
             plon = ll->ll[2*i+1];
             if( plat >= usr_top    ||
                 plat <  usr_bottom ||
                 plon >= usr_right  ||
                 plon <  usr_left    ) continue;
             (*module)( type, plat, plon );
             }
          rgis_free(ll);
          }
       }
    }


/*
 *
 *  module    :  GDB_RMN
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  CONVERT RESOLUTION AND LIMIT TO RMN GRID
 */

 extern void
 gdb_rmn  ( int reso, char *typ,  int *ni,  int *nj,
                                  int *ig1,   int *ig2, int *ig3, int *ig4,
                                  char *vtyp, float **xdef, float **ydef )
    {
    float fx,fy;
    int   i,res;
    int   x,y;
    int   w,h;

/*
 *  initialization
 */
     typ[0]  = '\0';
    *ni      = 0;
    *nj      = 0;
    *ig1     = 0;
    *ig1     = 0;
    *ig1     = 0;
    *ig1     = 0;
     vtyp[0] = '\0';
    *xdef    = (float *)NULL;
    *ydef    = (float *)NULL;

/*
 *  check reso
 */
    gdb_chkreso(res,reso);
    if( res == -1 )
      {
      printf(" Error gdb_rmn : reso  out of range\n");
      return;
      }

/*
 *  check map
 */
    if( gdb_chkmap(&gdb_desc[res],reso,&x,&y,&w,&h) < 0 )
      {
      printf(" Error gdb_rmn : limit out of dbase range\n");
      return;
      }

/*
 *  allocate space for xdef ydef
 */
    *xdef = (float *) malloc ( w * sizeof(float) );
    if( *xdef == NULL ) return;
     
    *ydef = (float *) malloc ( h * sizeof(float) );
    if( *ydef == NULL ) { free(*xdef); return; }

/*
 *  sets the results
 */
    typ[0]  =  'L';
   *ni      =    w;
   *nj      =    h;
   *ig1     =  100;
   *ig2     =  100;
   *ig3     = 9000;
   *ig4     =    0;
    vtyp[0] =  'Z';

/*
 *  reverse from gdb_chkmap
 */
    fx = ((int)(( usr_left  - gdb_desc[res].left  ) * reso )) / (float) reso
                            + gdb_desc[res].left;

    for( i=0; i<w; ++i ) (*xdef)[i] = fx + ((float)i)/reso;

    fy = -1.0 *
         ((int)(( gdb_desc[res].top - usr_bottom ) * reso )) / (float) reso
                            + gdb_desc[res].top;

    for( i=0; i<h; ++i ) (*ydef)[i] = fy + ((float)i)/reso;
    }


/*
 *
 *  module    :  GDB_TEXT
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
 *               int       reso;
 *               int       type;
 *               GDB_TEXT module;
 *               gdb_text( reso, type, module );
 *
 *  object    :  READ IN A LIMITED (GDB_LIMIT) SET OF TEXT
 *
 */

 extern void
 gdb_text( int reso, int type, GDB_TEXT module )
    {
    char  name[25];
    char  path[256];
    char *ptr;
    int   ilat,ilon;
    int   lat, lon;
    int   lat0,lat1;
    int   lon0,lon1;
    int   i,res,siz;

    float tlat,tlon;
    Llt  *llt;

/*
 *  check reso
 */
    gdb_chkreso(res,reso);
    if( res == -1 )
      {
      printf(" Error gdb_point : reso  out of range\n");
      return;
      }

/*
 *  check type
 */
    switch( type )
      {
      case GDB_TXT_AIRP  : strcpy(name,"aet_00");
                           if( reso < 16 ) return;
                           break;
      case GDB_TXT_CULT  : strcpy(name,"clt_00");
                           if( reso < 128 ) return;
                           break;
      case GDB_TXT_CITY  : strcpy(name,"ppt_00");
                           if( reso < 16 ) return;
                           break;
      case GDB_TXT_DRAIN : strcpy(name,"dnt_00");
                           if( reso < 16 ) return;
                           break;
      case GDB_TXT_ISLND : strcpy(name,"ist_00");
                           if( reso < 128 ) return;
                           break;
      case GDB_TXT_POLIT : strcpy(name,"pot_00");
                           if( reso < 16 ) return;
                           break;
      case GDB_TXT_RAIL  : strcpy(name,"rrt_00");
                           if( reso < 128 ) return;
                           break;
      case GDB_TXT_ROAD  : strcpy(name,"rdt_00");
                           if( reso < 128 ) return;
                           break;
      default            : printf(" Error gdb_text : type not supported\n");
                           return;
      }

/*
 *  building the path
 */
    strcpy(path,gdb_dbase);
    strcat(path,"/");
    strcat(path,gdb_dreso[res]);
    strcat(path,"/");
    ptr = &path[strlen(path)];

/*
 *  check tiles
 */
    gdb_chktiles(&gdb_desc[res],&lat0,&lat1,&lon0,&lon1);

/*
 *  loop on all the tiles
 */
    ilat = gdb_desc[res].t;
    ilon = gdb_desc[res].t;
    for( lon=lon0; lon<=lon1; lon+=ilon )
       {
       for( lat=lat0; lat<=lat1; lat+=ilat )
          {
          sprintf(ptr,"%.3d/%.2d/%s",lon,lat,name);
          if( rgis_reads(path,(char **)&llt,&siz) != 0 ) continue;

          for( i=0; i<llt->n; ++i )
             {
             tlat = llt->ll[2*i];
             tlon = llt->ll[2*i+1];
             if( tlat >= usr_top    ||
                 tlat <  usr_bottom ||
                 tlon >= usr_right  ||
                 tlon <  usr_left    ) continue;
             (*module)( type, tlat, tlon, llt->t[i] );
             }
          rgis_free(llt);
          }
       }
    }
