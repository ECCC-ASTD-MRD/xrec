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
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "rgis.h"
#include "swp.h"

#define  DEBUG   0

#define  COPY_LL(d,s)  strncpy((d)->magic, (s)->magic, 4 ); \
	    strncpy((d)->version, (s)->version, 4 ); \
	    strncpy((d)->datanam, (s)->datanam, 4 ); \
	    (d)->type = (s)->type; \
	    (d)->category = (s)->category; \
	    (d)->value = (s)->value; \
	    (d)->status = (s)->status; \
	    (d)->n = (s)->n; \
	    (d)->ll = NULL; \
	    (d)->data = NULL
	    
	    


 static void * rgis_update64le ( char *info );
 static void * rgis_update32be ( char *info, int do_swap );
 static void * rgis_transfer_header ( void *info, int *do_swap );
 static void * rgis_transfer_header_le ( void *info );

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
                    ll        = (Ll     *) ll;
                    for( i=0; i< ll->n; ++i )
                    printf(" %f %f \n",ll->ll[2*i],ll->ll[2*i+1]);
                    break;

          case 1  : 
                    lli       = (Lli    *) ll;
                    for( i=0; i< ll->n; ++i )
                         printf(" %f %f %d \n",
                                  lli->ll[2*i],lli->ll[2*i+1],lli->i[i]);
                    break;

          case 2  : 
          case 3  : 
                    llt       = (Llt    *) ll;
                    for( i=0; i< ll->n; ++i )
                         printf(" %f %f %s \n",
                                  llt->ll[2*i],llt->ll[2*i+1],llt->t[i]);
                    break;

          case 4  : 
                    lls       = (Lls    *) ll;
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

#if DEBUG
    fprintf( stderr, "Open gisfile: %s\n", path );
#endif
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
        strncmp( &mem[4], VERSION12, 4 ) != 0 &&
        strncmp( &mem[4], VERSION1, 4 ) != 0 ))
      {
      free(mem);
      return(5);
      }

/*
 *  swap if needed
 */
   if (strncmp( &mem[4], VERSION12, 4 ) == 0 )
      {
      *info = rgis_update64le( mem );
      }
   else
      {
/*      *info = rgis_update32be( mem, SWAP_IS == SWAP_LSBF ); */
      *info = rgis_update32be( mem, 0 );
      }
   if( *info == NULL )
      {
      free(mem);
      return(6);
      }


/*
 *  everything all right
 */
    return( (*info != NULL)? 0 : -1);
    }


/*
 *
 *  module    :  RGIS_update64le
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
 *               gis_update( info, swap );
 *
 *  object    :  THIS MODULE LOAD THE GIS INFORMATION FROM DISK
 *
 */

 static void *
 rgis_update64le ( char *info )
    {
    int       i, j, n, n2;
    Ll       *ll;
    Lli      *lli;
    Llt      *llt;
    Lls      *lls;
    caddr_t   ll_addr, t_addr, tex_addr;
    int32_t  *t_i32;

    ll = (Ll *) rgis_transfer_header_le( info  );
#if DEBUG
fprintf( stderr, "...Little Endian: no swap, type=%d\n", ll->type );
#endif

/*
 *  treat according to the type
 */
    switch( ll->type )
       {
       case 0  :
          ll_addr   = (caddr_t)info;
          ll_addr  += sizeof(__Ll);
          ll->ll    = (float *)ll_addr;
          break;
       case 1  : 
          ll_addr   = (caddr_t)info;
          ll_addr  += sizeof(__Lli);
          lli       = (Lli    *) ll;
          lli->ll   = (float  *) ll_addr;
		    n2        = 2 * n;
		    ll_addr  += n2 * sizeof(float);
		    lli->i    = (int    *) ll_addr;
          break;

       case 2  : 
       case 3  : 
          ll_addr = (caddr_t)info;
          ll_addr += sizeof(__Llt);
          llt       = (Llt    *) ll;
          llt->ll   = (float *)ll_addr;
		    n         = llt->n;
		    n2        = 2*llt->n;
          t_addr    = ll_addr + n2 * sizeof(float);
          tex_addr  = t_addr + n * sizeof(int32_t);
          llt->t    = (char **)malloc( n * sizeof(char *) );
          llt->tex  = (char *)tex_addr;
		    t_i32     = (int32_t *)t_addr;
          for( i=0; i<n ; ++i ) 
             llt->t[i] = &llt->tex[t_i32[i]];
          break;
       case 4  : 
		    ll_addr = (caddr_t)info;
		    ll_addr += sizeof(__Lls);
          lls       = (Lls    *) ll;
		    lls->ll = (float *)ll_addr;
		    n2         = 2*lls->n;
		    ll_addr  += n2 * sizeof(float);
		    lls->sl   = (int    *) ll_addr;
		    ll_addr  += lls->ns * sizeof(int);
		    lls->seg  = (float **)malloc( n2 * sizeof(float *) );

          for( i=0, j=0; i<lls->ns; ++i )
             {
             lls->seg[i] = &lls->ll[j];
             j += 2*lls->sl[i];
             }
          break;

       default : 
	       free( ll );
	       return(NULL);
          }
/*
 *  return
 */
    ll->data = info;
    return(ll);
    }

/*
 *
 *  module    :  RGIS_update32be
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
 *               gis_update32( info, swap );
 *
 *  object    :  THIS MODULE SWAPS THE GIS INFORMATION
 *
 */

 static void *
 rgis_update32be ( char *info, int do_swap )
    {
    int   i,j,n, n2;
    Ll   *ll;
    Lli  *lli;
    Llt  *llt;
    Lls  *lls;
    caddr_t  ll_addr, t_addr, tex_addr;
    int32_t  *t_i32;

    ll = (Ll *) rgis_transfer_header( info, &do_swap );

#if DEBUG
fprintf( stderr, "...Big Endian: need swapping, type=%d\n", ll->type );
#endif
/*
 *  treat according to the type
 */
    switch( ll->type )
       {
       case 0  :
          ll_addr   = (caddr_t)info;
          ll_addr  += sizeof(_Ll);
          ll->ll    = (float *)ll_addr;
                    
		    if (do_swap)
		       {
		       n2 = 2 * ll->n;
             for( i=0; i<n2; ++i ) 
                swap_4x(ll->ll[i]);
		       }
          break;

       case 1  : 
          lli       = (Lli    *) ll;
          ll_addr   = (caddr_t)info;
          ll_addr  += sizeof(_Lli);
          lli->ll   = (float  *) ll_addr;
		    
		    n2        = 2 * n;
		    ll_addr  += n2 * sizeof(float);
		    lli->i    = (int    *) ll_addr;
                    
		    if (do_swap)
		       {
             n  = lli->n;
             for( i=0; i<n2;      ++i ) swap_4x(lli->ll[i]);
             for( i=0; i<n ; ++i ) swap_4 (lli->i [i]);
             }
          break;

       case 2  : 
       case 3  : 
          llt       = (Llt    *) ll;
          ll_addr = (caddr_t)info;
          ll_addr += sizeof(_Llt);
          llt->ll   = (float *)ll_addr;
          n         = llt->n;
          n2        = 2*llt->n;
          t_addr    = ll_addr + n2 * sizeof(float);
          tex_addr  = t_addr + n * sizeof(addr32_t);
          llt->t    = (char **)malloc( n * sizeof(char *) );
          llt->tex  = (char *)tex_addr;
		    t_i32   = (int32_t *)t_addr;
		    if (do_swap)
		       {
             for( i=0; i<n2; ++i ) 
                swap_4x(llt->ll[i]);                    
             for( i=0; i<n ; ++i ) 
                swap_4x(t_i32[i]);
		       }
          for( i=0; i<n ; ++i ) llt->t[i] = &llt->tex[t_i32[i]];
          break;

       case 4  : 
       case 5  : 
          lls       = (Lls    *) ll;
		    ll_addr = (caddr_t)info;
		    ll_addr += sizeof(_Lls);
		    lls->ll = (float *)ll_addr;
		    n2         = 2*lls->n;
		    ll_addr  += n2 * sizeof(float);
		    lls->sl   = (int    *) ll_addr;
		    lls->seg  = (float **)malloc( n2 * sizeof(float *) );

		    if (do_swap)
		       {
             for( i=0; i<n2;   ++i ) swap_4x(lls->ll[i]);
             for( i=0, j=0; i<lls->ns; ++i )
                {
                swap_4x(lls->sl[i]);
                lls->seg[i] = &lls->ll[j];
                j += 2*lls->sl[i];
                }
		       }
		    else 
		       {
             for( i=0, j=0; i<lls->ns; ++i )
                {
                lls->seg[i] = &lls->ll[j];
                j += 2*lls->sl[i];
                }
		       }
          break;

       default : 
	       free( ll );
	       return(NULL);
       }
/*
 *  return
 */
    ll->data = info;
    return(ll);
    }




/*
 *
 *  module    :  RGIS_FREE
 *
 *  author    :  
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  char *info;
 *               rgis_free( info );
 *
 *  object    :  THIS MODULE FREE THE GIS INFORMATION
 *
 */

 void
 rgis_free ( void *info )
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
                    break;

          case 1  : 
                    lli       = (Lli    *) info;
                    break;

          case 2  : 
          case 3  : 
                    llt       = (Llt    *) info;
		    if (llt->t)
		       free( llt->t );
                    break;

          case 4  : 
                    lls       = (Lls    *) info;
		    if (lls->seg)
                       free ( lls->seg );
                    break;
          }

    if (ll->data) free( ll->data );
    free( ll );
    }

/*
 *
 *  module    :  RGIS_transfer_header
 *
 *  author    :  
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  void *info;
 *               rgis_transfer_header( info );
 *
 *  object    :  THIS MODULE TRANSFER THE GIS INFORMATION FROM physical 32 bits 
 *               on DISK TO MEM which could be other than 32 bits.
 *
 */

 static void *
 rgis_transfer_header ( void *info, int *pdo_swap )
    {
    Ll       *ll;
    Lli      *lli;
    Llt      *llt;
    Lls      *lls;
    _Ll      *_ll;
    _Lli     *_lli;
    _Llt     *_llt;
    _Lls     *_lls;
    int32_t   type;
    int      do_swap = *pdo_swap;

/*
 *  treat according to the type
 */
    _ll = (_Ll *)info;
    type = _ll->type;
    if ((type < 0)||(type > 100))
       {
       do_swap = 1;
       }
    else if (type == 0)
       {
       if (_ll->size > (INT_MAX/3*2))
          do_swap = 1;
       }

    if (do_swap)
       {
       swap_4(type);
       }
       
    switch( type )
          {
          case 0  :
                    ll         = (Ll     *)malloc( sizeof(Ll) );
                    _ll        = (_Ll    *) info;
		    COPY_LL( ll, _ll );
          ll->size = _ll->size;
		    if (do_swap) swap_4(ll->size);
          break;

          case 1  : 
                    lli        = (Lli     *)malloc( sizeof(Lli) );
                    _lli       = (_Lli    *) info;
		    COPY_LL( lli, _lli );
          lli->size = _lli->size;
		    if (do_swap) swap_4(lli->size);
		    ll         = (Ll     *)lli;
		    lli->i     = NULL;
                    break;

          case 2  : 
          case 3  : 
                    llt        = (Llt    *)malloc( sizeof(Llt) );
                    _llt       = (_Llt    *) info;
		    COPY_LL( llt, _llt );
          llt->size = _llt->size;
		    if (do_swap) swap_4(llt->size);
		    ll         = (Ll     *)llt;
		    llt->l     = _llt->l;
		    if (do_swap)
		       swap_4( llt->l );
		    llt->t     = NULL;
		    llt->tex   = NULL;
                    break;

          case 4  : 
                    lls        = (Lls    *)malloc( sizeof(Lls) );
                    _lls       = (_Lls    *) info;
		    COPY_LL( lls, _lls );
          lls->size = _lls->size;
		    if (do_swap) swap_4(lls->size);
		    ll         = (Ll     *)lls;
		    lls->ns = _lls->ns;
		    if (do_swap)
		       swap_4(lls->ns);
		    lls->seg   = NULL;
		    lls->sl    = NULL;
                    break;
	  default :
	            return NULL;
          }
          
/*
 *  swap description in header
 */
    if (do_swap)
       {
       ll->type = type;
       swap_4(ll->category);
       swap_4(ll->value);
       swap_4(ll->status);
       swap_4(ll->n);
       }

    *pdo_swap = do_swap;
    return(ll);
    }

/*
 *
 *  module    :  RGIS_transfer_header
 *
 *  author    :  
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  void *info;
 *               rgis_transfer_header_le( info );
 *
 *  object    :  THIS MODULE TRANSFER THE GIS INFORMATION FROM physical 32 bits 
 *               on DISK TO MEM which could be other than 32 bits.
 *
 */

 static void *
 rgis_transfer_header_le ( void *info )
   {
   Ll       *ll;
   Lli      *lli;
   Llt      *llt;
   Lls      *lls;
   __Ll      *__ll;
   __Lli     *__lli;
   __Llt     *__llt;
   __Lls     *__lls;

/*
 *  treat according to the type
 */
   __ll = (__Ll *)info;
       
   switch( __ll->type )
   {
      case 0  :
         ll         = (Ll     *)malloc( sizeof(Ll) );
         __ll        = (__Ll    *) info;
		   COPY_LL( ll, __ll );
         ll->size = __ll->size;
         break;

      case 1  : 
         lli        = (Lli     *)malloc( sizeof(Lli) );
         __lli       = (__Lli    *) info;
		   COPY_LL( lli, __lli );
         lli->size = __lli->size;
		   ll         = (Ll     *)lli;
		   lli->i     = NULL;
         break;

      case 2  : 
      case 3  : 
         llt        = (Llt    *)malloc( sizeof(Llt) );
         __llt       = (__Llt    *) info;
		   COPY_LL( llt, __llt );
         llt->size = __llt->size;
		   ll         = (Ll     *)llt;
		   llt->l     = __llt->l;
		   llt->t     = NULL;
		   llt->tex   = NULL;
         break;

      case 4  : 
         lls        = (Lls    *)malloc( sizeof(Lls) );
         __lls       = (__Lls    *) info;
		   COPY_LL( lls, __lls );
         lls->size = __lls->size;
		   ll         = (Ll     *)lls;
		   lls->ns = __lls->ns;
		   lls->seg   = NULL;
		   lls->sl    = NULL;
         break;
	  default :
	      return NULL;
     }
          
    return(ll);
    }
