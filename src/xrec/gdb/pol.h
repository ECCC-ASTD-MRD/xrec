/*
 *
 *  file      :  POL.H
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
 *               FOR POLYGONE 
 *
 */

#ifndef include_POL
#define include_POL

#include "declare.h"

 __BEGIN_DECLS

 typedef void (*POLOUT)(int, float, float, float, float, float *);

 extern void
 pol_cutxGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT polout );

 extern void
 pol_cutxLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT polout );

 extern void
 pol_cutyGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT polout );

 extern void
 pol_cutyLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pol, float dlimit, POLOUT polout );

 extern void
 pol_cutBox ( int n,
              float minx,float miny,float maxx,float maxy,
              float *pol,
              float iminx, float iminy, float imaxx, float imaxy,
              POLOUT polout );

 __END_DECLS

#endif /* include_POL */
