#include <stdio.h>
#include <stdlib.h>
#include <gdb.h>

 main( int argc, char **argv )
     {
     FILE  *fp;
     unsigned char  *msk;
     short *topo, min, max;
     int    w, h, d;
     int    i;
     char   typ[4];
     int    ni, nj;
     int    ig1, ig2, ig3, ig4;
     char   vtyp[4];
     float *xdef;
     float *ydef;
     float rval;

/*
 *   initialisation de la base de donnees
 */
     gdb_init();

/*
 *   limite de la recuperation des donnees
 */
     gdb_limit(40.0, -80.0, 50.0, -60.0 );
     
     /*
 *   demande des parametre rmn pour cette limite a cette resolution
 */
     gdb_rmn(64, typ,&ni,&nj,&ig1,&ig2,&ig3,&ig4, vtyp,&xdef,&ydef );

     printf(" grille %c %d %d %d %d %d %d \n",typ[0],ni,nj,ig1,ig2,ig3,ig4);
     printf("        %c                   \n",vtyp[0]);
     printf(" xdef = ");
     for(i=0;i<ni;++i) { if( i%10 == 0 ) printf("\n"); printf(" %f ",xdef[i]); }
     printf(" \nydef = ");
     for(i=0;i<nj;++i) { if( i%10 == 0 ) printf("\n"); printf(" %f ",ydef[i]); }

 /*
 *   lecture des donnees de masque pour cette limite
 */
     /*      msk = (char *) malloc(10* ni*nj*sizeof(char)); */
     gdb_map(64, GDB_MAP_DEM, (char **) &topo, &w, &h, &d );
     printf("\nd = %d %d %d\n", w, h, d);

     min = topo[0];
     max = topo[0];
     for (i=0; i < ni*nj; i++)
       {
       min = min < topo[i] ? min : topo[i];
       max = max > topo[i] ? max : topo[i];
       }

     printf("min: %d max: %d\n", min, max);

     msk = (char *) malloc(ni * nj * sizeof(unsigned char));
     for (i=0; i < ni*nj; i++)
       {
       rval = 255 * (topo[i]-min)/(max-min);
       msk[i]  = (unsigned char)  rval;
       }
     



/*
 *   ecrit le masque dans un fichier pour visualisation
 */
     fp = fopen("montreal.gray","w");
     fprintf(fp, "P5\n%d %d\n255\n", ni, nj);
     fwrite( msk, w*h, 1, fp );

     fclose(fp);

/*
 *   libere la memoire
 */
     free(xdef);
     free(ydef);
     free(msk);
     }

