#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desc.h"
#include "scnio.h"

 extern void
 dsc_print( char *d )
    {
    Desc *desc;

    desc = (Desc *) d;

    printf(" BOTTOM  %f \n",desc->bottom);
    printf(" TOP     %f \n",desc->top   );
    printf(" LEFT    %f \n",desc->left  );
    printf(" RIGHT   %f \n",desc->right );
    printf(" WIDTH   %d \n",desc->w     );
    printf(" HEIGHT  %d \n",desc->h     );
    printf(" TILE    %d \n",desc->t     );
    printf(" SCALE   %f \n",desc->scale );
    }

 extern int
 dsc_read( char *path, char **d )
    {
    char  info[256];
    char *file;
    char *line;
    float f;
    fileref_t fp;
    int   i,n;
    int   siz;
    Desc *desc;

    desc = (Desc *) malloc ( sizeof(Desc) );
   *d    = (char *) desc;

    fp = scn_open( path, "r", &siz);
    if( fp == 0 ){ free(desc); return(-1); }

    file = (char *) malloc ( siz * sizeof(char) );
    if( file == NULL ){ scn_close(fp); free(desc); return(-1);}

    if( scn_read( file, siz, fp) != siz )
                      { scn_close(fp); free(desc); return(-1);}
    scn_close(fp);

    n    = 0;
    line = file;

    for( i=0; i<siz; i++ )
       if( line[i] == '\n' ) { line[i]   = '\0'; n++; }
    if( line[i-1]  != '\0' ) { line[i-1] = '\0'; n++; }

    for( i=0; i<n; ++i )
       {
       sscanf(line,"%s  %f",info,&f);

       if( strncmp( info, "BOTTOM", 6) == 0 ) desc->bottom = f;
       if( strncmp( info, "TOP",    3) == 0 ) desc->top    = f;
       if( strncmp( info, "LEFT",   4) == 0 ) desc->left   = f;
       if( strncmp( info, "RIGHT",  5) == 0 ) desc->right  = f;
       if( strncmp( info, "WIDTH",  5) == 0 ) desc->w      = (int)(f+0.5);
       if( strncmp( info, "HEIGHT", 6) == 0 ) desc->h      = (int)(f+0.5);
       if( strncmp( info, "TILE",   4) == 0 ) desc->t      = (int)(f+0.5);
       if( strncmp( info, "SCALE",  5) == 0 ) desc->scale  = f;

       line += strlen(line)+1;
       }

    free(file);

    return(0);
    }
