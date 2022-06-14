/*
 *
 * file     : PLN.C
 *
 * author   : Michel Grenier
 *
 * revision : V0.0
 *
 * status   : DEVELOPMENT
 *
 * language : C
 *
 * object   : THIS FILE CUTS THE EDGES OF A POLYLINE
 *            INSIDE A BOX
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "segment.h"
#include "pln.h"

#define   EPSILON     0.0005 
#define   EPS_EQ(a,b) fabs((double)(a-b)) <= EPSILON

#define   PX          0   
#define   PY          1   

#define   P_EQ_L      0  /*  P_EQ_L << 2 = 0 */
#define   P_LT_L      1  /*  P_LT_L << 2 = 4 */
#define   P_GT_L      2  /*  P_GT_L << 2 = 8 */

#define   PE_TO_PE    0  /*  P_EQ_L << 2 + P_EQ_L = 0 + 0 =  0 */
#define   PE_TO_PL    1  /*  P_EQ_L << 2 + P_LT_L = 0 + 1 =  1 */
#define   PE_TO_PG    2  /*  P_EQ_L << 2 + P_GT_L = 0 + 2 =  2 */
#define   PL_TO_PE    4  /*  P_LT_L << 2 + P_EQ_L = 4 + 0 =  4 */
#define   PL_TO_PL    5  /*  P_LT_L << 2 + P_LT_L = 4 + 1 =  5 */
#define   PL_TO_PG    6  /*  P_LT_L << 2 + P_GT_L = 4 + 2 =  6 */
#define   PG_TO_PE    8  /*  P_GT_L << 2 + P_EQ_L = 8 + 0 =  8 */
#define   PG_TO_PL    9  /*  P_GT_L << 2 + P_LT_L = 8 + 1 =  9 */
#define   PG_TO_PG   10  /*  P_GT_L << 2 + P_GT_L = 8 + 2 = 10 */

/*
 *  add point
 */

#define   ADD_PNT(x,y)   p2[0] = x; \
                         p2[1] = y; \
                         p2   += 2; \
                         ns2  += 1;
/*
 *  classify point
 */

#define   CLS_PNT(p)     pnttyp = 0;                     \
                         if( p < dlimit ) pnttyp = 1;    \
                         if( p > dlimit ) pnttyp = 2;    \
                         if( EPS_EQ(p,dlimit) )          \
                           {                             \
                           p = dlimit;                   \
                           pnttyp = 0;                   \
                           }

/*
 *  find max min than send out segment
 */

#define   MXMN_OUT()  {  minx = maxx = pln2[0];                           \
                         miny = maxy = pln2[1];                           \
                         for( j=1; j<ns2; ++j )                           \
                            {                                             \
                            if( maxx < pln2[2*j]   ) maxx  = pln2[2*j];   \
                            if( minx > pln2[2*j]   ) minx  = pln2[2*j];   \
                            if( maxy < pln2[2*j+1] ) maxy  = pln2[2*j+1]; \
                            if( miny > pln2[2*j+1] ) miny  = pln2[2*j+1]; \
                            }                                             \
                         cutout( ns2, minx, miny, maxx, maxy, pln2 );     \
                         p2  = pln2;                                      \
                         ns2 = 0;                                         }


/*
 *  cuts the polygon where x is greater than limit
 */

 extern void
 pln_cutxGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT cutout )
    {
    float    x,  y;

    register int            i,j;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       ns2  = 0;
    float    *pln2 = NULL;

/*
 *  completly inside dlimit
 */
    if( maxx-EPSILON <= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pln ); 
      return;
      }

/*
 *  outside dlimit
 */
    if( minx+EPSILON >= dlimit ) return;

/*
 *  enough memory to work
 */
    ns2  = 0;
    pln2 = (float *) realloc ( pln2, (n+4) * 2 * sizeof(float) );

/*
 *  initialize
 */
    p1     = pln;
    p2     = pln2;

/*
 *  classify segment
 */
    CLS_PNT(p1[0]);
    segtyp = pnttyp;

/*
 *  if first point inside add it
 */
    if( pnttyp != 2 )
      {
      ADD_PNT(p1[0],p1[1]);
      }

/*
 *  loop on all the points
 */
    p1 += 2;
    for( i=1; i<n; ++i, p1+=2 )
       {
/*
 *  classify segment
 */
       CLS_PNT(p1[0]);
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PG_TO_PE : 
			     ADD_PNT(p1[0],p1[1]);
                             break;

             case PG_TO_PL :
                             x = dlimit; 
                             if( EPS_EQ(p1[-1],p1[1]) )
                                y = p1[1];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );
			     ADD_PNT(x,y);
			     ADD_PNT(p1[0],p1[1]);
                             break;

/* continuing */

             case PE_TO_PE : 
             case PE_TO_PL : 
             case PL_TO_PE : 
             case PL_TO_PL : 
			     ADD_PNT(p1[0],p1[1]);
                             break;

/* ending conditions */

             case PE_TO_PG : 
			     if( ns2 >= 2 ) MXMN_OUT();
			     ns2 = 0;
                             break;

             case PL_TO_PG : 
                             x = dlimit; 
                             if( EPS_EQ(p1[-1],p1[1]) )
                                y = p1[1];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );

			     ADD_PNT(x,y);
			     MXMN_OUT();
                             break;

             }
       }

/*
 *  if a segment was left
 */
    if( ns2 >= 2 ) MXMN_OUT();

    free(pln2);
    }

/*
 *  cuts the polygon where x is lower than limit
 */

 extern void
 pln_cutxLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT cutout )
    {
    float    x,  y;

    register int            i,j;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       ns2  = 0;
    float    *pln2 = NULL;

/*
 *  completly inside dlimit
 */
    if( minx+EPSILON >= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pln );
      return;
      }

/*
 *  outside dlimit
 */
    if( maxx-EPSILON <= dlimit ) return;

/*
 *  enough memory to work
 */
    ns2  = 0;
    pln2 = (float *) realloc ( pln2, (n+4) * 2 * sizeof(float) );

/*
 *  initialize
 */
    p1     = pln;
    p2     = pln2;

/*
 *  classify segment
 */
    CLS_PNT(p1[0]);
    segtyp = pnttyp;

/*
 *  if first point inside add it
 */
    if( pnttyp != 1 )
      {
      ADD_PNT(p1[0],p1[1]);
      }

/*
 *  loop on all the points
 */
    p1 += 2;
    for( i=1; i<n; ++i, p1+=2 )
       {
/*
 *  classify segment
 */
       CLS_PNT(p1[0]);
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PL_TO_PE : 
			     ADD_PNT(p1[0],p1[1]);
                             break;

             case PL_TO_PG :
                             x = dlimit; 
                             if( EPS_EQ(p1[-1],p1[1]) )
                                y = p1[1];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );
                             ADD_PNT(x,y);
                             ADD_PNT(p1[0],p1[1]);
                             break;

/* continuing */

             case PE_TO_PE : 
             case PE_TO_PG : 
             case PG_TO_PE : 
             case PG_TO_PG : 
                             ADD_PNT(p1[0],p1[1]);
                             break;

/* ending conditions */

             case PE_TO_PL : 
			     if( ns2 >= 2 ) MXMN_OUT();
			     ns2 = 0;
                             break;

             case PG_TO_PL : 
                             x = dlimit; 
                             if( EPS_EQ(p1[-1],p1[1]) )
                                y = p1[1];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );
                             ADD_PNT(x,y);
			     MXMN_OUT();
                             break;

             }
       }

/*
 *  if a segment was left
 */
    if( ns2 >= 2 ) MXMN_OUT();

    free(pln2);
    }

/*
 *  cuts the polygon where y is greater than limit
 */

 extern void
 pln_cutyGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT cutout )
    {
    float    x,  y;

    register int            i,j;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       ns2  = 0;
    float    *pln2 = NULL;

/*
 *  completly inside dlimit
 */
    if( maxy-EPSILON <= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pln );
      return;
      }

/*
 *  outside dlimit
 */
    if( miny+EPSILON >= dlimit ) return;

/*
 *  enough memory to work
 */
    ns2  = 0;
    pln2 = (float *) realloc ( pln2, (n+4) * 2 * sizeof(float) );

/*
 *  initialize
 */
    p1     = pln;
    p2     = pln2;

/*
 *  classify segment
 */
    CLS_PNT(p1[1]);
    segtyp = pnttyp;

/*
 *  if first point inside add it
 */
    if( pnttyp != 2 )
      {
      ADD_PNT(p1[0],p1[1]);
      }

/*
 *  loop on all the points
 */
    for( i=1; i<n; ++i )
       {
       p1 += 2;
/*
 *  classify segment
 */
       CLS_PNT(p1[1]);
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PG_TO_PE : 
			     ADD_PNT(p1[0],p1[1]);
                             break;

             case PG_TO_PL :
                             if( EPS_EQ(p1[-2],p1[0]) )
                                x = p1[0];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             y = dlimit; 
                             ADD_PNT(x,y);
                             ADD_PNT(p1[0],p1[1]);
                             break;

/* continuing */

             case PE_TO_PE : 
             case PE_TO_PL : 
             case PL_TO_PE : 
             case PL_TO_PL : 
			     ADD_PNT(p1[0],p1[1]);
                             break;

/* ending conditions */

             case PE_TO_PG : 
			     if( ns2 >= 2 ) MXMN_OUT();
			     ns2 = 0;
                             break;

             case PL_TO_PG : 
                             if( EPS_EQ(p1[-2],p1[0]) )
                                x = p1[0];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             y = dlimit; 

                             ADD_PNT(x,y);
			     MXMN_OUT();
                             break;

             }
       }

/*
 *  if a segment was left
 */
    if( ns2 >= 2 ) MXMN_OUT();

    free(pln2);
    }

/*
 *  cuts the polygon where y is lower than limit
 */

 extern void
 pln_cutyLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT cutout )
    {
    float    x,  y;

    register int            i,j;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       ns2  = 0;
    float    *pln2 = NULL;

/*
 *  completly inside dlimit
 */
    if( miny+EPSILON >= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pln ); 
      return;
      }

/*
 *  outside dlimit
 */
    if( maxy-EPSILON <= dlimit ) return;

/*
 *  enough memory to work
 */
    ns2  = 0;
    pln2 = (float *) realloc ( pln2, (n+4) * 2 * sizeof(float) );

/*
 *  initialize
 */
    p1     = pln;
    p2     = pln2;

/*
 *  classify segment
 */
    CLS_PNT(p1[1]);
    segtyp = pnttyp;

/*
 *  if first point inside add it
 */
    if( pnttyp != 1 )
      {
      ADD_PNT(p1[0],p1[1]);
      }

/*
 *  loop on all the points
 */
    p1 += 2;
    for( i=1; i<n; ++i, p1+=2 )
       {
/*
 *  classify segment
 */
       CLS_PNT(p1[1]);
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PL_TO_PE : 
			     ADD_PNT(p1[0],p1[1]);
                             break;

             case PL_TO_PG :
                             if( EPS_EQ(p1[-2],p1[0]) )
                                x = p1[0];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             y = dlimit; 
                             ADD_PNT(x,y);
                             ADD_PNT(p1[0],p1[1]);
                             break;

/* continuing */

             case PE_TO_PE : 
             case PE_TO_PG : 
             case PG_TO_PE : 
             case PG_TO_PG : 
			     ADD_PNT(p1[0],p1[1]);
                             break;

/* ending conditions */

             case PE_TO_PL : 
			     if( ns2 >= 2 ) MXMN_OUT();
			     ns2 = 0;
                             break;

             case PG_TO_PL : 
                             if( EPS_EQ(p1[-2],p1[0]) )
                                x = p1[0];
			     else
                                (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             y = dlimit; 

                             ADD_PNT(x,y);
			     MXMN_OUT();
                             break;
             }
       }

/*
 *  if a segment was left
 */
    if( ns2 >= 2 ) MXMN_OUT();

    free(pln2);
    }

 static float  cminx, cminy, cmaxx, cmaxy;
 static PLNOUT pln_done;

/*
 * debug tool
 */
  
 static void
 pln_write( int n, float minx,float miny,float maxx,float maxy, float *pln )
    {
    int i;
    float lx, ly;
    printf(" ---------------- \n");
    printf(" n = %d \n",n);
    printf(" min x y -> %f %f  max x y -> %f %f \n",minx, miny, maxx, maxy);
    for( i=0; i<n; ++i )
       printf("      %f %f \n",pln[2*i],pln[2*i+1]);
    pln_done(n,minx,miny,maxx,maxy,pln);
    }


 static void
 pln_cutBottom( int n, float minx,float miny,float maxx,float maxy, float *pln )
    {
    pln_cutyLTl(n,minx,miny,maxx,maxy,pln,cminy,pln_done);
    }

 static void
 pln_cutLeft( int n, float minx,float miny,float maxx,float maxy, float *pln )
    {
    pln_cutxLTl(n,minx,miny,maxx,maxy,pln,cminx,pln_cutBottom);
    }

 static void
 pln_cutRight( int n, float minx,float miny,float maxx,float maxy, float *pln )
    {
    pln_cutxGTl(n,minx,miny,maxx,maxy,pln,cmaxx,pln_cutLeft);
    }

 static void
 pln_cutTop( int n, float minx,float miny,float maxx,float maxy, float *pln )
    {
    pln_cutyGTl(n,minx,miny,maxx,maxy,pln,cmaxy,pln_cutRight);
    }

 extern void
 pln_cutBox( int n,
             float minx,float miny,float maxx,float maxy,
             float *pln,
             float iminx, float iminy, float imaxx, float imaxy,
             PLNOUT done )
    {

/* debug
    {
    int i;
    float lx, ly;
    printf(" ********************************************* \n");
    printf(" BOX =  %f %f  %f %f \n",iminx, iminy, imaxx, imaxy);
    printf(" ********************************************* \n");
    printf(" n = %d \n",n);
    printf(" min x y -> %f %f  max x y -> %f %f \n",minx, miny, maxx, maxy);
    for( i=0; i<n; ++i )
       printf("      %f %f \n",pln[2*i],pln[2*i+1]);
    }
    */

    if( maxx+EPSILON <= iminx ) return;
    if( maxy+EPSILON <= iminy ) return;
    if( minx-EPSILON >= imaxx ) return;
    if( miny-EPSILON >= imaxy ) return;

    cminx    = iminx;
    cminy    = iminy;
    cmaxx    = imaxx;
    cmaxy    = imaxy;
    pln_done = done;

    pln_cutTop( n, minx,miny,maxx,maxy, pln );
    }
