/*
 *
 *  file      :  RGIS.H
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
 *               NEEDED FOR THE DIGITAL CHART OF THE WORLD
 *
 *
 */

#ifndef include_RGIS
#define include_RGIS

#include "declare.h"

 __BEGIN_DECLS

#define MAGIC    "GIS@"
#define VERSION  "V000"
#define VERSION1 "V100"
#define VERSION12 "V120"

/*
 * type for padding the legacy 32 bits pointers space on disk
 */
typedef int32_t  addr32_t;

/*
 *  data type 0 means the structure Ll should be use
 *  under  this type, the structure contains a table
 *  of independant latitude longitude coordinates
 */



 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  float   *ll;
                  caddr_t  data;
                } Ll;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  addr32_t ll;
                } _Ll;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                } __Ll;

/*
 *  data type 1 means the structure Lli should be use
 *  under  this type, the structure contains a table
 *  of independant latitude longitude coordinates
 *  an a table of integer value associated with it
 */

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  float   *ll;
                  caddr_t  data;
                  int     *i;
                } Lli;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  addr32_t ll;
                  addr32_t i;
                } _Lli;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                } __Lli;

/*
 *  data type 2 and 3 means the structure Llt should
 *  be use under this type, the structure contains a
 *  table of independant latitude longitude coordinates
 *  with a table of string pointers associated with it
 *  type 2 means that many positions can une the same
 *  string... type 3 means the string pointer is unique
 *  for all position
 */

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  float   *ll;
                  caddr_t  data;
                  char   **t;
                  int      l;
                  char    *tex;
                } Llt;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  addr32_t ll;
                  addr32_t t;
                  int      l;
                  addr32_t tex;
                } _Llt;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  int      l;
                } __Llt;

/*
 *  data type 4 means the structure Lls should be use
 *  under  this type, the structure contains a table
 *  of independant latitude longitude segments
 *  sl vecteur of segments length (in coord latlon)
 */

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  float   *ll;
                  caddr_t  data;
                  int      ns;
                  int     *sl;
                  float  **seg;
                } Lls;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  addr32_t ll;
                  int      ns;
                  addr32_t sl;
                  addr32_t seg;
                } _Lls;

 typedef struct { char     magic  [4];
                  char     version[4];
                  char     datanam[4];
                  int      size;
                  int      type;
                  int      category;
                  int      value;
                  int      status;
                  int      n;
                  int      ns;
                } __Lls;

 extern void   rgis_print       ( char  * );
 extern int    rgis_reads       ( char  *, char **, int * );
 extern void   rgis_free        ( void * );
 

 __END_DECLS

#endif /* include_RGIS */
