/*
 *
 *  file      :  PLN.H
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
 *               FOR POLYLINE 
 *
 */

#ifndef include_PLN
#define include_PLN

#include "declare.h"

 __BEGIN_DECLS

 typedef void (*PLNOUT)(int, float, float, float, float, float * );

 extern void
 pln_cutxGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT plnout );

 extern void
 pln_cutxLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT plnout );

 extern void
 pln_cutyGTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT plnout );

 extern void
 pln_cutyLTl( int n,
              float minx, float miny, float maxx, float maxy,
              float *pln, float dlimit, PLNOUT plnout );

 extern void
 pln_cutBox ( int n,
              float minx,float miny,float maxx,float maxy,
              float *pln,
              float iminx, float iminy, float imaxx, float imaxy,
              PLNOUT plnout );

 __END_DECLS

#endif /* include_PLN */
