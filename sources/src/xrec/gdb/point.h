/*
 *
 *  file      :  POINT.H
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
 *               NEEDED FOR POINTS
 *
 *
 */

#ifndef include_POINT
#define include_POINT

#include "declare.h"

 __BEGIN_DECLS

#define pnt_EQ(x,y,a,b)       ( x == a && y == b )
#define pnt_distance(x,y,a,b) (x-a)*(x-a)+(y-b)*(y-b)

 extern int    pnt_almostBOX( float, float );
 extern int    pnt_almostEQ ( float, float, float, float );

 extern float  pnt_frmbox   ( float, float );

 extern int    pnt_BOX      ( float, float );

 extern void   pnt_setbox   ( float, float, float, float );
 extern void   pnt_setseuil ( float  );

 __END_DECLS

#endif /* include_POINT */
