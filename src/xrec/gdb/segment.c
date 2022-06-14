/*
 *
 *  file      :  SEGMENT.C
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
 *               MANAGING SEGMENTS
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "point.h"
#include "segment.h"


/*
 *
 *  module    :  SEG_CLOSE
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
 *               int     n;
 *               float  *v;
 *               int    *nc;
 *               float **vc;
 *               seg_close( n, v, nc, vc );
 *
 *  object    :  THIS MODULE CLOSES A VECTOR IF NOT ALREADY CLOSES
 *
 */

 extern void
 seg_close ( int n, float *v, int *nc, float **vc )

    {
    int x, y;

/*
 *  initialisation
 */
    *nc = 0;
    *vc = NULL;

/*
 *  test inputs
 */
    if( n == 0 || v == NULL ) return;

/*
 *  compute position of last coordinates
 */
    x = 2 * n - 2;
    y = x + 1;

/*
 *  check if already and "realy" closed
 */
    if( pnt_EQ(v[0],v[1],v[x],v[y]) ) return;

/*
 *  createw the closed vector and copy the other
 */
    *nc = n + 1;
    *vc = (float *) malloc ( *nc * 2 * sizeof(float) );
    memcpy( *vc, v, 2*n*sizeof(float) );

/*
 *  copy the last coordinates
 */
    x = 2 * *nc - 2;
    y = x + 1;

    (*vc)[x] = v[0];
    (*vc)[y] = v[1];
    }


/*
 *
 *  module    :  SEG_CLOSEIN
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
 *               float   n1;
 *               float  *v1;
 *               int     n2;
 *               float  *v2;
 *               int    *ng;
 *               float **vg;
 *               seg_closein( n1, v1, n2, v2, ng, vg );
 *
 *  object    :  THIS MODULE FINDS IS v1 IS THE VECTOR
 *               CAN BE CLOSED BY A PART OF v2. IT RETURNS
 *               THE RESULTING CLOSED VECTOR IF POSSIBLE.
 *
 */

 extern int
 seg_closein ( int n1, float *v1, int n2, float *v2, int *ng, float **vg )

    {
    int closed;
    int k;
    int x;
    int y;
    int ph;
    int pt;
    int faux;

    int    nx;
    float *vx;

    faux = (0==1);
/*
 *  initialisation
 */
    *ng = 0;
    *vg = NULL;

/*
 *  test inputs
 */
    if( n1 == 0 || v1 == NULL ) return(faux);
    if( n2 == 0 || v2 == NULL ) return(faux);

/*
 *  affectation
 */
    x = 2 * n1 - 2;
    y = x + 1;

/*
 *  if segment v1 is already closed than nothing to do
 */
    if( seg_isclose(n1,v1) ) return(faux);

/*
 *  if segment v2 is closed 
 */
    nx = 0;
    closed = seg_isclose(n2,v2);
    if( closed ) seg_close( n2,v2, &nx, &vx );
    if( nx == 0 )
      {
      nx = n2;
      vx = v2;
      }

/*
 *  try to find the head and tail of v1 in v2
 */
    ph = seg_find( v1[0],v1[1], nx,vx );
    pt = seg_find( v1[x],v1[y], nx,vx );
    if( ph < 0 || pt < 0 )
      {
      if( vx != v2 ) free(vx);
      return(faux);
      }

/*
 *  simple connection
 */
    k = abs(ph-pt)+1;
    if( !closed || k <= nx-k+1 )
      {
      if( ph < pt )
        seg_join ( n1, v1, k, &vx[2*ph], SEG_H2H, ng, vg );
      else
        seg_join ( n1, v1, k, &vx[2*pt], SEG_H2T, ng, vg );
      }

/*
 *  v2 is closed and shortest join is by the extrimities
 */
    else
      {
      int    nt;
      float *vt;
      if( ph < pt )
        {
        seg_join ( n1, v1, nx-pt, &vx[2*pt], SEG_T2H, &nt, &vt );
        seg_join ( nt, vt, ph+1, vx, SEG_H2T, ng, vg );
        }
      else
        {
        seg_join ( n1, v1, pt+1, vx, SEG_T2T, &nt, &vt );
        seg_join ( nt, vt, nx-ph, &vx[2*ph], SEG_H2H, ng, vg );
        }
      free(vt);
      }

    if( vx != v2 ) free(vx);

    return(!faux);
    }


/*
 *
 *  module    :  SEG_CONNBOX
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
 *               float   n1;
 *               float  *v1;
 *               int     n2;
 *               float  *v2;
 *               int     type;
 *               seg_connbox( n1, v1, n2, v2, type );
 *
 *  object    :  THIS MODULE FINDS IF v1 WHEN CONNECTED TO v2
 *               MAKING A BOXED SEGMENT.
 *
 */

 extern int
 seg_connbox ( int n1, float *v1, int n2, float *v2, int type )

    {
    int faux;
    int x2,y2;
    int i, x,y;
    float v[4];

    faux = (0 == 1);


/*
 *  check the input
 */
    if( n1 <= 0    || n2 <= 0    ) return(faux);
    if( v1 == NULL || v2 == NULL ) return(faux);

/*
 *  if connection makes a closed segment than it is no a boxed segment
 */
    if( seg_connclose(n1,v1,n2,v2,type) ) return(faux);

/*
 *  assignations
 */
    x  = 2  * n1 - 2;
    y  = x  + 1;
    x2 = 2  * n2 - 2;
    y2 = x2 + 1;

/*
 *  creates the resulting segment
 */
    i = 0;
    if( type==SEG_T2H ){i++; v[0]=v1[0]; v[1]=v1[1]; v[2]=v2[x2]; v[3]=v2[y2];}
    if( type==SEG_T2T ){i++; v[0]=v1[0]; v[1]=v1[1]; v[2]=v2[0 ]; v[3]=v2[1 ];}
    if( type==SEG_H2T ){i++; v[0]=v1[x]; v[1]=v1[y]; v[2]=v2[0 ]; v[3]=v2[1 ];}
    if( type==SEG_H2H ){i++; v[0]=v1[x]; v[1]=v1[y]; v[2]=v2[x2]; v[3]=v2[y2];}

/*
 *  check for any error
 */
    if( i != 1 ) return(faux);

/*
 *  return the result
 */
    return( seg_isbox(2,v) );
    }


/*
 *
 *  module    :  SEG_CONNCLOSE
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
 *               float   n1;
 *               float  *v1;
 *               int     n2;
 *               float  *v2;
 *               int     type;
 *               seg_connclose( n1, v1, n2, v2, type );
 *
 *  object    :  THIS MODULE FINDS IF v1 WHEN CONNECTED TO v2
 *               MAKING A CLOSED SEGMENT.
 *
 */

 extern int
 seg_connclose ( int n1, float *v1, int n2, float *v2, int type )

    {
    int faux;

    faux = (0 == 1);

/*
 *  check the input
 */
    if( n1 <= 0    || n2 <= 0    ) return(faux);
    if( v1 == NULL || v2 == NULL ) return(faux);

/*
 *  if type of connection is for head and tail the two closes
 */
    return( ( (type&SEG_T2H) && (type&SEG_H2T) ) ||
            ( (type&SEG_T2T) && (type&SEG_H2H) )  );
    }


/*
 *
 *  module    :  SEG_CONNHOW
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
 *               float   n1;
 *               float  *v1;
 *               int     n2;
 *               float  *v2;
 *               int    *type;
 *               float  *d;
 *               seg_connhow( n1, v1, n2, v2, type, d );
 *
 *  object    :  THIS MODULE FINDS HOW v1 CONNECTS TO v2
 *               SELECTING THE CONNECTION FOR CLOSEST POINTS
 *
 */

 extern int
 seg_connhow ( int n1, float *v1, int n2, float *v2, int *type, float *d )
    {
    int faux;
    int x1,y1;
    int x2,y2;

    float f;

    faux = (0 == 1);

/*
 *  check the input
 */
    if( n1 <= 0    || n2 <= 0    ) return(faux);
    if( v1 == NULL || v2 == NULL ) return(faux);

/*
 *  assignations
 */
    x1 = 2  * n1 - 2;
    y1 = x1 + 1;
    x2 = 2  * n2 - 2;
    y2 = x2 + 1;
   *d  = 4.0e7;
   *type = 0;

/*
 *  compute distance between segments extremities
 *  the smallest distance is kept and its type saved
 */
    f = pnt_distance( v1[0], v1[1], v2[0], v2[1]  );
    if( f == *d ) { *d = f; *type |= SEG_H2H; }
    if( f <  *d ) { *d = f; *type  = SEG_H2H; }
    f = pnt_distance( v1[0], v1[1], v2[x2],v2[y2] );
    if( f == *d ) { *d = f; *type |= SEG_H2T; }
    if( f <  *d ) { *d = f; *type  = SEG_H2T; }
    f = pnt_distance( v1[x1],v1[y1],v2[0], v2[1]  );
    if( f == *d ) { *d = f; *type |= SEG_T2H; }
    if( f <  *d ) { *d = f; *type  = SEG_T2H; }
    f = pnt_distance( v1[x1],v1[y1],v2[x2],v2[y2] );
    if( f == *d ) { *d = f; *type |= SEG_T2T; }
    if( f <  *d ) { *d = f; *type  = SEG_T2T; }

    return( !faux );
    }


/*
 *
 *  module    :  SEG_CROSS
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
 *               float x11, y11, x12, y12;
 *               float x21, y21, x22, y22;
 *               float *x, *y;
 *               seg_cross( x11, y11, x12, y12,
 *                          x21, y21, x22, y22, x, y );
 *
 *  object    :  THIS MODULE COMPUTES IF 2 SEGMENTS CROSS
 *               IF IT RETURN  0 IT MEANS PARALLEL
 *               IF IT RETURN -1 IT MEANS ONTO
 *               IF IT RETURN  1 IT MEANS CROSSING AND POINT IS X,Y
 *
 *  note      :  Here are the formula
 *               
 *               line eq. the 2 point form
 *
 *   Y-y1 = y2-y1  (X-x1)  => (y2-y1)(X-x1) - (x2-x1)(Y-y1) = 0
 *          -----
 *          x2-x1
 *                         => (y2-y1)X - (x2-x1)Y + (x2-x1)y1 - (y2-y1)x1 = 0
 *
 *               so in line eq. of the form ax+by+c=0 we have
 *               a =  (y2-y1)
 *               b = -(x2-x1)
 *               c = (x2-x1)y1 - (y2-y1)x1 =  -b * y1 - a * x1 = (b*y1+a*x1)*-1
 *              
 *               ax + by + c = 0
 *               dx + ey + f = 0
 *
 *               2ieme Cramer Rule
 *
 *               d = | a b |    x = | c b |  /      y = | a c |  /
 *                   | d e |        | f e | /  d        | d f | /  d
 *
 *               if d = 0    NO SOLUTION OR PARALLEL
 *              
 *
 */

 extern int
 seg_cross( float x11, float y11, float x12, float y12,
            float x21, float y21, float x22, float y22,
            float *x, float *y )

   {
   float denom;
   float numx;
   float numy;

   float a, b, c; /* eq. seg 1 ax + by + c = 0 */
   float d, e, f; /* eq. seg 2 dx + ey + f = 0 */

   a = y12 - y11;
   b = x11 - x12;
   c = (a * x11 + b * y11) * -1;
   d = y22 - y21;
   e = x21 - x22;
   f = (d * x21 + e * y21) * -1;

   denom = a * e - b * d;

   numx  = b * f - c * e;
   numy  = c * d - f * a;

   if( denom == 0.0 )
     {
     return( numx == 0.0 || numy == 0.0 ? -1 /* same */ : 0 /* parallel */ );
     }

   *x = numx / denom;
   *y = numy / denom;

   return(1);
   }


/*
 *
 *  module    :  SEG_CROSSPT
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
 *               float x11, y11, x12, y12;
 *               float x21, y21, x22, y22;
 *               float *x, *y;
 *               seg_crosspt( x11, y11, x12, y12,
 *                           x21, y21, x22, y22, x, y );
 *
 *  object    :  THIS MODULE COMPUTES THE POINT WHERE 2 SEGMENTS CROSS
 *               THIS POINT HAS TO BE ON THE 2 SEGMENTS.
 *               IF IT RETURN  0 MEANS NO CROSSING POINTS
 *               IF IT RETURN  1 IT MEANS CROSSING AND POINT IS X,Y
 *
 */

 extern int
 seg_crosspt( float x11, float y11, float x12, float y12,
              float x21, float y21, float x22, float y22,
              float *x, float *y )

   {
   int   i;

   i = seg_cross( x11, y11, x12, y12, x21, y21, x22, y22, x, y );

   switch(i)
      {
      case -1 : /* over */

                if( x11 > x21 && x11 < x22 )
                    *x = x11;
                else if( x12 > x21 && x12 < x22 )
                    *x = x12;
                else
                    return(-1);

                if( y11 > y21 && y11 < y22 )
                    *y = y11;
                else if( y12 > y21 && y12 < y22 )
                    *y = y12;
                else
                    return(-2);

                return(1);

      case  0 : /* parallel */

                return(0);

      case  1 : /* cross */

                if( x11 == x12 && *x != x11 ) return(-3);
                if( x21 == x22 && *x != x21 ) return(-4);
                if( y11 == y12 && *y != y11 ) return(-5);
                if( y21 == y22 && *y != y21 ) return(-6);

                if( x12 != x11 && 
                    fabs( (double)((*x-x11)/(x12-x11)) ) > 1.0 ) return(-7);
                if( x22 != x21 && 
                    fabs( (double)((*x-x21)/(x22-x21)) ) > 1.0 ) return(-8);
                if( y12 != y11 &&
                    fabs( (double)((*y-y11)/(y12-y11)) ) > 1.0 ) return(-9);
                if( y22 != y21 &&
                    fabs( (double)((*y-y21)/(y22-y21)) ) > 1.0 ) return(-10);

                if( x11 != x12 &&
                    fabs( (double)((*x-x12)/(x11-x12)) ) > 1.0 ) return(-11);
                if( x21 != x22 &&
                    fabs( (double)((*x-x22)/(x21-x22)) ) > 1.0 ) return(-12);
                if( y11 != y12 &&
                    fabs( (double)((*y-y12)/(y11-y12)) ) > 1.0 ) return(-13);
                if( y21 != y22 &&
                    fabs( (double)((*y-y22)/(y21-y22)) ) > 1.0 ) return(-14);

                return(1);
      }
   return(-9999);
   }


/*
 *
 *  module    :  SEG_CUT
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
 *               int     n;
 *               float  *v;
 *               int     p;
 *               int    *n1;
 *               float **v1;
 *               int    *n2;
 *               float **v2;
 *               seg_cut( n, v, p, n1, v1, n2, v2 );
 *
 *  object    :  THIS MODULE CUTS SEGMENT V IN TWO PARTS
 *               AT POSITION p. THE TWO RESULTING VECTORS 
 *               WILL CONTAIN p.
 *
 */

 extern int
 seg_cut ( int n, float *v, int p, int *n1, float **v1, int *n2, float **v2 )

    {
/*
 *  initialisation
 */
    *n1 = 0;
    *v1 = NULL;
    *n2 = 0;
    *v2 = NULL;

/*
 *  test inputs
 */
    if( n == 0 || v == NULL ) return(-1);
    if( p == 0 || p == n-1  ) return(-1);

/*
 *  left part of the segment
 */
    *n1 = p+1;
    *v1 = (float *) malloc ( *n1 * 2 * sizeof(float) );
    memcpy( *v1, v, *n1 * 2 * sizeof(float) );

/*
 *  right part of the segment
 */
    *n2 = n-p;
    *v2 = (float *) malloc ( *n2 * 2 * sizeof(float) );
    memcpy( *v2, &v[2*p], *n2 * 2 * sizeof(float) );

    return(2);
    }


/*
 *
 *  module    :  SEG_FIND
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
 *               float  x;
 *               float  y;
 *               int    n;
 *               float *v;
 *               seg_find( x, y, n, v );
 *
 *  object    :  THIS MODULE FINDS (X,Y) IN THE VECTOR
 *
 */

 extern int
 seg_find ( float x, float y, int n, float *v )

    {
    int   i,p;
    float d,f;
/*
 *  test inputs
 */
    if( n == 0 || v == NULL ) return(-1);

/*
 *  loop on all coords
 */
    p = -1;
    d = 4.0e7;
    for( i=0; i<n; ++i )
       {
       if( pnt_EQ(x,y,v[2*i],v[2*i+1]) ) return(i);
       f = pnt_distance(x,y,v[2*i],v[2*i+1]);
       if( f < d ) { d = f; p = i; }
       }

    if( p != -1 && !pnt_almostEQ(x,y,v[2*p],v[2*p+1]) ) p = -1;

    return(p);
    }


/*
 *
 *  module    :  SEG_FRMSEG
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
 *               int    n1;
 *               float *v1;
 *               int    n2;
 *               float *v2
 *               float *dh;
 *               int   *th;
 *               float *dt;
 *               int   *tt;
 *               seg_frmseg( n1, v1, n2, v2, dh, th, dt, tt );
 *
 *  object    :  THIS MODULE DEFINES SOME VARIABLES TELLING HOW
 *               CLOSE v1 IS FROM v2... AND HOW THEY CONNECT
 */

 extern void
 seg_frmseg( int    n1, float *v1, int    n2, float *v2,
             float *dh, int   *th, float *dt, int   *tt  )

    {
    int   i, k;
    int   x1,y1;
    int   x2,y2;
    float d;

   *dh = 9999.0;
   *th = 0;
   *dt = 9999.0;
   *tt = 0;

    x1 = 2 * n1 - 2;
    y1 = x1 + 1;

    x2 = 2 * n2 - 2;
    y2 = x2 + 1;

    d = pnt_distance(v1[0],v1[1],v2[0], v2[1]);
    if( d < *dh ){ *dh = d; *th = SEG_H2H; }

    d = pnt_distance(v1[0],v1[1],v2[x2],v2[y2]);
    if( d < *dh ){ *dh = d; *th = SEG_H2T; }

    d = pnt_distance(v1[x1],v1[y1],v2[0],v2[1]);
    if( d < *dt ){ *dt = d; *tt = SEG_T2H; }

    d = pnt_distance(v1[x1],v1[y1],v2[x2],v2[y2]);
    if( d < *dt ){ *dt = d; *tt = SEG_T2T; }

/*
 *  a subvect of the vector will not be considered
 */
    k = ( n1 < n2 ? n1 : n2 );
    if( *th&SEG_H2H )
      {
      for( i=0; i<k; ++i )
         if( v1[2*i] != v2[2*i] || v1[2*i+1] != v2[2*i+1] ) break;
      if( i == k ){ *dh = *dt = 9999.0; *th = *tt = 0; return; }
      }

    if( *tt&SEG_T2H )
      {
      for( i=0; i<k; ++i )
         if( v1[x1-2*i] != v2[2*i] || v1[y1-2*i] != v2[2*i+1] ) break;
      if( i == k ){ *dh = *dt = 9999.0; *th = *tt = 0; return; }
      }

    if( *th&SEG_H2T )
      {
      for( i=0; i<k; ++i )
         if( v1[2*i] != v2[x2-2*i] || v1[2*i+1] != v2[y2-2*i] ) break;
      if( i == k ){ *dh = *dt = 9999.0; *th = *tt = 0; return; }
      }

    if( *tt&SEG_T2T )
      {
      for( i=0; i<k; ++i )
         if( v1[x1-2*i] != v2[x2-2*i] || v1[y1-2*i] != v2[y2-2*i] ) break;
      if( i == k ){ *dh = *dt = 9999.0; *th = *tt = 0; return; }
      }
    }


/*
 *
 *  module    :  SEG_ISBOX
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
 *               float   n;
 *               float  *v;
 *               seg_isbox( n, v );
 *
 *  object    :  THIS MODULE CHECKS IF v IS BOXED; MEANING THAT
 *               THE HEAD AND TAIL TOUCH (OR CROSS) THE TILE BOX
 *
 */

 extern int
 seg_isbox ( int n, float *v )

    {
    int x, y ;
    int faux;

    faux = (0 == 1);

/*
 *  check the input
 */
    if( n <= 0 || v == NULL ) return(faux);

/*
 *  assignations
 */
    x  = 2 * n - 2;
    y  = x + 1;

/*
 *  check for the box
 */
    return( pnt_almostBOX(v[0],v[1]) && pnt_almostBOX(v[x],v[y]) );
    }


/*
 *
 *  module    :  SEG_ISCLOSE
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
 *               float   n;
 *               float  *v;
 *               seg_isclose( n, v );
 *
 *  object    :  THIS MODULE FINDS IF v IS CLOSED
 *
 */

 extern int
 seg_isclose ( int n, float *v )

    {
    int x, y ;
    int faux;

    faux = (0==1);

/*
 *  check the input
 */
    if( n <= 0 || v == NULL ) return(faux);

/*
 *  assignations
 */
    x  = 2  * n - 2;
    y  = x  + 1;

/*
 *  check for closing
 */
    return( pnt_almostEQ(v[0],v[1],v[x],v[y]) );
    }


/*
 *
 *  module    :  SEG_ISCONN
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
 *               float   n1;
 *               float  *v1;
 *               int     n2;
 *               float  *v2;
 *               int    *type;
 *               seg_isconn( n1, v1, n2, v2, type );
 *
 *  object    :  THIS MODULE FINDS IF v1 CAN CONNECT TO v2
 *               IF IT CAN IT RETURNS THE TYPE.
 *
 */

 extern int
 seg_isconn ( int n1, float *v1, int n2, float *v2, int *type )

    {
    int x, y ;
    int x2,y2;
    int faux;

    *type = 0;
    faux = (0 == 1);

/*
 *  check the input
 */
    if( n1 <= 0    || n2 <= 0    ) return(faux);
    if( v1 == NULL || v2 == NULL ) return(faux);

/*
 *  assignations
 */
    x  = 2  * n1 - 2;
    y  = x  + 1;
    x2 = 2  * n2 - 2;
    y2 = x2 + 1;

/*
 *  check for any connections
 */
    if( pnt_almostEQ(v1[0],v1[1], v2[0 ],v2[1 ]) ) *type |= SEG_H2H;
    if( pnt_almostEQ(v1[0],v1[1], v2[x2],v2[y2]) ) *type |= SEG_H2T;
    if( pnt_almostEQ(v1[x],v1[y], v2[0 ],v2[1 ]) ) *type |= SEG_T2H;
    if( pnt_almostEQ(v1[x],v1[y], v2[x2],v2[y2]) ) *type |= SEG_T2T;

    return( *type>0 );
    }


/*
 *
 *  module    :  SEG_ISINTO
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
 *               float   n1;
 *               float  *v1;
 *               float   n2;
 *               float  *v2;
 *               seg_isinto( n1, v1, n2, v2 );
 *
 *  object    :  THIS MODULE CHECKS IF v1 IS INTO v2 OR INVERSLY
 *
 */

 extern int
 seg_isinto ( int n1, float *v1, int n2, float *v2 )

    {
    int i,j,k;
    int faux;

    int    ni;
    int    nl;
    float *vl;
    float *vi;

    faux = (0 == 1);

/*
 *  check the input
 */
    if( n1 <= 0 || v1 == NULL ) return(faux);
    if( n2 <= 0 || v2 == NULL ) return(faux);

/*
 *  check for the smallest vector into the other one
 */
    ni = ( n1 <= n2 ? n1 : n2 );
    nl = ( n1 <= n2 ? n2 : n1 );
    vi = ( n1 <= n2 ? v1 : v2 );
    vl = ( n1 <= n2 ? v2 : v1 );

/*
 *  find head of smallest in the other one
 */
    for( j=0; j<nl; ++j )
       if( vl[2*j] == vi[0] && vl[2*j+1] == vi[1] ) break;
    if( j == nl ) return(faux);

/*
 *  look for the direction to go
 */
    k = 0;
    if( j<nl-1 && vl[2*j+2] == vi[2] && vl[2*j+3] == vi[3] ) k =  1;
    if( j>0    && vl[2*j-2] == vi[2] && vl[2*j-1] == vi[3] ) k = -1;
    if( k == 0 ) return(faux);

/*
 *  check if all points of vi is inside vl
 */
    for( i=0; i<ni; ++i )
       {
       if( vl[2*j] != vi[2*i] || vl[2*j+1] != vi[2*i+1] ) return(faux);
       j += k;
       if( j < 0   ) j = nl - 1;
       if( j == nl ) j = 0;
       }

/*
 *  at this point they were all inside
 */
    return( !faux );
    }


/*
 *
 *  module    :  SEG_ISSUBCON
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
 *               float   n1;
 *               float  *v1;
 *               int     n2;
 *               float  *v2;
 *               int    *type;
 *               seg_issubcon( n1, v1, n2, v2, type );
 *
 *  object    :  THIS MODULE FINDS IF v1 CAN SUBCONNECT TO v2
 *               IF IT CAN IT RETURNS THE TYPE.
 *
 */

 extern int
 seg_issubcon ( int n1, float *v1, int n2, float *v2, int *type )

    {
    int h, t, typ;
    int x, y;
    int faux;

    *type = 0;
    faux = (0 == 1);

/*
 *  check the input
 */
    if( n1 <= 0    || n2 <= 0    ) return(faux);
    if( v1 == NULL || v2 == NULL ) return(faux);

/*
 *  assignations
 */
    x  = 2  * n1 - 2;
    y  = x  + 1;
    seg_type(n1,v1,&typ);

/*
 *  check for any connections
 */
    h = ( typ != SEG_BOX_CONNECT ? seg_find(v1[0],v1[1],n2,v2) : -1 );
    t = ( typ != SEG_CONNECT_BOX ? seg_find(v1[x],v1[y],n2,v2) : -1 );

/*
 *  building the type according to the finding
 */
    if( h == t ) return(faux);
    seg_type(n2,v2,&typ);
    if( h == -1 )
      *type = ( typ == SEG_BOX_CONNECT ? SEG_T2T :
              ( typ == SEG_CONNECT_BOX ? SEG_T2H : SEG_T2H) );
    else if( t == -1 )
      *type = ( typ == SEG_CONNECT_BOX ? SEG_H2H :
              ( typ == SEG_BOX_CONNECT ? SEG_H2T : SEG_H2T) );
    else if( h < t )
      *type = SEG_H2H|SEG_T2T;
    else
      *type = SEG_H2T|SEG_T2H;

    return( *type>0 );
    }


/*
 *
 *  module    :  SEG_JOIN
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n1;
 *               float *v1;
 *               int    n2;
 *               float *v2;
 *               int     type;
 *               int     n;
 *               float **v;
 *               seg_join( n1, v1, n2, v2, type, n, v );
 *
 *  object    :  THIS MODULE JOINS V1 AND V2 WITH A JOIN OF A CERTAIN TYPE
 *
 */

 extern void
 seg_join ( int n1, float *v1, int n2, float *v2, int type, int *n, float **v )
    {

/*
 *  initialisation
 */
    *n = 0;
    *v = NULL;

/*
 *  test inputs
 */
    if( n1 == 0 || v1 == NULL ||
        n2 == 0 || v2 == NULL  ) return;

/*
 *  the type can be made of several connections; pick one
 */
    if( (type&SEG_H2H) != 0 ) type = SEG_H2H;
    if( (type&SEG_H2T) != 0 ) type = SEG_H2T;
    if( (type&SEG_T2H) != 0 ) type = SEG_T2H;
    if( (type&SEG_T2T) != 0 ) type = SEG_T2T;

/*
 *  join according to type
 */
    switch(type)
      {
      case SEG_H2H : seg_joinh2h( n1, v1, n2, v2, n, v );
                     break;

      case SEG_H2T : seg_joinh2t( n1, v1, n2, v2, n, v );
                     break;

      case SEG_T2H : seg_joint2h( n1, v1, n2, v2, n, v );
                     break;

      case SEG_T2T : seg_joint2t( n1, v1, n2, v2, n, v );
                     break;
      }
    }


/*
 *
 *  module    :  SEG_JOINH2H
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n1;
 *               float *v1;
 *               int    n2;
 *               float *v2;
 *               int     n;
 *               float **v;
 *               seg_joinh2h( n1, v1, n2, v2, n, v );
 *
 *  object    :  THIS MODULE JOINS HEAD OF V1 AND HEAD OF V2 
 *
 */

 extern void
 seg_joinh2h ( int n1, float *v1, int n2, float *v2, int *n, float **v )
    {
    int    i;
    float *p;
    float *px;

/*
 *  initialisation
 */
    *n = 0;
    *v = NULL;

/*
 *  test inputs
 */
    if( n1 == 0 || v1 == NULL ||
        n2 == 0 || v2 == NULL  ) return;

/*
 *  make sure no point duplications
 */
    if( pnt_EQ(v1[0],v1[1],v2[0],v2[1]) )
      *n = n1 + n2 - 1;
    else
      *n = n1 + n2;

/*
 *  allocate space for v
 */
    *v = (float *) malloc ( 2 * *n  * sizeof(float) );

/*
 *  connect the two segments
 */
    p  = *v;
    px = &v1[2*n1-2];
    for( i=0; i<n1; ++i, p+=2, px-=2 )
       {
       p[0] = px[0];
       p[1] = px[1];
       }
    p = &(*v)[ 2 * (*n-n2) ];
    memcpy(  p, v2, 2*n2*sizeof(float) );
    }


/*
 *
 *  module    :  SEG_JOINH2T
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n1;
 *               float *v1;
 *               int    n2;
 *               float *v2;
 *               int     n;
 *               float **v;
 *               seg_joinh2t( n1, v1, n2, v2, n, v );
 *
 *  object    :  THIS MODULE JOINS HEAD OF V1 TO TAIL OF V2
 *
 */

 extern void
 seg_joinh2t ( int n1, float *v1, int n2, float *v2, int *n, float **v )
    {
    int x, y;
    float *p;

/*
 *  initialisation
 */
    *n = 0;
    *v = NULL;

/*
 *  test inputs
 */
    if( n1 == 0 || v1 == NULL ||
        n2 == 0 || v2 == NULL  ) return;

/*
 *  compute tail j position
 */
    x = 2*n2-2;
    y = x+1;

/*
 *  make sure no point duplications
 */
    if( pnt_EQ(v1[0],v1[1],v2[x],v2[y])  )
      *n = n1 + n2 - 1;
    else
      *n = n1 + n2;

/*
 *  allocate space for v
 */
    *v = (float *) malloc ( 2 * *n  * sizeof(float) );

/*
 *  connect the two segments
 */
    p = &(*v)[ 2 * (*n-n1) ];
    memcpy( *v, v2, 2*n2*sizeof(float) );
    memcpy(  p, v1, 2*n1*sizeof(float) );
    }


/*
 *
 *  module    :  SEG_JOINT2H
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n1;
 *               float *v1;
 *               int    n2;
 *               float *v2;
 *               int     n;
 *               float **v;
 *               seg_joint2h( n1, v1, n2, v2, n, v );
 *
 *  object    :  THIS MODULE JOINS TAIL OF V1 TO HEAD OF V2
 *
 */

 extern void
 seg_joint2h ( int n1, float *v1, int n2, float *v2, int *n, float **v )
    {
    int x, y;
    float *p;

/*
 *  initialisation
 */
    *n = 0;
    *v = NULL;

/*
 *  test inputs
 */
    if( n1 == 0 || v1 == NULL ||
        n2 == 0 || v2 == NULL  ) return;

/*
 *  compute tail j position
 */
    x = 2*n1-2;
    y = x+1;

/*
 *  make sure no point duplications
 */
    if( pnt_EQ(v1[x],v1[y],v2[0],v2[1])  )
      *n = n1 + n2 - 1;
    else
      *n = n1 + n2;

/*
 *  allocate space for v
 */
    *v = (float *) malloc ( 2 * *n  * sizeof(float) );

/*
 *  connect the two segments
 */
    p = &(*v)[ 2 * (*n-n2) ];
    memcpy( *v, v1, 2*n1*sizeof(float) );
    memcpy(  p, v2, 2*n2*sizeof(float) );
    }



/*
 *
 *  module    :  SEG_JOINT2T
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n1;
 *               float *v1;
 *               int    n2;
 *               float *v2;
 *               int     n;
 *               float **v;
 *               seg_joint2t( n1, v1, n2, v2, n, v );
 *
 *  object    :  THIS MODULE JOINS TAIL OF V1 TO TAIL OF V2
 *
 */

 extern void
 seg_joint2t ( int n1, float *v1, int n2, float *v2, int *n, float **v )
    {
    int    i;
    float *p;
    float *px;

/*
 *  initialisation
 */
    *n = 0;
    *v = NULL;

/*
 *  test inputs
 */
    if( n1 == 0 || v1 == NULL ||
        n2 == 0 || v2 == NULL  ) return;

/*
 *  make sure no point duplications
 */
    if( pnt_EQ(v1[2*n1-2],v1[2*n1-1],v2[2*n2-2],v2[2*n2-1]) )
      *n = n1 + n2 - 1;
    else
      *n = n1 + n2;

/*
 *  allocate space for v
 */
    *v = (float *) malloc ( 2 * *n  * sizeof(float) );

/*
 *  connect the two segments
 */
    p  = &(*v)[ 2 * (*n-n2) ];
    px = &v2[ 2 * n2 - 2 ];
    memcpy( *v, v1, 2*n1*sizeof(float) );
    for( i=0; i<n2; ++i, p+=2, px-=2 )
       {
       p[0] = px[0];
       p[1] = px[1];
       }
    }


/*
 *
 *  module    :  SEG_NOCROSS
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n;
 *               float *v;
 *               int   *n1;
 *               float **v1;
 *               int   *n2;
 *               float **v2;
 *               seg_nocross( n, v, n1, v1, n2, v2 );
 *
 *  object    :  THIS MODULE TAKE OUT 1 CROSSING IN VECTOR IF ANY
 *               HERE A CROSSING IS CONSIDER TO BE VECTOR OVERLAP
 *               WITHIN THE SEGMENTS
 *
 */

 extern int
 seg_nocross ( int n, float *v, int *n1, float **v1, int *n2, float **v2 )
    {
    int    nx;
    float *vx;

    int    ny;
    float *vy;

    int    i, j, k;
    int    icross;
    float x11,y11, x12,y12;
    float x21,y21, x22,y22;
    float x,y;

/*
 *  initialisation
 */
    *n1 = 0;
    *n2 = 0;
    *v1 = NULL;
    *v2 = NULL;
    vy = NULL;

/*
 *  no segment given return
 */
    if( v == NULL ) return(0);

/*
 *  take out duplicate points if any
 */
    if( seg_nodup( n, v, &nx, &vx ) == 0 )
      {
      nx = n;
      vx = v;
      }

/*
 *  less than 3 points
 */
    if( nx <= 3 )
      {
      if( vx != v ) free(vx);
      return(0);
      }

/*
 *  tries to find one loop
 */
    if( seg_noloop( nx, vx, n1, v1, n2, v2 ) != 0 )
      {
      if( vx != v ) free(vx);
      return(1);
      }

/*
 *  tries to find one crossing
 */
    icross = 0;
    for( j=1;   j<nx;   ++j )
       {
       for( i=j+2; i<nx-1; ++i )
          {
          x11 = vx[2*j-2];
          y11 = vx[2*j-1];
          x12 = vx[2*j];
          y12 = vx[2*j+1];
          x21 = vx[2*i-2];
          y21 = vx[2*i-1];
          x22 = vx[2*i];
          y22 = vx[2*i+1];

          if( seg_crosspt( x11,y11,x12,y12, x21,y21,x22,y22, &x, &y ) != 0 )
            {
printf(" x11 = %f ,y11 = %f ,x12 = %f ,y12 = %f \n",x11,y11,x12,y12);
printf(" x21 = %f ,y21 = %f ,x22 = %f ,y22 = %f \n",x21,y21,x22,y22);
printf(" x = %f ,y = %f \n",x,y);
printf(" j = %d ,i = %d \n",j,i);
            icross = 1;
            break;
            }
         }
       if( icross == 1 ) break;
       }

    if( icross == 1 )
      {
      printf(" start VX = ");
      for( k=0; k<4; ++k) printf(" %f ",vx[k]);
      printf(" \n ");
      printf(" end   VX = ");
      for( k=0; k<4; ++k) printf(" %f ",vx[2*nx-4+k]);
      printf(" \n ");
      printf("       NX = %d\n",nx);

      ny = nx+2;
      vy = (float *) malloc ( 2 * ny * sizeof(float) );

      memcpy( vy, vx, 2 * j * sizeof(float) );
      vy[2*j]   = x;
      vy[2*j+1] = y;
      memcpy( &vy[2*j+2], &vx[2*j],(2*(i-j))*sizeof(float) );
      vy[2*i+2] = x;
      vy[2*i+3] = y;
      memcpy( &vy[2*i+4], &vx[2*i],(2*(nx-i))*sizeof(float) );

      printf(" start VY = ");
      for( k=0; k<4; ++k) printf(" %f ",vy[k]);
      printf(" \n ");
      printf(" end   VY = ");
      for( k=0; k<4; ++k) printf(" %f ",vy[2*ny-4+k]);
      printf(" \n ");
      printf("       NY = %d\n",ny);

      if( vx != v ) free(vx);

      if( seg_noloop( ny, vy, n1, v1, n2, v2 ) == 0 ) 
        {
        free(vy);
        printf(" an error occured \n");
        return(0);
        }

      printf(" start V1 = ");
      for( k=0; k<4; ++k) printf(" %f ",(*v1)[k]);
      printf(" \n ");
      printf(" end   V1 = ");
      for( k=0; k<4; ++k) printf(" %f ",(*v1)[*n1*2-4+k]);
      printf(" \n ");
      printf("       N1 = %d\n",*n1);

      printf(" start V2 = ");
      for( k=0; k<4; ++k) printf(" %f ",(*v2)[k]);
      printf(" \n ");
      printf(" end   V2 = ");
      for( k=0; k<4; ++k) printf(" %f ",(*v2)[*n2*2-4+k]);
      printf(" \n ");
      printf("       N2 = %d\n",*n2);

      free(vy);
      return(1);
      }

    if( vx != v    ) free(vx);
    return(0);
    }


/*
 *
 *  module    :  SEG_NODUP
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n;
 *               float *v;
 *               int   *n2;
 *               float **v2;
 *               seg_nodup( n, v, n2, v2 );
 *
 *  object    :  THIS MODULE TAKE OUT DUPLICATE POINTS IN VECTOR IF ANY
 *
 */

 extern int
 seg_nodup ( int n, float *v, int *n2, float **v2 )
    {
    int   i, j, m;
    float *v1x, *v1y;
    float *v2x, *v2y;

/*
 *  initialisation
 */
    *n2 = 0;
    *v2 = NULL;

/*
 *  no segment given return
 */
    if( v == NULL ) return(0);

/*
 *  only one point
 */
    if( n == 1 ) return(0);

/*
 *  loop on all the points
 */
    v1x = v   - 2;
    v1y = v   - 1;
    v2x = v      ;
    v2y = v   + 1;
    m   = 2 * n;

    for( i=2; i<m; i+=2 )
       if( pnt_EQ(v1x[i],v1y[i],v2x[i],v2y[i]) ) break;

/*
 *  no duplication found
 */
    if( i == m ) return(0);

/*
 *  create a new vector without duplications
 */
    *v2 = (float *) malloc ( 2 * n * sizeof(float) );

/*
 *  pick only uniq consecutive points
 */
    v1x = *v2;
    v1y = *v2 + 1;

    v2x = v;
    v2y = v2x + 1;

    v1x[0] = v2x[0];
    v1y[0] = v2y[0];

    j = 0;
    for( i=2; i<m; i+=2 )
       {
       if( pnt_EQ(v1x[j],v1y[j],v2x[i],v2y[i]) ) continue;
       j+=2;
       v1x[j] = v2x[i];
       v1y[j] = v2y[i];
       }

    *n2 = j/2 + 1;

    return(1);
    }


/*
 *
 *  module    :  SEG_NOLOOP
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n;
 *               float *v;
 *               int   *n1;
 *               float **v1;
 *               int   *n2;
 *               float **v2;
 *               seg_noloop( n, v, n1, v1, n2, v2 );
 *
 *  object    :  THIS MODULE TAKE OUT 1 LOOP IN VECTOR IF ANY
 *               HERE A LOOP IS CONSIDER TO BE EQUAL POINTS
 *               WITHIN THE SEGMENTS
 *
 */

 extern int
 seg_noloop ( int n, float *v, int *n1, float **v1, int *n2, float **v2 )
    {
    int    nx;
    float *vx;

    int    i, j, m;
    float *p;

/*
 *  initialisation
 */
    *n1 = 0;
    *n2 = 0;
    *v1 = NULL;
    *v2 = NULL;

/*
 *  no segment given return
 */
    if( v == NULL ) return(0);

/*
 *  take out duplicate points if any
 */
    if( seg_nodup( n, v, &nx, &vx ) == 0 )
      {
      nx = n;
      vx = v;
      }

/*
 *  less than 3 points
 */
    if( nx <= 3 )
      {
      if( vx != v ) free(vx);
      return(0);
      }

/*
 *  tries to find one loop
 */
    m = nx-1;
    p = vx;
    for( i=0; i<m; ++i, p+=2 )
       {
       j = seg_find( p[0], p[1], m-i, &p[2] );
       if( j < 0 || !pnt_EQ(p[0],p[1],p[2+2*j],p[3+2*j]) ) continue;
       j += i+1;
       if( j == m && i == 0 ) continue;
       break;
       }

    if( j < 0 || i == m )
      {
      if( vx != v ) free(vx);
      return(0);
      }

/*
 *  cut the vector in 2 parts the loop and the rest
 */
    *n1 = nx - (j-i);
    *v1 = (float *) malloc( 2 * *n1 * sizeof(float) );
    p = *v1;
    memcpy( p,       vx,      2 *  i  * sizeof(float) );
    memcpy( &p[2*i], &vx[2*j],(2*(nx-j))*sizeof(float) );

    *n2 = j - i + 1;
    *v2 = (float *) malloc( 2* *n2 *sizeof(float) );
    memcpy( *v2,  &vx[2*i],  2* *n2 *sizeof(float) );

    if( vx != v ) free(vx);

    return(1);
    }


/*
 *
 *  module    :  SEG_PRINT
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int    n;
 *               float *v;
 *               seg_print( n, v );
 *
 *  object    :  THIS MODULE PRINTS OUT THE VECTOR 
 *
 */

 extern void
 seg_print ( int n, float *v )
    {
    int    i;
    float *p=v;

    printf(" \n");

    printf(" n = %d \n",n);
     
    if( v == NULL )
      {
      printf(" vect = NULL \n");
      return;
      }

    for( i=0; i<n; ++i, p+=2 )
      printf(" %f %f \n",p[0],p[1]);

    printf(" \n");

    return;
    }


/*
 *
 *  module    :  SEG_TYPE
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
 *               float   n;
 *               float  *v;
 *               int    *type;
 *               seg_type( n, v, type );
 *
 *  object    :  THIS MODULE FINDS THE SEGMENT TYPE
 *
 */

 extern int
 seg_type ( int n, float *v, int *type )
    {
    int i, j;
    int x, y ;
    int faux;

    *type = 0;
    faux = (0==1);

/*
 *  check the input
 */
    if( n <= 0 || v == NULL ) return(faux);

/*
 *  check for closing
 */
    i = seg_isclose(n,v);
    if( i ) { *type = SEG_CLOSE; return(!faux); }

/*
 *  assignations
 */
    x  = 2 * n - 2;
    y  = x + 1;

/*
 *  check for boxing
 */
    i = pnt_almostBOX(v[0],v[1]);
    j = pnt_almostBOX(v[x],v[y]);
    if( i && j ) { *type = SEG_BOX;         return(i); }
    if( i )      { *type = SEG_BOX_CONNECT; return(i); }
    if( j )      { *type = SEG_CONNECT_BOX; return(j); }

/*
 *  other than this type is connect
 */
    if( *type == 0 ) *type = SEG_CONNECT;

    return(!faux);
    }


#ifdef   TEST
/*
 *
 *  module    :  TEST_SEGMENT
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  test_segment( )
 *
 *  object    :  THIS MODULE TEST ALL PREVIOUS MODULES
 *
 */

 extern void
 test_segment()
    {

   int    i;
   int    n;
   float *v;
   int    nx;
   float *vx;

   {
   static float v1[12] = { 1.0,2.0,3.0,4.0,5.0,6.0 };
   static int   n1    = 3;
   int ny;
   int type;
   float *vy;

   if( seg_isclose(n1,v1) ) printf(" error seg_isclose\n");
   if( seg_type(n1,v1,&type) ) printf(" error seg_type0\n");
   seg_close( n1, v1, &nx, &vx );
   if( nx != 4 || vx == NULL )  printf(" error seg_close\n");
   if( !seg_isclose(nx,vx) ) printf(" error seg_isclose\n");
   if( !seg_type(nx,vx,&type) || type!= SEG_CLOSE )printf(" error seg_type1\n");

   for( i=0; i<2*n1; i++ )
      if( v1[i] != vx[i] )  printf(" error seg_close\n");
   if( v1[0]!=vx[2*nx-2] || v1[1]!=vx[2*nx-1] ) printf(" error seg_close\n");
   seg_close( nx, vx, &ny, &vy );
   if( ny != 0 || vy != NULL )  printf(" error seg_close\n");
   }


   {
   static float v1[12] = { 1.0,2.0,3.0,4.0,5.0,6.0 };
   static int   n1    = 3;
   static float v2[6] = { 7.0,8.0,0.0,1.0 };
   static int   n2    = 2;
   static float v3[6] = { 1.0,2.0,9.9,9.9 };
   static int   n3    = 2;
   static float v4[6] = { 9.9,9.9,1.0,2.0 };
   static int   n4    = 2;
   static float v5[6] = { 9.9,9.9,5.0,6.0 };
   static int   n5    = 2;
   static float v6[6] = { 5.0,6.0,9.9,9.9 };
   static int   n6    = 2;

   i = seg_find( 3.0, 4.0, n1, v1 );
   if( i !=  1 ) printf(" error seg_find \n");
   i = seg_find( 3.4, 4.3, n1, v1 );
   if( i >= 0 ) printf(" error seg_find \n");

   seg_joinh2h( n1, v1, n2, v2, &n, &v );
   if( n != 5 || v == NULL ) printf(" error seg_joinh2h\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*(n1-i)-2] ||
          v1[2*i+1] != v[2*(n1-i)-1]  ) printf(" error seg_joinh2h\n");
   for( i=0; i<n2; ++i )
      if( v2[2*i]   != v[2*(n1+i)  ] ||
          v2[2*i+1] != v[2*(n1+i)+1]  ) printf(" error seg_joinh2h\n");
   n=0; free(v);

   seg_joinh2h( n1, v1, n3, v3, &n, &v );
   if( n != 4 || v == NULL ) printf(" error seg_joinh2h\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*(n1-i)-2] ||
          v1[2*i+1] != v[2*(n1-i)-1]  ) printf(" error seg_joinh2h\n");
   for( i=0; i<n3; ++i )
      if( v3[2*i  ] != v[2*(n1+i)-2] ||
          v3[2*i+1] != v[2*(n1+i)-1]  ) printf(" error seg_joinh2h\n");
   n=0; free(v);

   seg_joinh2t( n1, v1, n2, v2, &n, &v );
   if( n != 5 || v == NULL ) printf(" error seg_joinh2t\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*(n2+i)  ] ||
          v1[2*i+1] != v[2*(n2+i)+1]  ) printf(" error seg_joinh2t\n");
   for( i=0; i<n2; ++i )
      if( v2[2*i]   != v[2*i  ] ||
          v2[2*i+1] != v[2*i+1]  ) printf(" error seg_joinh2t\n");
   n=0; free(v);

   seg_joinh2t( n1, v1, n4, v4, &n, &v );
   if( n != 4 || v == NULL ) printf(" error seg_joinh2t\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*(n4+i)-2] ||
          v1[2*i+1] != v[2*(n4+i)-1]  ) printf(" error seg_joinh2t\n");
   for( i=0; i<n2; ++i )
      if( v4[2*i]   != v[2*i  ] ||
          v4[2*i+1] != v[2*i+1]  ) printf(" error seg_joinh2t\n");
   n=0; free(v);

   seg_joint2h( n1, v1, n2, v2, &n, &v );
   if( n != 5 || v == NULL ) printf(" error seg_joint2h\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*i  ] ||
          v1[2*i+1] != v[2*i+1]  ) printf(" error seg_joint2h\n");
   for( i=0; i<n2; ++i )
      if( v2[2*i]   != v[2*(n1+i)  ] ||
          v2[2*i+1] != v[2*(n1+i)+1]  ) printf(" error seg_joint2h\n");
   n=0; free(v);

   seg_joint2h( n1, v1, n6, v6, &n, &v );
   if( n != 4 || v == NULL ) printf(" error seg_joint2h\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*i  ] ||
          v1[2*i+1] != v[2*i+1]  ) printf(" error seg_joint2h\n");
   for( i=0; i<n6; ++i )
      if( v6[2*i]   != v[2*(n1+i)-2] ||
          v6[2*i+1] != v[2*(n1+i)-1]  ) printf(" error seg_joint2h\n");
   n=0; free(v);

   seg_joint2t( n1, v1, n2, v2, &n, &v );
   if( n != 5 || v == NULL ) printf(" error seg_joint2h\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*i  ] ||
          v1[2*i+1] != v[2*i+1]  ) printf(" error seg_joint2t\n");
   for( i=0; i<n2; ++i )
      if( v2[2*i]   != v[2*(n-i)-2] ||
          v2[2*i+1] != v[2*(n-i)-1]  ) printf(" error seg_joint2t\n");
   if( n == 0 || v == NULL ) printf(" error seg_joint2t\n");
   n=0; free(v);

   seg_joint2t( n1, v1, n5, v5, &n, &v );
   if( n != 4 || v == NULL ) printf(" error seg_joint2h\n");
   for( i=0; i<n1; ++i )
      if( v1[2*i]   != v[2*i  ] ||
          v1[2*i+1] != v[2*i+1]  ) printf(" error seg_joint2t\n");
   for( i=0; i<n5; ++i )
      if( v5[2*i]   != v[2*(n-i)-2] ||
          v5[2*i+1] != v[2*(n-i)-1]  ) printf(" error seg_joint2t\n");
   if( n == 0 || v == NULL ) printf(" error seg_joint2t\n");
   n=0; free(v);

   }

   {
   static float v1[12] = { 1.0,2.0,1.0,2.0,3.0,4.0,3.0,4.0,5.0,6.0,5.0,6.0 };
   static int   n1    = 6;
   static float v2[6] = { 1.0,2.0,3.0,4.0,5.0,6.0 };
   static int   n2    = 3;

   i = seg_nodup( n1, v1, &n, &v );
   if( i == 0 ) printf(" error seg_nodup \n");

   for( i=0; i<2*n; ++i )
      if( v[i] != v2[i] ) printf(" error seg_nodup \n");
   i = seg_nodup( n, v, &nx, &vx );
   if( i != 0 || nx != 0 || vx != NULL ) printf(" error seg_nodup \n");
   }

   {
   static float v1[8] = { 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0 };
   static int   n1    = 4;

   static float v2[8] = { 1.0,2.0,3.0,4.0,1.0,2.0,7.0,8.0 };
   static int   n2    = 4;


   i = seg_noloop( n1, v1, &n, &v, &nx, &vx );
   if( i  != 0 ) printf(" error seg_noloop \n");
   if( n  != 0 || v  != NULL ) printf(" error seg_noloop \n");
   if( nx != 0 || vx != NULL ) printf(" error seg_noloop \n");

   v1[6] = 1.0;
   v1[7] = 2.0;
   i = seg_noloop( n1, v1, &n, &v, &nx, &vx );
   if( i  != 0 ) printf(" error seg_noloop \n");
   if( n  != 0 || v  != NULL ) printf(" error seg_noloop \n");
   if( nx != 0 || vx != NULL ) printf(" error seg_noloop \n");

   v1[4] = 3.0;
   v1[5] = 4.0;
   i = seg_noloop( n1, v1, &n, &v, &nx, &vx );
   if( i  != 0 ) printf(" error seg_noloop \n");
   if( n  != 0 || v  != NULL ) printf(" error seg_noloop \n");
   if( nx != 0 || vx != NULL ) printf(" error seg_noloop \n");

   i = seg_noloop( n2, v2, &n, &v, &nx, &vx );
   if( i  == 0 ) printf(" error seg_noloop \n");
   if( n  != 2 || v  == NULL ) printf(" error seg_noloop \n");
   if( nx != 3 || vx == NULL ) printf(" error seg_noloop \n");
   if( v != NULL )  
     {
     if( v[0] != 1.0 || v[1] != 2.0 ) printf(" error seg_noloop \n");
     if( v[2] != 7.0 || v[3] != 8.0 ) printf(" error seg_noloop \n");
     }
   if( vx != NULL )  
     {
     if( vx[0] != 1.0 || vx[1] != 2.0 ) printf(" error seg_noloop \n");
     if( vx[2] != 3.0 || vx[3] != 4.0 ) printf(" error seg_noloop \n");
     if( vx[4] != 1.0 || vx[5] != 2.0 ) printf(" error seg_noloop \n");
     }
   }

   {
   static float v[14]={1.1,1.2,2.1,2.2,3.1,3.2,4.1,4.2,5.1,5.2,6.1,6.2,7.1,7.2};
   static int   n    = 7;

   static float v1[6]={1.11,1.21,2.11,2.21,3.11,3.21 };
   static int   n1   = 3;

   int    nc;
   float *vc;

   if( seg_closein(n1,v1,n,v,&nc,&vc) > 0 ) printf(" error seg_closein \n");
   v1[0]=2.1;
   v1[1]=2.2;
   if( seg_closein(n1,v1,n,v,&nc,&vc) > 0 ) printf(" error seg_closein \n");
   v1[4]=5.1;
   v1[5]=5.2;
   if( seg_closein(n1,v1,n,v,&nc,&vc) <=0 ) printf(" error seg_closein \n");
   v1[0]=5.1;
   v1[1]=5.2;
   v1[4]=2.1;
   v1[5]=2.2;
   if( seg_closein(n1,v1,n,v,&nc,&vc) <=0 ) printf(" error seg_closein \n");
   v[12] = 1.1;
   v[13] = 1.2;
   if( seg_closein(n1,v1,n,v,&nc,&vc) <=0 ) printf(" error seg_closein \n");
   v1[0]=2.1;
   v1[1]=2.2;
   v1[4]=6.1;
   v1[5]=6.2;
   if( seg_closein(n1,v1,n,v,&nc,&vc) <=0 ) printf(" error seg_closein \n");
   v1[0]=6.1;
   v1[1]=6.2;
   v1[4]=1.1;
   v1[5]=1.2;
   if( seg_closein(n1,v1,n,v,&nc,&vc) <=0 ) printf(" error seg_closein \n");
   }

   {
   static float v[14]={1.1,1.2,2.1,2.2,3.1,3.2,4.1,4.2,5.1,5.2,6.1,6.2,7.1,7.2};
   static int   n    = 7;

   int    k, n1, n2;
   float *v1, *v2;

   k = seg_cut( n,v,1, &n1, &v1, &n2, &v2 );
   if( k  <  0 ) printf(" error seg_cut \n");
   if( n1 != 2 ) printf(" error seg_cut \n");
   if( n2 != 6 ) printf(" error seg_cut \n");
   for( k = 0; k<2*n1; ++k ) if( v[k] != v1[k] ) printf(" error seg_cut \n");
   for( k = 0; k<2*n2; ++k ) 
      if( v[2+k] != v2[k] ) printf(" error seg_cut \n");
   k = seg_cut( n,v,0, &n1, &v1, &n2, &v2 );
   if( k  >  0 ) printf(" error seg_cut \n");
   k = seg_cut( n,v,n-1, &n1, &v1, &n2, &v2 );
   if( k  >  0 ) printf(" error seg_cut \n");
   }

   {
   static float v1[4] = { 1.0, 2.0, 3.0, 4.0 };
   static float v2[4] = { 5.0, 6.0, 7.0, 8.0 };
   int  n = 2;
   int  type;

   pnt_setseuil( 0.1 );

   if( seg_isconn( n,v1,n,v2,&type)  ) printf(" error seg_isconn 1 \n");

   v2[0] = 1.0; v2[1]=2.0;
   if( !seg_isconn( n,v1,n,v2,&type) ) printf(" error seg_isconn 2 \n");
   if( type != SEG_H2H )               printf(" error seg_isconn 2b\n");

   v2[0] = 3.0; v2[1]=4.0;
   if( !seg_isconn( n,v1,n,v2,&type) ) printf(" error seg_isconn 3 \n");
   if( type != SEG_T2H )               printf(" error seg_isconn 3b\n");

   v2[0] = 5.0; v2[1]=6.0;
   v2[2] = 1.0; v2[3]=2.0;
   if( !seg_isconn( n,v1,n,v2,&type) ) printf(" error seg_isconn 4 \n");
   if( type != SEG_H2T )               printf(" error seg_isconn 4b\n");

   v2[2] = 3.0; v2[3]=4.0;
   if( !seg_isconn( n,v1,n,v2,&type) || type != SEG_T2T )
                                      printf(" error seg_isconn 5 \n");
   v2[0] = 1.0; v2[1]=2.0;
   if( !seg_isconn( n,v1,n,v2,&type) || type != (SEG_H2H|SEG_T2T) )
                                      printf(" error seg_isconn 6 \n");
   v2[0] = 3.0; v2[1]=4.0;
   v2[2] = 1.0; v2[3]=2.0;
   if( !seg_isconn( n,v1,n,v2,&type) || type != (SEG_H2T|SEG_T2H) )
                                      printf(" error seg_isconn 7 \n");
   }


   {
   static float v[4] = { -64.7, 103.0, -63.0, 100.3 };
   int type;
   pnt_setbox( -65.0, 100.0, -60.0, 105.0 );
   pnt_setseuil( 0.5 );

   if( ! seg_isbox( 2, v )  ) printf(" error seg_isbox\n");
   if( ! seg_type(2,v,&type) || type != SEG_BOX ) printf(" error seg_type2\n");
   v[0] = -62.0;
   if( seg_isbox( 2, v )  ) printf(" error seg_isbox\n");
   if( ! seg_type(2,v,&type) || type != SEG_CONNECT_BOX ) printf(" error seg_type3\n");
   v[3] = 104.0;
   if( seg_type(2,v,&type) ) printf(" error seg_type4\n");
   v[0] = -64.7;
   if( ! seg_type(2,v,&type) || type != SEG_BOX_CONNECT ) printf(" error seg_type5\n");
   }

   {
   static float v1[4] = { -64.7, 103.0, -63.0, 103.3 };
   static float v2[4] = { -63.0, 103.3, -63.0, 100.3 };
   static float v3[4] = { -62.7, 103.0, -63.0, 103.3 };
   static float v4[4] = { -63.0, 103.3, -62.7, 103.3 };
   int type;
   pnt_setbox( -65.0, 100.0, -60.0, 105.0 );
   pnt_setseuil( 0.5 );

   if( seg_isconn( 2, v1, 2, v2, &type ) )
     {
     if( ! seg_connbox (2, v1, 2, v2, type ) ) printf(" error seg_connbox \n");
     }
   else 
     printf(" error seg_isconn \n");

   if( seg_isconn( 2, v3, 2, v4, &type ) )
     {
     if( ! seg_connclose(2,v1,2,v2,type) ) printf(" error seg_connclose \n");
     }
   else 
     printf(" error seg_isconn \n");
   }

   {
   static
   float v[14] = { 1.1,1.2, 2.1,2.2, 3.1,3.2, 4.1,4.2,
                   5.1,5.2, 6.1,6.2, 7.1,7.2 };
   static float v1[4] = { 1.0,2.0,3.0,4.0 };
   static float v2[6] = { 2.1,2.2, 3.1,3.2, 4.1,4.2 };
   static float v3[6] = { 4.1,4.2, 3.1,3.2, 2.1,2.2 };
   static float v4[6] = { 6.1,6.2, 7.1,7.2, 1.1,1.2 };
   static float v5[6] = { 2.1,2.2, 1.1,1.2, 7.1,7.2 };

   if( seg_into( 7, v, 2, v1 ) ) printf(" error seg_into 1\n");
   if( seg_into( 2, v1, 7, v ) ) printf(" error seg_into 1\n");

   if( ! seg_into( 3, v2, 7, v ) ) printf(" error seg_into 2\n");
   if( ! seg_into( 7, v, 3, v2 ) ) printf(" error seg_into 2\n");

   if( ! seg_into( 3, v3, 7, v ) ) printf(" error seg_into 3\n");
   if( ! seg_into( 7, v, 3, v3 ) ) printf(" error seg_into 3\n");

   if( ! seg_into( 3, v4, 7, v ) ) printf(" error seg_into 4\n");
   if( ! seg_into( 7, v, 3, v4 ) ) printf(" error seg_into 4\n");

   if( ! seg_into( 3, v5, 7, v ) ) printf(" error seg_into 5\n");
   if( ! seg_into( 7, v, 3, v5 ) ) printf(" error seg_into 5\n");
   printf(" seg_into ?\n");

   }

   }

 main(){ test_segment(); }

#endif /* TEST */


