/*
 *
 *  file      :  SCNIO.C
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
 *               FOR SERVER-CLIENT AND NATIVE FILE IO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "scnio.h"
#include "sock.h"
#include "swp.h"


 static int scn_srvswap = 0;

/*
 *  action supported
 */

#define SCN_EXIT  1
#define SCN_BOX   2
#define SCN_CLOSE 3
#define SCN_FGETS 4
#define SCN_OPEN  5
#define SCN_READ  6
#define SCN_SEEK  7
#define SCN_SKRD  8

/*
 *  server port string
 */

 static char scn_srv_port[256];

/*
 *  struct of things to remember 
 */

 typedef struct  { int        comm;
                   fileref_t  fref;
                   void      *mm;
                 } Clt_file;

/*
 *  dispatch module pointers and defaults
 */

 typedef void * (*MOD_BOX)  ( fileref_t fref, int w, int h, int s,
                                          int x, int y, int wx, int hy );
 typedef void      (*MOD_CLOSE)( fileref_t fref );
 typedef int       (*MOD_FGETS)( void *addr, int  nbyte, fileref_t fref   );
 typedef fileref_t (*MOD_OPEN) ( void *input, char *mode, int *nbyte  );
 typedef int       (*MOD_READ) ( void *addr, int  nbyte, fileref_t  fref   );
 typedef void      (*MOD_SEEK) ( fileref_t fref, int offset, int  whence );
 typedef int       (*MOD_SKRD) ( fileref_t fref, int d, int n, long *offs, void *addr );

 extern  void * scn_ntvbox  ( fileref_t fref, int w, int h, int s,
                                          int x, int y, int wx, int hy );
 extern  void        scn_ntvclose( fileref_t fref );
 extern  int         scn_ntvfgets( void *addr, int  nbyte, fileref_t  fref   );
 extern  fileref_t   scn_ntvopen ( void *input, char *mode, int *nbyte  );
 extern  int         scn_ntvread ( void *addr, int  nbyte, fileref_t  fref   );
 extern  void        scn_ntvseek ( fileref_t fref, int offset, int  whence );
 extern  int         scn_ntvskrd ( fileref_t fref, int d, int n, long *offs, void *addr );

 static  MOD_BOX     mod_box   = scn_ntvbox;
 static  MOD_CLOSE   mod_close = scn_ntvclose;
 static  MOD_FGETS   mod_fgets = scn_ntvfgets;
 static  MOD_OPEN    mod_open  = scn_ntvopen;
 static  MOD_READ    mod_read  = scn_ntvread;
 static  MOD_SEEK    mod_seek  = scn_ntvseek;
 static  MOD_SKRD    mod_skrd  = scn_ntvskrd;


/*
 *
 *  module    :  SCN_...
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THESE MODULES ARE DISPATCH FOR OVERALL PORTABILITY
 */

 extern void * scn_box   ( fileref_t fref, int w, int h, int s,
                                     int x, int y, int wx, int hy )
                         { return( (*mod_box)(fref,w,h,s,x,y,wx,hy) ); }

 extern void   scn_close ( fileref_t fref ) 
                         { (*mod_close)(fref); }

 extern int    scn_fgets ( void * addr, int nbyte, fileref_t fref )
                         { return( (*mod_fgets)( addr, nbyte, fref ) ); }

 extern fileref_t scn_open ( void *addr, char *mode, int *nbyte )
                         { return( (*mod_open)( addr, mode, nbyte ) ); }

 extern int    scn_read  ( void *addr, int nbyte, fileref_t fref )
                         { return( (*mod_read)( addr, nbyte, fref ) ); }

 extern void   scn_seek  ( fileref_t fref, int offset, int  whence )
                         { (*mod_seek)( fref, offset, whence ); }

 extern int    scn_skrd  ( fileref_t   fref, int d, int n, int64_t *offs, void *addr )
                         { return( (*mod_skrd)( fref, d, n, offs, addr ) ); }


/*
 *
 *  module    :  SCN_CLT...
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THESES MODULES ARE THE CLIENT SIDE OF THE CALLS
 */

 extern void *
 scn_cltbox  ( fileref_t fref, int w, int h, int s,
                         int x, int y, int wx, int hy )
    {
    char     *buf;
    int       comm;
    int       nbyte;
    int       tot;
    int64_t   v[9];
    Clt_file *cf;

    tot = wx * hy *s;
    buf = (char *) malloc ( tot * sizeof(char) );
    if( buf == NULL ) return((void *)NULL);

    cf   = (Clt_file *)fref;
    comm = cf->comm;

    v[0] = SCN_BOX;
    v[1] = cf->fref;
    v[2] = w;
    v[3] = h;
    v[4] = s;
    v[5] = x;
    v[6] = y;
    v[7] = wx;
    v[8] = hy;

    if( swrite( comm, (char *) v,     36 ) < 0 ){ free(buf); return(0); }

    if( sread ( comm, (char *)&nbyte,  4 ) < 0 ){ free(buf); return(0); }
    if( nbyte == 0 )                            { free(buf); return(0); }
    if( sread ( comm, buf,         nbyte ) < 0 ){ free(buf); return(0); }

    return(buf);
    }

 extern void
 scn_cltclose( fileref_t fref ) 
    {
    int       comm;
    void     *mm;
    int64_t   v[3];
    Clt_file *cf;

    cf   = (Clt_file *)fref;
    comm = cf->comm;
    fref = cf->fref;
    mm   = cf->mm;
    free(mm);

    v[0] = SCN_CLOSE;
    v[1] = fref;
    v[2] = SCN_EXIT;

    (void) swrite( comm, (char *)v, 24 );

    sclose( comm );
    }

 extern int
 scn_cltfgets ( void *addr, int nbyte, fileref_t fref )
    {
    int       comm;
    int64_t   v[3];
    Clt_file *cf;

    cf   = (Clt_file *)fref;
    comm = cf->comm;
    fref = cf->fref;

    v[0] = SCN_FGETS;
    v[1] = nbyte;
    v[2] = fref;

    if( nbyte > 1024 ) printf(" Error scn_cltfgets : nbyte > 1024\n");

    if( swrite( comm, (char *)v,      24 ) < 0 ) return(0);

    if( sread ( comm, (char *)&nbyte,  4 ) < 0 ) return(0);

    if( nbyte != 0 ) (void) sread ( comm, (char *)addr, nbyte);

    return(nbyte);
    }

 extern int
 scn_clthandshake(int comm)
    {
    int   siz;

    static char *dlb = "load proto_scnio scn_proto";

/*
 *  the ritual of the any of my client side initialization of the connection
 *  open, sends the endianness, sends the load command length and string
 *  sends again the endianness to the server parser
 */

    siz = strlen(dlb) + 1;
    
    if( swrite( comm, (char *)&SWAP_IS, 1 ) < 0 ) return(-1);
    if( swrite( comm, (char *)    &siz, 4 ) < 0 ) return(-1);
    if( swrite( comm,            dlb, siz ) < 0 ) return(-1);
    if( swrite( comm, (char *)&SWAP_IS, 1 ) < 0 ) return(-1);

    return(0);
    }

 extern fileref_t 
 scn_cltopen ( void *input, char *mode, int *sz )
    {
    char      trsmit[1024];
    int       comm;
    fileref_t fref;
    int       siz;
    int       siz1;
    int       siz2;
    Clt_file *cf;
    char      *path = (char *)input;

    cf = (Clt_file *) malloc ( sizeof(Clt_file) );
    if( cf == NULL ) return(0);
    cf->mm = (void *)cf;

    comm = sopen( scn_srv_port, "c" );
    if( comm < 0 ) { free(cf); return(0); }
    cf->comm = comm;

    if( scn_clthandshake(comm) < 0 )          {sclose(comm);free(cf);return(0);}

    *((int *)&trsmit[0]) = SCN_OPEN;

    siz1 = strlen(path)+1;
    *((int *)&trsmit[4])  = siz1;
    strncpy(&trsmit[8],path,siz1);
    if( swrite( comm, trsmit,    siz1+8 ) < 0){sclose(comm);free(cf);return(0);}

    siz2 = strlen(mode)+1;
    *((int *)&trsmit[0]) = siz2;
    strncpy(&trsmit[4],mode,siz2);
    if( swrite( comm, trsmit,    siz2+4 ) < 0){sclose(comm);free(cf);return(0);}

    if( sread ( comm, (char *)&fref,  8 ) < 0){sclose(comm);free(cf);return(0);}

/*
 *  file could not be open...
 */
    if( fref == 0 )
      {
      *((int *)&trsmit[0]) = SCN_EXIT;
      swrite( comm, trsmit, 4 );
      sclose(comm);
      free(cf);
      return(0);
      }

    cf->fref = fref;

    if( sread ( comm, (char *)&siz,   4 ) < 0){sclose(comm);free(cf);return(0);}
   *sz = siz;

/*
 *  everything all right
 */
    return( (fileref_t)cf );
    }

 extern int
 scn_cltread ( void *addr, int nbyte, fileref_t fref )
    {
    int       comm;
    int64_t   v[3];
    Clt_file *cf;

    cf   = (Clt_file *)fref;
    comm = cf->comm;
    fref = cf->fref;

    v[0] = SCN_READ;
    v[1] = nbyte;
    v[2] = fref;

    if( swrite( comm, (char *)v,      24 ) < 0 ) return(0);

    if( sread ( comm, (char *)addr, nbyte) < 0 ) return(0);
    if( sread ( comm, (char *)&nbyte,  4 ) < 0 ) return(0);

    return(nbyte);
    }

 extern void
 scn_cltseek ( fileref_t fref, int offset, int  whence )
    {
    int       comm;
    int64_t   v[4];
    Clt_file *cf;

    cf   = (Clt_file *)fref;
    comm = cf->comm;

    v[0] = SCN_SEEK;
    v[1] = cf->fref;
    v[2] = offset;
    v[3] = whence;

    if( swrite( comm, (char *)v, 32 ) < 0 ) return;
    }

 extern int
 scn_cltskrd ( fileref_t fref, int d, int n, int *offset, void *addr )
    {
    int       comm;
    int64_t   v[4];
    Clt_file *cf;

    cf   = (Clt_file *)fref;
    comm = cf->comm;

    v[0] = SCN_SKRD;
    v[1] = cf->fref;
    v[2] = d;
    v[3] = n;

    if( swrite( comm, (char *)v,       32 ) < 0 ) return(0);
    if( swrite( comm, (char *)offset, n*4 ) < 0 ) return(0);

    if( sread ( comm, (char *)addr,   n*d ) < 0 ) return(0);
    if( sread ( comm, (char *)&n,       4 ) < 0 ) return(0);

    return(n);
    }


/*
 *
 *  module    :  SCN_NTV...
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THESES MODULES EMULATE THE NATIVE C CALLS
 */

 extern void *
 scn_ntvbox( fileref_t fref, int w, int h, int s, int x, int y, int wx, int hy )
    {
    char *b, *buf;
    int   incr,j;
    int   pos,nline;

    if( y+hy-1 > h ) return((void *)NULL);

    buf = (char *) malloc ( wx * hy * s );
    if( buf == NULL ) return( (void *)NULL );

    b     = buf;
    incr  = s*w;
    nline = wx*s;
    pos   = ( y * w + x ) * s;

    for( j=0; j<hy; j++ )
       {
       scn_ntvseek(fref, pos, SEEK_SET );
       if( scn_ntvread( b, nline, fref ) != nline )
         { free(buf); return( (void *)NULL ); }
       pos += incr;
       b   += nline;
       }

    return(buf);
    }

 extern void
 scn_ntvclose( fileref_t fref ) 
    { FILE *fp = (FILE *)fref; fclose(fp);  }

 extern int
 scn_ntvfgets( void *addr, int nbyte, fileref_t fref  )
    { char *t; t=fgets((char *)addr,nbyte,(FILE *)fref); return(t==NULL ?0:nbyte); }

 extern fileref_t
 scn_ntvopen ( void *input, char *mode, int *sz )
    {
    FILE *fp;
    struct stat buf;
    char *path = (char *)input;
    
    if( stat(path,&buf) != 0 ) return(0);
    *sz = (int) buf.st_size;
    fp = fopen(path,mode);
    return( (fileref_t)fp );
    }

 extern int
 scn_ntvread ( void *addr, int nbyte, fileref_t fref  )
    { return( fread( addr, 1, nbyte, (FILE *)fref ) ); }

 extern void
 scn_ntvseek ( fileref_t fref, int offset, int  whence )
    { fseek( (FILE *)fref, offset, whence ); }

 extern int
 scn_ntvskrd ( fileref_t fref, int d, int n, long *offset, void *addr )
    { int i,k;
      FILE *fp = (FILE *)fref;
      char *p = (char *)addr;
      for( i=0; i<n; ++i)
         {
         fseek( fp, offset[i], SEEK_SET );
         k = fread( &p[i*d], 1, d, fp );
         if( k != d ) return(0);
         }
     return(n);
     }


/*
 *
 *  module    :  SCN_SRV...
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THESES MODULES ARE THE SERVER SIDE OF THE CALLS
 */

 extern void *
 scn_srvbox  ( fileref_t fref, int w, int h, int s,
                         int x, int y, int wx, int hy )
    {
    char     *buf;
    int       comm;
    int       nbyte;
    int       tot;
    int64_t   v[8];

    comm = fref;

    if( sread ( comm, (char *) v,     64 ) < 0 ) return(0);

    if( scn_srvswap )
      {
      swap_4(v[0]);
      swap_4(v[1]);
      swap_4(v[2]);
      swap_4(v[3]);
      swap_4(v[4]);
      swap_4(v[5]);
      swap_4(v[6]);
      swap_4(v[7]);
      }

    fref = v[0];
    w    = v[1];
    h    = v[2];
    s    = v[3];
    x    = v[4];
    y    = v[5];
    wx   = v[6];
    hy   = v[7];

    nbyte = wx * hy * s;
    buf = (char *)scn_ntvbox(fref, w, h, s, x, y, wx, hy );
    if( buf == NULL ) nbyte = 0 ;

    tot = nbyte;
    if( scn_srvswap )   swap_4(tot);
    if( swrite( comm, (char *)&tot,  4 ) < 0 ){ free(buf); return(0); }
    if( nbyte == 0 )                          { free(buf); return(0); }

    if( swrite ( comm, buf, nbyte) < 0 ) return(0);

    free(buf);
    return(0);
    }

 extern void
 scn_srvclose( fileref_t vcomm ) 
    {
    fileref_t    fref;
    int          comm = (int)vcomm;

    if( sread ( comm, (char *)&fref, 8 ) < 0 ) return; 
    if( scn_srvswap )   swap_8x(fref);

    scn_ntvclose( fref );
    }

 extern int
 scn_srvfgets ( void *addr, int nbyte, fileref_t fref )
    {
    char     buf[1024];
    int      comm;
    int64_t  v[2], i64;

    i64 = (int64_t)addr;
    comm = i64 & 0xffffffff;

    if( sread ( comm, (char *)v, 16 ) < 0 ) return(0); 
    if( scn_srvswap )
      {
      swap_4(v[0]);
      swap_4(v[1]);
      }

    nbyte = v[0];
    fref  = v[1];

    nbyte = scn_ntvfgets(buf,nbyte,fref);
    if( scn_srvswap ) swap_4(nbyte);

    if( swrite( comm, (char *)&nbyte, 4 ) < 0 ) return(0);

    if( nbyte != 0 ) (void) swrite( comm, buf, nbyte );

    return(nbyte);
    }

 extern fileref_t
 scn_srvopen ( void *input, char *mode, int *sz )
    {
    char       pth[256];
    char       mde[256];
    int64_t    v[2], v64;
    int        comm;
    fileref_t  fref;
    int    siz;

#ifdef HP
    mode = NULL;/* useless compiler warning shut up */
    *sz  = 0;   /* useless compiler warning shut up */
#endif

    v64 = (int64_t)input;
    comm = (int)v64 & 0xffffffff;

    if( sread ( comm, (char *)&siz,  4 ) < 0 ) return(0);
    if( scn_srvswap )   swap_4(siz);
    if( sread ( comm,         pth, siz ) < 0 ) return(0); 

    if( sread ( comm, (char *)&siz,  4 ) < 0 ) return(0);
    if( scn_srvswap )   swap_4(siz);
    if( sread ( comm,         mde, siz ) < 0 ) return(0); 

    fref = scn_ntvopen( pth, mde, &siz );

    v[0] = fref;
    v[1] = siz;

    if( scn_srvswap )
      {
      swap_4(v[0]);
      swap_4(v[1]);
      }

    if( swrite( comm, (char *)v, 8 ) < 0 ) return(0);

    return(fref);
    }

 extern int
 scn_srvread ( void *addr, int nbyte, fileref_t fref )
    {
    char buf[1024];
    int  comm;
    int  ok;
    int  remain;
    int  ret;
    int  size;
    int64_t  v[2], i64;

    i64 = (int64_t)addr;
    comm = (int)i64 & 0xffffffff;

    if( sread ( comm, (char *)v, 16 ) < 0 ) return(0); 

    if( scn_srvswap )
      {
      swap_4(v[0]);
      swap_4(v[1]);
      }

    nbyte  = v[0];
    fref   = v[1];

    ok     = 1;
    remain = nbyte;

    while( remain > 0 )
         {
         size = ( remain >= 1024 ? 1024 : remain );
         if( ok  ==    1 ) ret = scn_ntvread(buf,size,fref);
         if( ret != size ) ok  = 0;
         if( swrite(comm,buf,size) < 0 ) return(0);
         remain -= size;
         }

    size = ( ok == 1 ? nbyte : 0 );

    if( scn_srvswap )   swap_4(size);
    if( swrite( comm, (char *)&size, 4 ) < 0 ) return(0); 

    return(nbyte);
    }

 extern void
 scn_srvseek ( fileref_t fref, int offset, int  whence )
    {
    int64_t comm;
    int64_t v[3];

    comm = fref;

    if( sread( comm, (char *)v, 24 ) < 0 ) return;

    if( scn_srvswap )
      {
      swap_4(v[0]);
      swap_4(v[1]);
      swap_4(v[2]);
      }

    fref   = v[0];
    offset = v[1];
    whence = v[2];

    scn_ntvseek( fref, offset, whence );
    }

 extern int
 scn_srvskrd ( fileref_t fref, int d, int n, long *offset, void *addr )
    {
    int       i;
    int64_t   comm;
    int64_t   v[4];

    comm = fref;

    if( sread( comm, (char *)v, 24 ) < 0 ) return(0);

    if( scn_srvswap )
      {
      swap_8x(v[0]);
      swap_8x(v[1]);
      swap_8x(v[2]);
      }

    fref   = v[0];
    d      = v[1];
    n      = v[2];

    offset = (long *)  malloc ( n * sizeof(long) );
    addr   = (void *) malloc ( n * d );

    if( sread( comm, (char *)offset, n*4 ) < 0 )
      { free(addr); free(offset); return(0); }

    if( scn_srvswap )
      {
      for( i=0; i<n; ++i )
         { swap_8(offset[i]); }
      }

    i = scn_ntvskrd( fref, d, n, offset, addr );
    if( i == 0 ) { free(addr); free(offset); return(0); }

    if( swrite( comm, (char *)addr,   n*d ) < 0 )
      { free(addr); free(offset); return(0); }
    if( swrite( comm, (char *)&n,       4 ) < 0 )
      { free(addr); free(offset); return(0); }

    free(addr);
    free(offset);

    return(n);
    }


/*
 *
 *  module    :  SCN_TYPE
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int   type;
 *               char *str;
 *               scn_type( type, str );
 *
 *  object    :  THIS MODULE SETS THE IO TYPE
 */

 extern void
 scn_type( int itype, char *str )
    {
    switch(itype)
       {
       case SCN_CLIENT :
                         mod_box   = (MOD_BOX)   scn_cltbox;
                         mod_close = (MOD_CLOSE) scn_cltclose;
                         mod_fgets = (MOD_FGETS) scn_cltfgets;
                         mod_open  = (MOD_OPEN)  scn_cltopen;
                         mod_read  = (MOD_READ)  scn_cltread;
                         mod_seek  = (MOD_SEEK)  scn_cltseek;
                         mod_skrd  = (MOD_SKRD)  scn_cltskrd;
                         strcpy(scn_srv_port,str);
                         break;

       case SCN_NATIVE : 
                         mod_box   = (MOD_BOX)   scn_ntvbox;
                         mod_close = (MOD_CLOSE) scn_ntvclose;
                         mod_fgets = (MOD_FGETS) scn_ntvfgets;
                         mod_open  = (MOD_OPEN)  scn_ntvopen;
                         mod_read  = (MOD_READ)  scn_ntvread;
                         mod_seek  = (MOD_SEEK)  scn_ntvseek;
                         mod_skrd  = (MOD_SKRD)  scn_ntvskrd;
                         break;

       case SCN_SERVER : 
                         mod_box   = (MOD_BOX)   scn_srvbox;
                         mod_close = (MOD_CLOSE) scn_srvclose;
                         mod_fgets = (MOD_FGETS) scn_srvfgets;
                         mod_open  = (MOD_OPEN)  scn_srvopen;
                         mod_read  = (MOD_READ)  scn_srvread;
                         mod_seek  = (MOD_SEEK)  scn_srvseek;
                         mod_skrd  = (MOD_SKRD)  scn_srvskrd;
                         break;

       default         : printf(" Error scn_mode : bad mode\n");
                         break;
       }
    }



/*
 *
 *  module    :  SCN_PROTO
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int i;
 *               i = scn_proto( comm );
 *
 *  object    :  THE SERVER... 
 *               ALL MY SERVERS STARTS BY READING THE ENDIANNESS OF THE
 *               CLIENT THEN AN ENDLESS LOOP IS STARTED TREATING ALL
 *               THE CLIENT REQUESTS
 */

 extern int
 scn_proto(int comm)
    {
    int           act;
    unsigned char client;
    static   int  fin = 0;
/*
#ifdef HP
    unsigned long *t = swap_ptr; * useless compiler warning shut up *
#endif
*/

/*
 *  sets scnio to server
 */
    scn_type( SCN_SERVER, NULL );

/*
 *  read endianness of client
 */
    if( sread( comm, (char *)&client, 1 ) < 0 ) return(-1);
    if( SWAP_IS != client ) scn_srvswap = 1;

/*
 *  endless loop to interpret and take action 
 *  on client requests
 */
    while(1)
         {
/*
 *  read action size and number
 */
         if( sread( comm, (char *)&act, 4 ) < 0 ) break;
         if( scn_srvswap ) swap_4(act);

/*
 *  treat accordingly
 */
         switch( act )
            {
            case SCN_BOX   : scn_box( comm, 0,0,0,0,0,0,0 );
                             break;

            case SCN_CLOSE : scn_close( comm );
                             break;

            case SCN_FGETS : scn_fgets( (void *)((int64_t)comm), 0, 0 );
                             break;

            case SCN_OPEN  : scn_open( (void *)((int64_t)comm), (char *)NULL, (int *)NULL);
                             break;

            case SCN_READ  : scn_read( (void *)((int64_t)comm), 0, 0 );
                             break;

            case SCN_SEEK  : scn_seek( comm, 0, 0 );
                             break;

            case SCN_SKRD  : scn_skrd( comm, 0, 0, (int64_t *)NULL, (char *)NULL );
                             break;

            case SCN_EXIT  : fin = 1;
                             break;
            default : printf(" action = inconnu\n");
                             break;
            }

         if( fin == 1 ) break;
         }

   return(0);
   }
