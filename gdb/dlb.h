/*
 *
 *  file      :  DLB.H
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
 *               NEEDED FOR DYNAMIC LOADING OF LIBRARY
 *
 */

#ifndef include_DLB
#define include_DLB

#include "declare.h"

 __BEGIN_DECLS

 extern void * dlb_library(char *path   );
 extern void * dlb_module (void *handle, char *name   );
 extern void   dlb_close  (void *handle );

 __END_DECLS

#endif /* include_DLB */
