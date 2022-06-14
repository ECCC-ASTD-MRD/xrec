/*
 *
 *  file      :  SEGMENT.H
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
 *               NEEDED FOR SEGMENTS
 *
 *
 */

#ifndef include_SEGMENT
#define include_SEGMENT

#include "declare.h"

 __BEGIN_DECLS

 enum SEG_JOINS {
                  SEG_H2H = 1,
                  SEG_H2T = 2,
                  SEG_T2H = 4,
                  SEG_T2T = 8
                };

 enum SEG_TYPES {
                  SEG_CLOSE = 1,
                  SEG_BOX,
                  SEG_BOX_CONNECT,
                  SEG_CONNECT_BOX,
                  SEG_CONNECT
                };

 extern int    seg_cross     ( float,   float, float, float,
                               float,   float, float, float,
                               float *, float * );
 extern int    seg_crosspt   ( float,   float, float, float,
                               float,   float, float, float,
                               float *, float * );
 extern void   seg_close     ( int,     float *, int *,float ** );
 extern int    seg_closein   ( int,     float *, int,float *,  int *,float ** );
 extern int    seg_connbox   ( int,float *, int,float *,  int );
 extern int    seg_connclose ( int,float *, int,float *,  int );
 extern int    seg_connhow   ( int,float *, int,float *,  int *, float * );
 extern int    seg_cut       ( int,float *, int, int *,float **, int *,float ** );

 extern void   seg_deletept  ( float **, int, int, int * );

 extern int    seg_find      ( float,float, int,float * );
 extern void   seg_frmseg    ( int,float *, int,float *, 
                               float *,int *,float *,int *);

 extern int    seg_insertpt  ( int *,float *, float, float, int,float **);
 extern int    seg_isbox     ( int,float *);
 extern int    seg_isconn    ( int,float *, int,float *,  int * );
 extern int    seg_isclose   ( int,float *);
 extern int    seg_isinto    ( int,float *, int,float *);
 extern int    seg_issubcon  ( int,float *, int,float *,  int * );

 extern void   seg_join      ( int,float *, int,float *, int,  int *,float ** );
 extern void   seg_joinh2h   ( int,float *, int,float *, int *,float ** );
 extern void   seg_joinh2t   ( int,float *, int,float *, int *,float ** );
 extern void   seg_joint2h   ( int,float *, int,float *, int *,float ** );
 extern void   seg_joint2t   ( int,float *, int,float *, int *,float ** );

 extern int    seg_nodup     ( int,float *, int *,float ** );
 extern int    seg_noloop    ( int,float *, int *,float **, int *,float ** );
 extern int    seg_nolooppt  ( int,float *, float, float, int *,float **, int *,float ** );
 extern void   seg_print     ( int,float * );
 extern int    seg_type      ( int,float *, int *);
 extern int    seg_xtract    ( int,float *, int,float *, int, int *,float ** );
 extern int    seg_whichloop ( int,float *, float, float, float, float );

 __END_DECLS

#endif /* include_SEGMENT */
