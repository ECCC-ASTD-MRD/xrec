/*
 *
 *  file      :  DLB.C
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
 *               MANAGING DYNAMIC LIBRARIES
 *
 *
 */

#include <stdio.h>
#include <string.h>

#include "dlb.h"

/*
 *  HP-UX definition
 */

#ifdef HP

#include <dl.h>

#define DLOAD(h,p)   h = (void *)shl_load(p,BIND_DEFERRED|BIND_VERBOSE,0L);
#define DFIND(f,h,n) shl_findsym((shl_t*)&h,n,(short)TYPE_PROCEDURE,(void *)&f);
#define DCLOS(h)     shl_unload((shl_t)h);

 static char ext[16] = ".sl";

#else

/*
 *  IRIX AND LINUX definition
 */

#include <dlfcn.h>

#define DLOAD(h,p)   h=dlopen(p,RTLD_LAZY|RTLD_GLOBAL)
#define DFIND(f,h,n) f=dlsym(h,n)
#define DCLOS(h)     dlclose(h)

 static char ext[16] = ".so";

#endif


/*
 *
 *  module    :  DLB_LIBRARY
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  void *handle;
 *               char *path;
 *               handle = dlb_library( path );
 *
 *  object    :  THIS MODULE OPENS THE SHARED LIBRARY GIVEN BY PATH
 *
 *  note      :  THE EXTENSION MUST NOT BE GIVEN... BECAUSE IT IS
 *               PLATEFORM DEPENDANT THE PACKAGE ASSUMES IT FOR PORTABILITY
 *
 */

 extern void *
 dlb_library(char *path )
   {
   char  epath[512];
   void *handle;

   strcpy(epath,path);
   strcat(epath,ext);
   epath[strlen(epath)] = '\0';
   DLOAD(handle,epath);

   return(handle);
   }


/*
 *
 *  module    :  DLB_MODULE
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  void *module;
 *               void *handle;
 *               char *name;
 *               handle = dlb_library( path );
 *
 *  object    :  THIS MODULE FINDS IN THE SHARED LIBRARY POINTED BY handle
 *               A MODULE NAMED name.
 *
 */

 extern void *
 dlb_module( void *handle, char *name )
   {
   void *fptr;

   DFIND(fptr,handle,name); 

   return(fptr);
   }



/*
 *
 *  module    :  DLB_CLOSE
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  void *handle;
 *               dlb_close( handle );
 *
 *  object    :  THIS MODULE CLOSED A THE SHARED LIBRARY POINTED BY handle
 *
 */

 extern void
 dlb_close(void *handle )
   {
   DCLOS(handle);
   }
