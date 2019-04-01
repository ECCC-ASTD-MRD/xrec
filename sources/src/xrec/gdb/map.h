/*
 *
 *  file      :  MAP.H
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
 *               NEEDED FOR MAP EXTRACTION
 *
 */

#ifndef include_MAP
#define include_MAP

#include "declare.h"

 __BEGIN_DECLS

 extern void *
 map_box( char *path, int w, int h, int s, int x, int y, int wx, int wh );

 extern int map_read( char *path, int s, char **map, int *siz );

 __END_DECLS

#endif /* include_MAP */
