
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "desc.h"
#include "map.h"
#include "swp.h"

 extern void *
 map_box( char *path, int w, int h, int s, int x, int y, int wx, int hy )
    {
/*
#ifdef HP
    unsigned long *t = swap_ptr; * useless compiler warning shut up *
#endif
*/

    char  *b, *buf;
    FILE  *fp;
    int    i,j,n,incr;
    int    pos,nline;
    short *sp;

    fp = fopen(path,"r");
    if( fp == 0 ) return(NULL);

    if( y+hy-1 > h ) return((void *)NULL);

    buf = (char *) malloc ( wx * hy * s );
    if( buf == NULL ) return( (void *)NULL );

    b     = buf;
    incr  = s*w;
    nline = wx*s;
    pos   = ( y * w + x ) * s;

    for( j=0; j<hy; j++ )
       {
       fseek(fp, pos, SEEK_SET );
       if( fread( b, 1, nline, fp ) != nline )
         { free(buf); return( (void *)NULL ); }
       pos += incr;
       b   += nline;
       }

    fclose(fp);

    if( buf != NULL && s == 2 && SWAP_IS == SWAP_LSBF )
      {
      n  = wx * hy;
      sp = (short *)buf;
      for( i=0; i<n; ++i )
         { swap_2(sp[i]); }
      }

     return((void *)buf);
     }

 extern int
 map_read ( char *path, int s, char **map, int *siz )
    {
    char       *mem;
    int         i,n;
    FILE       *fp;
    struct stat buf;
    short      *sp;

/*
 *  initialize
 */
    *map = NULL;
    *siz = 0;

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
 *  swap if needed
 */
    if( s == 2 && SWAP_IS == SWAP_LSBF )
      {
      sp = (short *)mem;
      n  = *siz/2;
      for( i=0; i<n; ++i )
         { swap_2(sp[i]); }
      }

/*
 *  everything all right
 */
   *map = mem;
    return(0);
    }
