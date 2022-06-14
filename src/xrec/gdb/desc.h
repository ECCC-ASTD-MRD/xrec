/*
 *
 *  file      :  DESC.H
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
 *               NEEDED FOR DBASE DESCRIPTION FILE
 *
 */

#ifndef include_DESC
#define include_DESC

#include "declare.h"

 __BEGIN_DECLS

 typedef struct {
                float bottom;
                float left;
                float top;
                float right;
                int   w;
                int   h;
                int   t;
                float scale;
                } Desc;

 extern void dsc_print( char *d );
 extern int  dsc_read ( char *path, char **d );

 __END_DECLS

#endif /* include_DESC */
