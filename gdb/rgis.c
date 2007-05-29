/*
 *
 *  file      :  RGIS.C
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
 *               READING THE GEOGRAPHICAL INFORMATION SYSTEM
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "rgis.h"
#include "swp.h"

 static int rgis_swaponl( char *info );
 static int rgis_update ( char *info );

 extern void
 rgis_print ( char *info )
    {
    int   i,j;
    Ll   *ll;
    Lli  *lli;
    Llt  *llt;
    Lls  *lls;

/*
 *  print header information
 */
    ll        = (Ll     *) info;
    printf(" magic    : %.4s \n",ll->magic);
    printf(" version  : %.4s \n",ll->version);
    printf(" datanam  : %.4s \n",ll->datanam);
    printf(" size     : %d   \n",ll->size);
    printf(" type     : %d   \n",ll->type);
    printf(" category : %d   \n",ll->category);
    printf(" value    : %d   \n",ll->value);
    printf(" status   : %d   \n",ll->status);
    printf(" n        : %d   \n",ll->n);

/*
 *  treat according to the type
 */
    switch( ll->type )
          {
          case 0  :
                    ll        = (Ll     *) info;
                    for( i=0; i< ll->n; ++i )
                    printf(" %f %f \n",ll->ll[2*i],ll->ll[2*i+1]);
                    break;

          case 1  : 
                    lli       = (Lli    *) info;
                    for( i=0; i< ll->n; ++i )
                         printf(" %f %f %d \n",
                                  lli->ll[2*i],lli->ll[2*i+1],lli->i[i]);
                    break;

          case 2  : 
          case 3  : 
                    llt       = (Llt    *) info;
                    for( i=0; i< ll->n; ++i )
                         printf(" %f %f %s \n",
                                  llt->ll[2*i],llt->ll[2*i+1],llt->t[i]);
                    break;

          case 4  : 
                    lls       = (Lls    *) info;
                    printf(" ns       : %d   \n",lls->ns);
                    for( i=0; i< lls->ns; ++i )
                       {
                       printf(" sl[%d]    : %d   \n",i,lls->sl[i]);
                       for( j=0; j< lls->sl[i]; ++j )
                          printf(" %f %f \n",lls->seg[i][2*j],lls->seg[i][2*j+1]); 
                       }
                    break;

          default : return;
          }
    return;
    }


/*
 *
 *  module    :  RGIS_READS
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  char  *path;
 *               char **info;
 *               rgis_read( path, info );
 *
 *  object    :  THIS MODULE READS THE GIS BINAIRY FILE NAMED path
 *
 */

 extern int
 rgis_reads ( char *path, char **info, int *siz )
    {
    char       *mem;
    int         n;
    FILE       *fp;
    struct stat buf;

/*
 *  initialize
 */
    *info = NULL;
    *siz  = 0;

/*
 *  find file size
 */
    if( stat(path,&buf) != 0 ) return(1); 
    *siz = (int) buf.st_size;

/*
 *  open the files and gets its size
 */
    fp = fopen(path,"r");
    if( fp == NULL ) { *siz = 0; return(2); }

/*
 *  allocate space for gis
 */
    mem = (char *) malloc ( *siz * sizeof(char) );
    if( mem == NULL ) return(3);

/*
 *  read and close the gis file
 */
    n = fread( (void *)mem, 1, *siz, fp );
    fclose(fp);

/*
 *  check reading
 */
    if( n != *siz )
      {
      free(mem);
      return(4);
      }

/*
 *  check if it is realy a gis file
 */
    if( strncmp( &mem[0], MAGIC,    4 ) != 0 ||
      ( strncmp( &mem[4], VERSION,  4 ) != 0 &&
        strncmp( &mem[4], VERSION1, 4 ) != 0 ))
      {
      free(mem);
      return(5);
      }

/*
 *  swap if needed
 */
    if( SWAP_IS == SWAP_LSBF )
      {
      n = rgis_swaponl( mem );
      if( n != 0 )
        {
        free(mem);
        return(6);
        }
      }

/*
 *  else update the pointers
 */
    else
      {
      rgis_update( mem );
      }

/*
 *  everything all right
 */
   *info = mem;
    return(0);
    }


/*
 *
 *  module    :  RGIS_SWAPONL
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  char *info;
 *               gis_swaponl( info );
 *
 *  object    :  THIS MODULE SWAPS THE GIS INFORMATION
 *
 */

 static int
 rgis_swaponl ( char *info )
    {
    int   i,j,n;
    Ll   *ll;
    Lli  *lli;
    Llt  *llt;
    Lls  *lls;

    ll = (Ll *) info;

/*
 *  swap description in header
 */
    swap_4(ll->size);
    swap_4(ll->type);
    swap_4(ll->category);
    swap_4(ll->value);
    swap_4(ll->status);
    swap_4(ll->n);

/*
 *  treat according to the type
 */
    switch( ((Ll *)info)->type )
          {
          case 0  :
                    ll        = (Ll     *) info;
                    ll->ll    = (float  *) &ll[1];
                    n         = 2 * ll->n;
                    for( i=0; i<n; ++i ) swap_4x(ll->ll[i]);
                    break;

          case 1  : 
                    lli       = (Lli    *) info;
                    lli->ll   = (float  *) &lli[1];
                    lli->i    = (int    *) &lli->ll[2*lli->n];
                    n         = 2 * ll->n;
                    for( i=0; i<n;      ++i ) swap_4x(lli->ll[i]);
                    for( i=0; i<lli->n; ++i ) swap_4 (lli->i [i]);
                    break;

          case 2  : 
          case 3  : 
                    llt       = (Llt    *) info;
                    swap_4(llt->l);
                    llt->ll   = (float  *) &llt[1];
                    llt->t    = (char  **) &llt->ll[2*llt->n];
                    llt->tex  = (char   *) &llt->t[llt->n];
                    n         = 2*llt->n;
                    for( i=0; i<n;      ++i ) swap_4x(llt->ll[i]);
                    for( i=0; i<llt->n; ++i ) swap_4x(llt->t[i]);
                    for( i=0; i<llt->n; ++i ) llt->t[i] = &llt->tex[(int)llt->t[i]];
                    break;

          case 4  : 
                    lls       = (Lls    *) info;
                    swap_4(lls->ns);
                    lls->ll   = (float  *) &lls[1];
                    lls->sl   = (int    *) &lls->ll[2*lls->n];
                    lls->seg  = (float **) &lls->sl[lls->ns];
                    n         = 2*lls->n;
                    for( i=0; i<n;   ++i ) swap_4x(lls->ll[i]);
                    for( i=0, j=0; i<lls->ns; ++i )
                       {
                       swap_4x(lls->sl[i]);
                       lls->seg[i] = &lls->ll[j];
                       j += 2*lls->sl[i];
                       }
                    break;

          default : return(3);
          }

/*
 *  return
 */
    return(0);
    }


/*
 *
 *  module    :  RGIS_UPDATE
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  char *info;
 *               rgis_update( info );
 *
 *  object    :  THIS MODULE SWAPS THE GIS INFORMATION
 *
 */

 static int
 rgis_update ( char *info )
    {
    int   i,j;
    Ll   *ll;
    Lli  *lli;
    Llt  *llt;
    Lls  *lls;

    ll = (Ll *) info;

/*
 *  treat according to the type
 */
    switch( ((Ll *)info)->type )
          {
          case 0  :
                    ll        = (Ll     *) info;
                    ll->ll    = (float  *) &ll[1];
                    break;

          case 1  : 
                    lli       = (Lli    *) info;
                    lli->ll   = (float  *) &lli[1];
                    lli->i    = (int    *) &lli->ll[2*lli->n];
                    break;

          case 2  : 
          case 3  : 
                    llt       = (Llt    *) info;
                    llt->ll   = (float  *) &llt[1];
                    llt->t    = (char  **) &llt->ll[2*llt->n];
                    llt->tex  = (char   *) &llt->t[llt->n];
                    for( i=0; i<llt->n; ++i )
                       llt->t[i] = &llt->tex[(int)llt->t[i]];
                    break;

          case 4  : 
                    lls       = (Lls    *) info;
                    lls->ll   = (float  *) &lls[1];
                    lls->sl   = (int    *) &lls->ll[2*lls->n];
                    lls->seg  = (float **) &lls->sl[lls->ns];
                    j = 0;
                    for( i=0; i<lls->ns; ++i )
                       {
                       lls->seg[i] = &lls->ll[j];
                       j += 2*lls->sl[i];
                       }
                    break;
          }

/*
 *  return
 */
    return(0);
    }
