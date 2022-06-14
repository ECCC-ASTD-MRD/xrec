/*
 *
 *  file      :  POINT.C
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
 *               MANAGING POINTS
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "point.h"

#define   EQ(a,b) (fabs((double)(a-b))<=seuil)
#define   NE(a,b) (fabs((double)(a-b)) >seuil)

 static double seuil = 0.000001;

 static float lat1  =  -90.0;
 static float lon1  = -180.0;
 static float lat2  =   90.0;
 static float lon2  =  180.0;


/*
 *
 *  module    :  PNT_ALMOSTBOX
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
 *               float x;
 *               float y;
 *               pnt_almostBOX( x,y );
 *
 *  object    :  THIS MODULE CHECK IF A POINT IS ALMOST ON THE BOX
 *
 */

 extern int
 pnt_almostBOX( float x, float y )
    {
    int i;

    i = pnt_BOX(x,y);
    if( i ) return(i);

    i = pnt_almostEQ(x,lon1,lat1,lon1) ||
        pnt_almostEQ(x,lon2,lat2,lon2) ||
        pnt_almostEQ(lat1,y,lat1,lon1) ||
        pnt_almostEQ(lat2,y,lat2,lon2)  ;

    return(i);
    }


/*
 *
 *  module    :  PNT_ALMOSTEQ
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
 *               float x1;
 *               float y1;
 *               float x2;
 *               float y2;
 *               pnt_almostEQ( x1,y1,x2,y2 );
 *
 *  object    :  THIS MODULE CHECK IF 2 POINTS A EQUAL
 *
 */

 extern int
 pnt_almostEQ ( float x1, float y1, float x2, float y2 )
    {
    int i;

/*
 *  latitude differ enough
 */
    i = NE(x1,x2);
    if( i ) return(!i);

/*
 *  latitude and longitude are the same enough
 */
    i = EQ(y1,y2);
    if( i ) return(i);

/*
 *  longitude differ : check if it is because of the world wraping at 180 ?
 */
    i = ( EQ(180.0,fabs((double)y1)) && EQ(180.0,fabs((double)y2)) );
    if( !i ) return(i);

/*
 *  longitude in the world wraping (180) check if they are the same enough
 */
    i = ( 360.0 - fabs((double)y1) - fabs((double)y2) <= seuil );
    return(i);
    }


/*
 *
 *  module    :  PNT_BOX
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
 *               float x;
 *               float y;
 *               pnt_BOX( x,y );
 *
 *  object    :  THIS MODULE CHECK IF A POINT IS EXACTLY ON THE BOX
 * 
 *  note      :  A POINT OUTSIDE THE BOX IS CONSIDER ON THE BOX
 *
 */

 extern int
 pnt_BOX( float x, float y )
    {
    return( x <= lat1 || x >= lat2 || y <= lon1 || y >= lon2 );
    }


/*
 *
 *  module    :  PNT_FRMBOX
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
 *               float x;
 *               float y;
 *               pnt_frmbox( x,y );
 *
 *  object    :  THIS MODULE COMPUTES THE DISTANCE BETWEEN
 *               ONE LATLON COORDINATES AND CLOSEST BOX EDGE
 *
 */

 extern float
 pnt_frmbox ( float x, float y )
    {
    float d;
    float min;

    if( pnt_BOX(x,y) ) return(0.0);

    min = 4.0e7;
    
    d = pnt_distance ( x,lon1,lat1,lon1 ); if( d < min ) min = d;
    d = pnt_distance ( x,lon2,lat2,lon2 ); if( d < min ) min = d;
    d = pnt_distance ( lat1,y,lat1,lon1 ); if( d < min ) min = d;
    d = pnt_distance ( lat2,y,lat2,lon2 ); if( d < min ) min = d;
 
    return(min);
    }


/*
 *
 *  module    :  PNT_SETBOX
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
 *               float  x1;
 *               float  y1;
 *               float  x2;
 *               float  y2;
 *               pnt_setbox( x1,y1,x2,y2 );
 *
 *  object    :  THIS MODULE SETS THE TILE BOX
 *
 */

 extern void
 pnt_setbox ( float x1, float y1, float x2, float y2 )
    {
/*
 *  checking inputs
 */
    if( x1<  -90.0 || x1>  90.0 ){printf(" error pnt_setbox x1 \n"); exit(1);}
    if( x2<  -90.0 || x2>  90.0 ){printf(" error pnt_setbox x2 \n"); exit(1);}
    if( y1< -180.0 || y1> 180.0 ){printf(" error pnt_setbox y1 \n"); exit(1);}
    if( y2< -180.0 || y2> 180.0 ){printf(" error pnt_setbox y2 \n"); exit(1);}

    if( x1 >= x2 ) { printf(" error pnt_setbox x1,x2 \n"); exit(1); }
    if( y1 >= y2 ) { printf(" error pnt_setbox y1,y2 \n"); exit(1); }

/*
 * setting the box
 */
    lat1 = x1;
    lon1 = y1;
    lat2 = x2;
    lon2 = y2;
    }


/*
 *
 *  module    :  PNT_SETSEUIL
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
 *               float s;
 *               pnt_setseuil( s );
 *
 *  object    :  THIS MODULE SETS THE THRESHOLD FOR COMPARAISON
 *
 */

 extern void
 pnt_setseuil ( float s )
    {
/*
 *  checking inputs
 */
    if( s < 0.0 || s > 1.0 ) { printf(" error pnt_setseuil x1 \n"); exit(1); }

/*
 * setting the threshold
 */
    seuil = (double)s;
    }


#ifdef   TEST
/*
 *
 *  module    :  TEST_POINT
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  test_point( )
 *
 *  object    :  THIS MODULE TEST ALL PREVIOUS MODULES
 *
 */

 extern void
 test_point()
    {
    if( ! EQ( 64.3, 64.3 ) ) printf(" error EQ\n");
    if( ! NE( 64.3, 30.4 ) ) printf(" error NE\n");

    if(   pnt_EQ( 64.3, 30.4, 67.3, 40.4 ) ) printf(" error pnt_EQ 1\n");
    if( ! pnt_EQ( 64.3, 30.4, 64.3, 30.4 ) ) printf(" error pnt_EQ 2\n");

    pnt_setbox( -65.0, 100.0, -60.0, 105.0 );
    if( ! pnt_BOX( -65.0, 103.0 ) ) printf(" error pnt_BOX 1\n");
    if( ! pnt_BOX( -63.0, 100.0 ) ) printf(" error pnt_BOX 2\n");
    if( ! pnt_BOX( -60.0, 103.0 ) ) printf(" error pnt_BOX 3\n");
    if( ! pnt_BOX( -63.0, 105.0 ) ) printf(" error pnt_BOX 4\n");

    pnt_setseuil( 0.000001 );
    if( ! pnt_almostEQ( -5.0,3.0,-5.0,3.0 ) ) printf(" error pnt_almostEQ 1\n");
    if(   pnt_almostEQ( -5.0,3.0,-4.0,3.0 ) ) printf(" error pnt_almostEQ 2\n");
    if(   pnt_almostEQ( -5.0,3.0,-5.0,4.0 ) ) printf(" error pnt_almostEQ 3\n");
    pnt_setseuil( 0.5 );
    if( ! pnt_almostEQ( -5.0,3.0,-4.7,3.0 ) ) printf(" error pnt_almostEQ 4\n");
    if( ! pnt_almostEQ( -5.0,3.0,-5.3,3.0 ) ) printf(" error pnt_almostEQ 5\n");
    if( ! pnt_almostEQ( -5.0,3.0,-5.0,3.3 ) ) printf(" error pnt_almostEQ 6\n");
    if( ! pnt_almostEQ( -5.0,3.0,-5.0,2.7 ) ) printf(" error pnt_almostEQ 7\n");

    if(!pnt_almostEQ( 3.0,-180.0,3.0,179.7) ) printf(" error pnt_almostEQ 8\n");
    if(!pnt_almostEQ( 3.0,180.0,3.0,-179.7) ) printf(" error pnt_almostEQ 9\n");

    pnt_setseuil( 0.000001 );
    if(!pnt_almostEQ( 3.0,-180.0,3.0,179.999999) )
                                              printf(" error pnt_almostEQ a\n");
    if(!pnt_almostEQ( 3.0,180.0,3.0,-179.999999) )
                                              printf(" error pnt_almostEQ b\n");

    pnt_setbox( -65.0, 100.0, -60.0, 105.0 );
    pnt_setseuil( 0.0001 );
    if( ! pnt_almostBOX( -65.0, 103.0 ) ) printf(" error pnt_almostBOX 1\n");
    if( ! pnt_almostBOX( -63.0, 100.0 ) ) printf(" error pnt_almostBOX 2\n");
    if( ! pnt_almostBOX( -60.0, 103.0 ) ) printf(" error pnt_almostBOX 3\n");
    if( ! pnt_almostBOX( -63.0, 105.0 ) ) printf(" error pnt_almostBOX 4\n");

    pnt_setbox( -65.0, 100.0, -60.0, 105.0 );
    pnt_setseuil( 0.5 );
    if( ! pnt_almostBOX( -65.3, 103.0 ) ) printf(" error pnt_almostBOX 1\n");
    if( ! pnt_almostBOX( -64.7, 103.0 ) ) printf(" error pnt_almostBOX 2\n");
    if( ! pnt_almostBOX( -63.0, 104.7 ) ) printf(" error pnt_almostBOX 3\n");
    if( ! pnt_almostBOX( -63.0, 105.3 ) ) printf(" error pnt_almostBOX 4\n");
    if( ! pnt_almostBOX( -60.3, 103.0 ) ) printf(" error pnt_almostBOX 5\n");
    if( ! pnt_almostBOX( -59.7, 103.0 ) ) printf(" error pnt_almostBOX 6\n");
    if( ! pnt_almostBOX( -63.0,  99.7 ) ) printf(" error pnt_almostBOX 7\n");
    if( ! pnt_almostBOX( -63.0, 100.3 ) ) printf(" error pnt_almostBOX 8\n");

    pnt_setbox( -65.0, -180.0, -60.0, -175.0 );
    if( ! pnt_almostBOX( -63.0, 179.8 ) ) printf(" error pnt_almostBOX 9\n");
    if( ! pnt_almostBOX( -63.0,-179.8 ) ) printf(" error pnt_almostBOX a\n");
    pnt_setbox( -65.0,  175.0, -60.0,  180.0 );
    if( ! pnt_almostBOX( -63.0, 179.8 ) ) printf(" error pnt_almostBOX b\n");
    if( ! pnt_almostBOX( -63.0,-179.8 ) ) printf(" error pnt_almostBOX c\n");

    {
    float qlat =  46.802071;
    float qlon = -71.244926;

    float mlat =  45.541019;
    float mlon = -73.653526;

    if( pnt_distance(mlat,mlon,qlat,qlon) !=  232492.468750 )
        printf(" error pnt_distance mtl que \n" );
    if( pnt_distance(qlat,qlon,mlat,mlon) !=  232492.468750 )
        printf(" error pnt_distance que mtl \n" );
    }
   }

/*  main(){ test_point(); } */

#endif /* TEST */

