/*
 *
 * file     : POL.C
 *
 * author   : Michel Grenier
 *
 * revision : V0.0
 *
 * status   : DEVELOPMENT
 *
 * language : C
 *
 * object   : THIS FILE CUTS THE EDGES OF A POLYGONE
 *            INSIDE A LATLON BOX
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
#include "pol.h"

#define   EPSILON    0.00009

#define   PX         0   
#define   PY         1   

#define   P_EQ_L     0   /*  P_EQ_L << 2 = 0 */
#define   P_LT_L     1   /*  P_LT_L << 2 = 4 */
#define   P_GT_L     2   /*  P_GT_L << 2 = 8 */

#define   PE_TO_PE   0   /*  P_EQ_L << 2 + P_EQ_L = 0 + 0 =  0 */
#define   PE_TO_PL   1   /*  P_EQ_L << 2 + P_LT_L = 0 + 1 =  1 */
#define   PE_TO_PG   2   /*  P_EQ_L << 2 + P_GT_L = 0 + 2 =  2 */
#define   PL_TO_PE   4   /*  P_LT_L << 2 + P_EQ_L = 4 + 0 =  4 */
#define   PL_TO_PL   5   /*  P_LT_L << 2 + P_LT_L = 4 + 1 =  5 */
#define   PL_TO_PG   6   /*  P_LT_L << 2 + P_GT_L = 4 + 2 =  6 */
#define   PG_TO_PE   8   /*  P_GT_L << 2 + P_EQ_L = 8 + 0 =  8 */
#define   PG_TO_PL   9   /*  P_GT_L << 2 + P_LT_L = 8 + 1 =  9 */
#define   PG_TO_PG  10   /*  P_GT_L << 2 + P_GT_L = 8 + 2 = 10 */

 typedef  struct {
                    int n;
                    float minx;
                    float miny;
                    float maxx;
                    float maxy;
                    float *seg;
                 }  Seg;

/*
 *  joining cuts to a closed polygon
 */

 static void
 joinxy(int nc, Seg *cuts, int i, int *pos, int n, POLOUT cutout )
    {
    int     j,k;

    int     pmin;
    int     n1,  n2, type, tmin;
    float  *v1, *v2, d,    dmin;

    int    nr;
    float  minx, miny, maxx, maxy;
    float *vr;

    j = nc; /* useless compiler warning shut up */
            /* this should be looked at one day */

/*
 *  start with the enclosing parts
 */
    nr = cuts[i].n;
    vr = cuts[i].seg;
    cuts[i].n = 0;

/*
 *  for all cuts, join with closest each iteration
 *  there may not be any
 */
    for( j=0; j<n; ++j )
       {
       if( pos[j] == -1 ) continue;
       n1   = nr;
       v1   = vr;
       pmin = -1;
       dmin = 4.0e7;
/*
 *  find closest to resulting and how to join
 */
       for( k=0; k<n; k++ )
          {
          if( pos[k] == -1 ) continue;
          if( cuts[pos[k]].n == 0 ) continue;
          n2 = cuts[pos[k]].n;
          v2 = cuts[pos[k]].seg;
          if( ! seg_connhow( n1, v1, n2, v2, &type, &d ) )
            { printf(" ERROR 1 join\n"); exit(1); }
          if( d < dmin )
            {
            dmin = d;
            pmin = k;
            tmin = type;
            }
          }
/*
 *  join closest cuts found
 */

       if( pmin == -1 ){ printf(" ERROR 2 join\n"); exit(1); }
       n2 = cuts[pos[pmin]].n;
       v2 = cuts[pos[pmin]].seg;
       seg_join(n1,v1,n2,v2,tmin,&nr,&vr);
       free(v1);
       free(v2);
       cuts[pos[pmin]].n   = 0;
       cuts[pos[pmin]].seg = NULL;
       }
        
/*
 *  for resulting polygon
 *  check close, find min/max and sends out the resulting polygon
 */
    if( vr[0] != vr[2*nr-2] || vr[1] != vr[2*nr-1] )
      {
      nr += 1;
      vr  = (float *)realloc(vr,nr*2*sizeof(float));
      vr[2*nr-2] = vr[0];
      vr[2*nr-1] = vr[1];
      }

    minx = maxx = vr[0];
    miny = maxy = vr[1];
    for( k=1; k<nr; k++ )
       {
       if( minx > vr[2*k]   ) minx = vr[2*k];
       if( maxx < vr[2*k]   ) maxx = vr[2*k];
       if( miny > vr[2*k+1] ) miny = vr[2*k+1];
       if( maxy < vr[2*k+1] ) maxy = vr[2*k+1];
       }

    cutout( nr, minx, miny, maxx, maxy, vr );

    free(vr);
    cuts[i].seg = NULL;
    }

/*
 *  sorting edge span decreasingly
 */

 static int
 pol_cmpxy( const void *e1, const void *e2 )
    {
    float  d1,  d2;
    Seg   *s1, *s2;

    s1 = (Seg *)e1;
    s2 = (Seg *)e2;

    d1 = s1->maxy - s1->miny;
    d2 = s2->maxy - s2->miny;

    if( d1 > d2 ) return(-1);
    if( d1 < d2 ) return( 1);

    return(0);
    }

/*
 *  for all cuts, finds its joins 
 *  take away evenly inners
 *  joins the result
 */

 static void
 pol_joinxy( int nc, Seg *cuts, POLOUT cutout )
    {
    int   i,   j,   k,   l;
    float il1, il2, jl1, jl2, kl1, kl2;

    int  *pos;
    int   n;
    
/*
 *  sorting from biggest head-tail span to lowest
 */
    qsort((void *)cuts, nc, sizeof(Seg), pol_cmpxy);

/*
 *  allocate space to work
 */
    n   = 0;
    pos = (int *) malloc ( nc * sizeof(int) );

/*
 *  loop in all cuts
 */
    for( i=0; i<nc; ++i )
       {
       if( cuts[i].n == 0 ) continue;
       il1 = cuts[i].miny;
       il2 = cuts[i].maxy;

/*
 *  find inner cuts and check if
 *  seg inside another one skip it
 */
       n = 0;
       for( j=0; j<nc; ++j )
          {
          if( j == i ) continue;
          if( cuts[j].n == 0 ) continue;
          jl1 = cuts[j].miny;
          jl2 = cuts[j].maxy;

          if( il1 <= jl1 && jl1 <= il2 ||
              il1 <= jl2 && jl2 <= il2  ){ pos[n++] = j; continue; }

          if( jl1 < il1 && il1 < jl2 ||
              jl1 < il2 && il2 < jl2  ) break;
          }
       if( j != nc ) continue;

/*
 *  release inner-inner cuts
 */
       if( n > 1 )
         {
         for( j=0; j<n; ++j )
            {
            if( pos[j] == -1 ) continue;
            jl1 = cuts[pos[j]].miny;
            jl2 = cuts[pos[j]].maxy;

            for( k=j+1; k<n; ++k )
               {
               if( k == j ) continue;
               if( pos[k] == -1 ) continue;
               kl1 = cuts[pos[k]].miny;
               kl2 = cuts[pos[k]].maxy;

               if( jl1 <= kl1 && kl1 <= jl2 ||
                   jl1 <= kl2 && kl2 <= jl2  ){ pos[k] = -1; continue; }

               if( kl1 < jl1 && jl1 < kl2 ||
                   kl1 < jl2 && jl2 < kl2  )
                 {
                 pos[j] = -1;
                 break;
                 }
               }
            }

         for( j=0, l=0; j<n; ++j )
            if( pos[j] != -1 ) pos[l++] = pos[j];
         n = l;
         }

/*
 *  join and create the resulting segment
 */
       joinxy(nc,cuts,i,pos,n,cutout);
       }

    free(pos);
    }

/*
 *  find a point where the polygone is crossing limit in x
 *  add the point if not in polygone... make the s pointer
 *  be the polygone that starts at that point
 */

 static void
 pol_crossxl( int n, 
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit,  int *ns, float *s  )
    {
    float    x,  y;

    register int            i;
    register float         *p1;

    x = minx; /* useless compiler warning shut up */
    x = maxx; /* useless compiler warning shut up */

/*
 *  find a position where the segment cross or is onto the limit
 */
    p1 = pol;
    for( i=0; i<n-1; ++i, p1+=2 )
       {
       if( fabs(p1[0]-dlimit) <= EPSILON )
         {
         p1[0] = dlimit;
         break;
         }
       if( p1[0]  < dlimit && p1[2] > dlimit ) break;
       if( p1[0]  > dlimit && p1[2] < dlimit ) break;
       }

/*
 *  onto
 */
    if( fabs(pol[2*i]-dlimit) <= EPSILON )
      {
      memcpy( &s[0],       &pol[2*i], (n-i)*2*sizeof(float) );
      memcpy( &s[2*(n-i)], &pol[2],    i   *2*sizeof(float) );
      *ns = n;
      }

/*
 *  crossing
 */
    else
      {
      p1 = &pol[2*i];
      (void) seg_cross( p1[0], p1[1], p1[2],  p1[3],
                        dlimit, miny-1.0, dlimit, maxy+1.0,
                        &x, &y );
      s[0] = dlimit;
      s[1] = (p1[1] == p1[3] ? p1[1] : y);
      i += 1;
      memcpy( &s[2],         &pol[2*i], (n-i)*2*sizeof(float) );
      memcpy( &s[2+2*(n-i)], &pol[2],    i   *2*sizeof(float) );
      n+=1;
      s[2*n-2] = dlimit;
      s[2*n-1] = s[1];
      *ns = n;
      }
    }

/*
 *  find a point where the polygone is crossing limit in y
 *  add the point if not in polygone... make the s pointer
 *  be the polygone that starts at that point
 */

 static void
 pol_crossyl( int n, 
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit,  int *ns, float *s  )
    {
    float    x,  y;

    register int            i;
    register float         *p1;

    x = miny; /* useless compiler warning shut up */
    x = maxy; /* useless compiler warning shut up */


/*
 *  find a position where the segment cross or is onto the limit
 */
    p1 = pol;
    for( i=0; i<n-1; ++i, p1+=2 )
       {
       if( fabs(p1[1]-dlimit) <= EPSILON )
         {
         p1[1] = dlimit;
         break;
         }
       if( p1[1]  < dlimit && p1[3] > dlimit ) break;
       if( p1[1]  > dlimit && p1[3] < dlimit ) break;
       }

/*
 *  onto
 */
    if( fabs(pol[2*i+1]-dlimit) <= EPSILON )
      {
      memcpy( &s[0],       &pol[2*i], (n-i)*2*sizeof(float) );
      memcpy( &s[2*(n-i)], &pol[2],    i   *2*sizeof(float) );
      *ns = n;
      }

/*
 *  crossing
 */
    else
      {
      p1 = &pol[2*i];
      (void) seg_cross( p1[0], p1[1], p1[2],  p1[3],
                        minx-1.0, dlimit, maxx+1.0, dlimit,
                        &x, &y );
      s[0] = (p1[0] == p1[2] ? p1[0] : x);
      s[1] = dlimit;
      i += 1;
      memcpy( &s[2],         &pol[2*i], (n-i)*2*sizeof(float) );
      memcpy( &s[2+2*(n-i)], &pol[2],    i   *2*sizeof(float) );
      n+=1;
      s[2*n-2] = s[0];
      s[2*n-1] = dlimit;
      *ns = n;
      }
    }

/*
 *  cuts the polygon where x is greater than limit
 */

 extern void
 pol_cutxGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT cutout )
    {
    float    x,  y;

    register int            i;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       n2   = 0;
    float    *s2   = NULL;
    int       ns2  = 0;
    float    *seg2 = NULL;

    int       nc   = 0;
    int       siz  = 0;
    Seg      *cuts = NULL;

/*
 *  outside dlimit
 */
    if( minx+EPSILON >= dlimit ) return;

/*
 *  completly inside dlimit
 */
    if( maxx-EPSILON <= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pol ); 
      return;
      }

/*
 *  enough memory to work
 */
    n2   = 0;
    s2   = (float *) realloc ( s2,   (n+4) * 2 * sizeof(float) );

    ns2  = 0;
    seg2 = (float *) realloc ( seg2, (n+4) * 2 * sizeof(float) );

/*
 *  find a position where the segment cross or is onto the limit
 *  result is place into s (global)
 */
    pol_crossxl( n, minx, miny, maxx, maxy, pol, dlimit, &n2, s2 );

/*
 *  initialize
 */
    p1     = s2;
    p2     = seg2;
    segtyp = 0;

/*
 *  loop on all the points
 */
    for( i=0; i<n2; ++i, p1+=2 )
       {
/*
 *  classify segment
 *  avoid useless computation ( fabs(p1[1]-dlimit) <= EPSILON )
 */
       pnttyp = 0;
       if( p1[0] < dlimit ) pnttyp = 1;
       if( p1[0] > dlimit ) pnttyp = 2;
       if( fabs(p1[0]-dlimit) <= EPSILON )
         {
         p1[0]  = dlimit;
         pnttyp = 0;
         }
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PE_TO_PL : 
                             p2[0] = p1[-2]; 
                             p2[1] = p1[-1];
                             p2[2] = p1[ 0];
                             p2[3] = p1[ 1];
                             p2   += 4;
                             ns2  += 2;
                             break;

             case PG_TO_PL :
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );
                             p2[0] = dlimit; 
                             p2[1] = (p1[-1] == p1[1] ? p1[1] : y);
                             p2[2] = p1[0];
                             p2[3] = p1[1];
                             p2   += 4;
                             ns2  += 2;
                             break;

/* continuing */

             case PL_TO_PL : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             p2   += 2;
                             ns2  += 1;
                             break;

/* ending conditions */

             case PL_TO_PE : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             ns2  += 1;

                             if( seg2[1] == seg2[2*ns2-1] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;

             case PL_TO_PG : 
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );
                             p2[0] = dlimit; 
                             p2[1] = (p1[-1] == p1[1] ? p1[1] : y);
                             ns2  += 1;

                             if( seg2[1] == seg2[2*ns2-1] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;
             }
       }

/*
 *  puts y end points into miny and maxy
 */
    for( i=0; i<nc; ++i )
       {
       cuts[i].miny = cuts[i].seg[1];
       cuts[i].maxy = cuts[i].seg[2*cuts[i].n-1];
       if( cuts[i].miny > cuts[i].maxy )
         {
         cuts[i].miny = cuts[i].maxy;
         cuts[i].maxy = cuts[i].seg[1];
         }
       }

/*
 *  rebuild segments
 */
    pol_joinxy(nc,cuts,cutout);

    free(s2);
    free(seg2);
    for( i=0; i<nc; ++i )
       if( cuts[i].seg != NULL )
         { printf(" WANDER ??? 3 CUTS  cutxGTl\n"); exit(1); }
    free(cuts);
    }

/*
 *  cuts the polygon where x is lower than limit
 */

 extern void
 pol_cutxLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT cutout )
    {
    float    x,  y;

    register int            i;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       n2   = 0;
    float    *s2   = NULL;
    int       ns2  = 0;
    float    *seg2 = NULL;

    int       nc   = 0;
    int       siz  = 0;
    Seg      *cuts = NULL;

/*
 *  outside dlimit
 */
    if( maxx-EPSILON <= dlimit ) return;

/*
 *  completly inside dlimit
 */
    if( minx+EPSILON >= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pol );
      return;
      }

/*
 *  enough memory to work
 */
    n2   = 0;
    s2   = (float *) realloc ( s2,   (n+4) * 2 * sizeof(float) );

    ns2  = 0;
    seg2 = (float *) realloc ( seg2, (n+4) * 2 * sizeof(float) );

/*
 *  find a position where the segment cross or is onto the limit
 *  result is place into s (global)
 */
    pol_crossxl( n, minx, miny, maxx, maxy, pol, dlimit, &n2, s2 );

/*
 *  initialize
 */
    p1     = s2;
    p2     = seg2;
    segtyp = 0;

/*
 *  loop on all the points
 */
    for( i=0; i<n2; ++i, p1+=2 )
       {
/*
 *  classify segment
 *  avoid useless computation ( fabs(p1[1]-dlimit) <= EPSILON )
 */
       pnttyp = 0;
       if( p1[0] < dlimit ) pnttyp = 1;
       if( p1[0] > dlimit ) pnttyp = 2;
       if( fabs(p1[0]-dlimit) <= EPSILON )
         {
         p1[0]  = dlimit;
         pnttyp = 0;
         }
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PE_TO_PG : 
                             p2[0] = p1[-2]; 
                             p2[1] = p1[-1];
                             p2[2] = p1[ 0];
                             p2[3] = p1[ 1];
                             p2   += 4;
                             ns2  += 2;
                             break;

             case PL_TO_PG :
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );
                             p2[0] = dlimit; 
                             p2[1] = (p1[-1] == p1[1] ? p1[1] : y);
                             p2[2] = p1[0];
                             p2[3] = p1[1];
                             p2   += 4;
                             ns2  += 2;
                             break;

/* continuing */

             case PG_TO_PG : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             p2   += 2;
                             ns2  += 1;
                             break;

/* ending conditions */

             case PG_TO_PE : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             ns2  += 1;

                             if( seg2[1] == seg2[2*ns2-1] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;

             case PG_TO_PL : 
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             dlimit, miny-1.0, dlimit, maxy+1.0,
                                             &x, &y );
                             p2[0] = dlimit; 
                             p2[1] = (p1[-1] == p1[1] ? p1[1] : y);
                             ns2  += 1;

                             if( seg2[1] == seg2[2*ns2-1] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;
             }
       }

/*
 *  puts y end points into miny and maxy
 */
    for( i=0; i<nc; ++i )
       {
       cuts[i].miny = cuts[i].seg[1];
       cuts[i].maxy = cuts[i].seg[2*cuts[i].n-1];
       if( cuts[i].miny > cuts[i].maxy )
         {
         cuts[i].miny = cuts[i].maxy;
         cuts[i].maxy = cuts[i].seg[1];
         }
       }

/*
 *  rebuild segments
 */
    pol_joinxy(nc,cuts,cutout);

    free(s2);
    free(seg2);
    for( i=0; i<nc; ++i )
       if( cuts[i].seg != NULL )
         { printf(" WANDER ??? 3 CUTS  cutxLTl\n"); exit(1); }
    free(cuts);
    }

/*
 *  cuts the polygon where y is greater than limit
 */

 extern void
 pol_cutyGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT cutout )
    {
    float    x,  y;

    register int            i;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       n2   = 0;
    float    *s2   = NULL;
    int       ns2  = 0;
    float    *seg2 = NULL;

    int       nc   = 0;
    int       siz  = 0;
    Seg      *cuts = NULL;

/*
 *  outside dlimit
 */
    if( miny+EPSILON >= dlimit ) return;

/*
 *  completly inside dlimit
 */
    if( maxy-EPSILON <= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pol );
      return;
      }

/*
 *  enough memory to work
 */
    n2   = 0;
    s2   = (float *) realloc ( s2,   (n+4) * 2 * sizeof(float) );

    ns2  = 0;
    seg2 = (float *) realloc ( seg2, (n+4) * 2 * sizeof(float) );

/*
 *  find a position where the segment cross or is onto the limit
 *  result is place into s (global)
 */
    pol_crossyl( n, minx, miny, maxx, maxy, pol, dlimit, &n2, s2 );

/*
 *  initialize
 */
    p1     = s2;
    p2     = seg2;
    segtyp = 0;

/*
 *  loop on all the points
 */
    for( i=0; i<n2; ++i, p1+=2 )
       {
/*
 *  classify segment
 *  avoid useless computation ( fabs(p1[1]-dlimit) <= EPSILON )
 */
       pnttyp = 0;
       if( p1[1] < dlimit ) pnttyp = 1;
       if( p1[1] > dlimit ) pnttyp = 2;
       if( fabs(p1[1]-dlimit) <= EPSILON )
         {
         p1[1]  = dlimit;
         pnttyp = 0;
         }
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PE_TO_PL : 
                             p2[0] = p1[-2]; 
                             p2[1] = p1[-1];
                             p2[2] = p1[ 0];
                             p2[3] = p1[ 1];
                             p2   += 4;
                             ns2  += 2;
                             break;

             case PG_TO_PL :
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             p2[0] = (p1[-2] == p1[0] ? p1[0] : x);
                             p2[1] = dlimit; 
                             p2[2] = p1[0];
                             p2[3] = p1[1];
                             p2   += 4;
                             ns2  += 2;
                             break;

/* continuing */

             case PL_TO_PL : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             p2   += 2;
                             ns2  += 1;
                             break;

/* ending conditions */

             case PL_TO_PE : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             ns2  += 1;

                             if( seg2[0] == seg2[2*ns2-2] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;

             case PL_TO_PG : 
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             p2[0] = (p1[-2] == p1[0] ? p1[0] : x);
                             p2[1] = dlimit; 
                             ns2  += 1;

                             if( seg2[0] == seg2[2*ns2-2] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;
             }
       }

/*
 *  puts y end points into miny and maxy
 */
    for( i=0; i<nc; ++i )
       {
       cuts[i].miny = cuts[i].seg[0];
       cuts[i].maxy = cuts[i].seg[2*cuts[i].n-2];
       if( cuts[i].miny > cuts[i].maxy )
         {
         cuts[i].miny = cuts[i].maxy;
         cuts[i].maxy = cuts[i].seg[0];
         }
       }

/*
 *  rebuild segments
 */
    pol_joinxy(nc,cuts,cutout);

    free(s2);
    free(seg2);
    for( i=0; i<nc; ++i )
       if( cuts[i].seg != NULL )
         { printf(" WANDER ??? 3 CUTS  pol_cutyGTl\n"); exit(1); }
    free(cuts);
    }

/*
 *  cuts the polygon where y is lower than limit
 */

 extern void
 pol_cutyLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT cutout )
    {
    float    x,  y;

    register int            i;
    register float         *p1;
    register float         *p2;
    register unsigned char  pnttyp;
    register unsigned char  segtyp;

    int       n2   = 0;
    float    *s2   = NULL;
    int       ns2  = 0;
    float    *seg2 = NULL;

    int       nc   = 0;
    int       siz  = 0;
    Seg      *cuts = NULL;

/*
 *  outside dlimit
 */
    if( maxy-EPSILON <= dlimit ) return;

/*
 *  completly inside dlimit
 */
    if( miny+EPSILON >= dlimit )
      {
      cutout( n, minx, miny, maxx, maxy, pol ); 
      return;
      }

/*
 *  enough memory to work
 */
    n2   = 0;
    s2   = (float *) realloc ( s2,   (n+4) * 2 * sizeof(float) );

    ns2  = 0;
    seg2 = (float *) realloc ( seg2, (n+4) * 2 * sizeof(float) );

/*
 *  find a position where the segment cross or is onto the limit
 *  result is place into s (global)
 */
    pol_crossyl( n, minx, miny, maxx, maxy, pol, dlimit, &n2, s2 );

/*
 *  initialize
 */
    p1     = s2;
    p2     = seg2;
    segtyp = 0;

/*
 *  loop on all the points
 */
    for( i=0; i<n2; ++i, p1+=2 )
       {
/*
 *  classify segment
 *  avoid useless computation ( fabs(p1[1]-dlimit) <= EPSILON )
 */
       pnttyp = 0;
       if( p1[1] < dlimit ) pnttyp = 1;
       if( p1[1] > dlimit ) pnttyp = 2;
       if( fabs(p1[1]-dlimit) <= EPSILON )
         {
         p1[1]  = dlimit;
         pnttyp = 0;
         }
       segtyp = (segtyp&3) << 2 | pnttyp;

/*
 *  treat according to segment type
 */
       switch( segtyp )
             {

/*  begin conditions */

             case PE_TO_PG : 
                             p2[0] = p1[-2]; 
                             p2[1] = p1[-1];
                             p2[2] = p1[ 0];
                             p2[3] = p1[ 1];
                             p2   += 4;
                             ns2  += 2;
                             break;

             case PL_TO_PG :
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             p2[0] = (p1[-2] == p1[0] ? p1[0] : x);
                             p2[1] = dlimit; 
                             p2[2] = p1[0];
                             p2[3] = p1[1];
                             p2   += 4;
                             ns2  += 2;
                             break;

/* continuing */

             case PG_TO_PG : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             p2   += 2;
                             ns2  += 1;
                             break;

/* ending conditions */

             case PG_TO_PE : 
                             p2[0] = p1[0];
                             p2[1] = p1[1];
                             ns2  += 1;

                             if( seg2[0] == seg2[2*ns2-2] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;

             case PG_TO_PL : 
                             (void)seg_cross(p1[-2], p1[-1], p1[0],  p1[1],
                                             minx-1.0, dlimit, maxx+1.0, dlimit,
                                             &x, &y );
                             p2[0] = (p1[-2] == p1[0] ? p1[0] : x);
                             p2[1] = dlimit; 
                             ns2  += 1;

                             if( seg2[0] == seg2[2*ns2-2] )
                               {
                               int i;
                               float minx,miny,maxx,maxy;
                               if( ns2 <= 3 ) break;
                               minx = maxx = seg2[0];
                               miny = maxy = seg2[1];
                               for( i=1; i<ns2; ++i )
                                  {
                                  if( minx > seg2[2*i]   ) minx = seg2[2*i];
                                  if( maxx < seg2[2*i]   ) maxx = seg2[2*i];
                                  if( miny > seg2[2*i+1] ) miny = seg2[2*i+1];
                                  if( maxy < seg2[2*i+1] ) maxy = seg2[2*i+1];
                                  }
                               cutout( ns2, minx, miny, maxx, maxy, seg2 ); 
                               break;
                               }

                             if( nc+1 > siz )
                               {
                               siz += 10;
                               cuts = (Seg *) realloc(cuts,siz * sizeof(Seg));
                               }

                             cuts[nc].n   = ns2;
                             cuts[nc].seg =(float *)malloc(ns2*2*sizeof(float));
                             memcpy(cuts[nc].seg,seg2,ns2*2*sizeof(float));
                             nc++;

                             p2    = seg2;
                             ns2   = 0;
                             break;
             }
       }

/*
 *  puts y end points into miny and maxy
 */
    for( i=0; i<nc; ++i )
       {
       cuts[i].miny = cuts[i].seg[0];
       cuts[i].maxy = cuts[i].seg[2*cuts[i].n-2];
       if( cuts[i].miny > cuts[i].maxy )
         {
         cuts[i].miny = cuts[i].maxy;
         cuts[i].maxy = cuts[i].seg[0];
         }
       }

/*
 *  rebuild segments
 */
    pol_joinxy(nc,cuts,cutout);

    free(s2);
    free(seg2);
    for( i=0; i<nc; ++i )
       if( cuts[i].seg != NULL )
         { printf(" WANDER ??? 3 CUTS  pol_cutyLTl\n"); exit(1); }
    free(cuts);
    }

 static float  cminx, cminy, cmaxx, cmaxy;
 static POLOUT pol_done;

 static void
 pol_cutBottom( int n, float minx,float miny,float maxx,float maxy, float *pol )
    {
    pol_cutyLTl(n,minx,miny,maxx,maxy,pol,cminy,pol_done);
    }

 static void
 pol_cutLeft( int n, float minx,float miny,float maxx,float maxy, float *pol )
    {
    pol_cutxLTl(n,minx,miny,maxx,maxy,pol,cminx,pol_cutBottom);
    }

 static void
 pol_cutRight( int n, float minx,float miny,float maxx,float maxy, float *pol )
    {
    pol_cutxGTl(n,minx,miny,maxx,maxy,pol,cmaxx,pol_cutLeft);
    }

 static void
 pol_cutTop( int n, float minx,float miny,float maxx,float maxy, float *pol )
    {
    pol_cutyGTl(n,minx,miny,maxx,maxy,pol,cmaxy,pol_cutRight);
    }

 extern void
 pol_cutBox( int n,
             float minx,float miny,float maxx,float maxy,
             float *pol,
             float iminx, float iminy, float imaxx, float imaxy,
             POLOUT done )
    {
    if( maxx <= iminx ) return;
    if( maxy <= iminy ) return;
    if( minx >= imaxx ) return; 
    if( miny >= imaxy ) return;

    cminx    = iminx;
    cminy    = iminy;
    cmaxx    = imaxx;
    cmaxy    = imaxy;
    pol_done = done;

    pol_cutTop( n, minx,miny,maxx,maxy, pol );
    }
