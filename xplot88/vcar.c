/* RMNLIB - Library of useful routines for C and FORTRAN programming
 * Copyright (C) 1975-2001  Division de Recherche en Prevision Numerique
 *                          Environnement Canada
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation,
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 *
 *  librairie :  VCAR.C
 *
 *  auteur    :  MICHEL GRENIER    DDO    AOUT 1988
 *
 *  revision  :  V0.0                     AOUT 1988
 *               V1.0                     NOV  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  CETTE LIBRAIRIE CONTIENT TOUS LES MODULES POUR CREER
 *               ET AFFICHER DES CARACTERES DES FONTES VECTORIELLES
 *
 */

#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <varargs.h>
#include <sys/types.h>

#include <vcar.h>

 static  short   attrib[6][20] = {
  -8, -8,-10,-10,-10,-10,-10,-10,-10,-10, -7, -7, -7, -7, -8,-10,-10, -7, -5,0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,0,
 -16,-16,-22,-22,-22,-22,-20,-20,-20,-20,-14,-14,-14,-14,-16,-20,-20,-14,-10,0,
   4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,0,
 -20,-20,-25,-25,-25,-25,-24,-24,-24,-24,-18,-18,-18,-18,-20,-25,-25,-18,-14,0,
   8,  8, 10, 10, 10, 10, 10, 10, 10, 10,  7,  7,  7,  7,  8, 10, 10,  7,  5,0};

 static  int     baselin[20] = {
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 20, 16, 16, 16, 16,0};

 static  Vcar    fontes[64][20];

 static  Vcarop  vcarop;


/*
 *
 *  module    :  GEN_VCAR
 *
 *  auteur    :  MICHEL GRENIER    DDO    AOUT 1988
 *
 *  revision  :  V0.0                     AOUT 1988
 *               V1.0                     NOV  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  call      :  GEN_VCAR( ipath, opath )
 *
 *  arguments :  ipath FICHIER A LIRE,   LES FONTES VECTORIELLES EN ASCII
 *               opath FICHIER A ECRIRE, LES FONTES VECTORIELLES BINAIRES
 *
 *  objet     :  CE MODULE CREE LE FICHIER BINAIRE DE FONTES
 *
 */

 extern void
 gen_vcar ( ipath, opath )
 char *ipath;
 char *opath;
    {
    FILE *in;
    FILE *out;

    char car, car2;
    char *ptr;
    char str[80];
    char work[15];

    int  font, font2, i, icar, j, k, n;

    PointP2 vect[130];

/*
 *  ouverture des fichiers
 */
    in  = fopen( ipath, "r" );
    if( in == NULL ) fprintf(stderr, "Erreur fichier %s \n",ipath);

    out = fopen( opath, "w" );
    if( out == NULL ) fprintf(stderr, "Erreur fichier %s \n",opath);

    if( in == NULL || out == NULL ) return;

/*
 *  lecture des caracteres des fontes
 */
    car = 0;
    n   = 0;
    while( fgets(str, 73, in ) != NULL ) 
         {
         if( (ptr = strchr(str,'\n')) != NULL ) ptr[0] = '\0';

/*
 *  sauvegarde le dernier caractere
 */
         if( strlen(str) > 50 && car != 0 )
           { 
           fontes[icar][font].npnts=n;
           fontes[icar][font].vecteurs=(PointP2 *)calloc(n, sizeof(PointP2));
           memcpy(fontes[icar][font].vecteurs,vect, n*sizeof(PointP2));
           n = 0;
           };

/*
 *  fin des description de fonte
 */
         if( strstr(str,"FIN") != NULL) break;

/*
 *  nouveau caractere
 */
         if( strlen(str) > 50 )
           {
           sscanf(&str[55], "%c%d", &car, &font);
           icar = ((int)car) - 32;
           sscanf(&str[69], "%d", &fontes[icar][font].largeur);
           };

/*
 *  point de dessin du caractere
 */
         for( i = 0; i < strlen(str) && i < 50; i += 10, n++)
            {
            if (str[i] == ' ') break;
            strncpy(work, &str[i], 10);
            work[10] = '\0';
            k= sscanf(work,"%d  %2d  %2d",&vect[n].pen,&vect[n].x,&vect[n].y);
            if( k == 0 ) break;
            if( vect[n].pen == 0 && vect[n].x == 0 && vect[n].y == 0 ) break;
            j = vect[n].y + 16 - baselin[font];
            vect[n].y = j > 0 ? j : 0 ;
            }
         }
/*
 *  duplication : car1 fonte1 = car2 fonte2
 */
    while( fgets(str, 73, in ) != NULL )
         {
         if( (ptr = strchr(str,'\n')) != NULL ) ptr[0] = '\0';
         sscanf(str, " %c%2d%c%2d", &car, &font, &car2, &font2);
         memcpy(&fontes[((int) car) - 32][font],
                &fontes[((int) car2) - 32][font2],
                sizeof(Vcar));
         }

/*
 *  ecrit le resultat dans un fichier
 */
    for( i = 0; i <= 19; i++ )
       {
       for( j = 0; j < 64; j++ )
          {
          fwrite(&fontes[j][i], 1, sizeof(Vcar)-sizeof(PointP2 *), out);
          fwrite(fontes[j][i].vecteurs, fontes[j][i].npnts,
                 sizeof(PointP2), out);
          }
       }

/*
 *  fermeture des fichiers
 */
    fclose(in);
    fclose(out);

    return;
    }


/*
 *
 *  module    :  GET_VCAR
 *
 *  auteur    :  MICHEL GRENIER    DDO    AOUT 1988
 *
 *  revision  :  V0.0                     AOUT 1988
 *               V1.0                     NOV  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  call      :  GET_VCAR( args )
 *
 *  arguments :  args    LES VARIABLES ET LES VALEURS SE RAPPORTANT A VCAR
 *
 *  objet     :  CE MODULE RETOURNE UNE LISTE DE VALEURS SE RAPPORTANT A VCAR
 *
 */

 extern void
 get_vcar ( va_alist )
 va_dcl
    {

    va_list argv;

    int     variable;

/*
 *  boucle sur tous les variables
 */

    va_start(argv);
    while( (variable = va_arg(argv,int)) != 0 )
         {
         switch( variable )
               {
               case ANGLE   : *va_arg(argv,int *) = vcarop.angle;   break;
               case CENTRE  : *va_arg(argv,int *) = vcarop.centre;  break;
               case FONTE   : *va_arg(argv,int *) = vcarop.fonte;   break;
               case HAUTEUR : *va_arg(argv,int *) = vcarop.hauteur; break;
               case LARGEUR : *va_arg(argv,int *) = vcarop.largeur; break;
               case YMIROIR : *va_arg(argv,int *) = vcarop.ymiroir; break;
               default      : fprintf(stderr," erreur vcar option !\n");break;
               }
         }
    va_end(argv);

    return;
    }


/*
 *
 *  module    :  LIR_VCAR
 *
 *  auteur    :  MICHEL GRENIER    DDO    AOUT 1988
 *
 *  revision  :  V0.0                     AOUT 1988
 *               V1.0                     NOV  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  call      :  LIR_VCAR( ipath )
 *
 *  arguments :  ipath FICHIER A LIRE, LES FONTES VECTORIELLES BINAIRES
 *
 *  objet     :  CE MODULE LIT LE FICHIER BINAIRE DE FONTES
 *
 */

 extern void
 lir_vcar ( ipath )
 char *ipath;
    {
    FILE *in;

    register int  i, j;

/*
 *  ouverture des fichiers
 */

    in  = fopen( ipath, "r" );
    if( in == NULL )
      {
      fprintf(stderr, "Erreur fichier %s \n",ipath);
      return;
      }

/*
 *  lecture des fontes vectorielles binaires
 */

#if defined (hp720) || defined (SGI)
    for(i = 0; i < 20; i++)
      {
      for(j = 0; j < 64; j++)
	{
	fread(&fontes[j][i], sizeof(Vcar) - sizeof(PointP2 *) ,1, in);
	fontes[j][i].vecteurs = (PointP2 *)malloc(fontes[j][i].npnts*sizeof(PointP2));
	fread(fontes[j][i].vecteurs,sizeof(PointP2),fontes[j][i].npnts, in);
	}
      }
#else
    for(i = 0; i < 20; i++)
      {
      for(j = 0; j < 64; j++)
	{
	fread32(&fontes[j][i], sizeof(Vcar) - sizeof(PointP2 *) ,1, in);
	if (fontes[j][i].npnts == 0)
	  {
	  fontes[j][i].vecteurs = (PointP2 *)malloc(1*sizeof(PointP2));
	  }
	else
	  {
	  fontes[j][i].vecteurs = (PointP2 *)malloc(fontes[j][i].npnts*sizeof(PointP2));
	  fread32(fontes[j][i].vecteurs,sizeof(PointP2),
		  fontes[j][i].npnts, in);
	  }
	}
      }
#endif
    
    /*
     *  fermeture du fichier
     */
    fclose(in);
    
    return;
    }


/*
 *
 *  module    :  SET_VCAR
 *
 *  auteur    :  MICHEL GRENIER    DDO    AOUT 1988
 *
 *  revision  :  V0.0                     AOUT 1988
 *               V1.0                     NOV  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  call      :  SET_VCAR ( args )
 *
 *  arguments :  args    LES VARIABLES ET LES VALEURS SE RAPPORTANT A VCAR  
 *
 *  objet     :  CE MODULE INITIALISE UNE LISTE DE VALEURS SE RAPPORTANT A VCAR
 *
 */

 extern void
 set_vcar ( va_alist )
 va_dcl
    {

    va_list argv;

    int     variable;

/*
 *  boucle sur tous les variables
 */

    va_start(argv);
    while( (variable = va_arg(argv,int)) != 0 )
         {
         switch( variable )
               {
               case ANGLE   : vcarop.angle   = va_arg(argv,int); break;
               case CENTRE  : vcarop.centre  = va_arg(argv,int); break;
               case FONTE   : vcarop.fonte   = va_arg(argv,int); break;
               case HAUTEUR : vcarop.hauteur = va_arg(argv,int); break;
               case LARGEUR : vcarop.largeur = va_arg(argv,int); break;
               case YMIROIR : vcarop.ymiroir = va_arg(argv,int); break;
               default      : fprintf(stderr," erreur vcar option !\n");break;
               }
         }
    va_end(argv);

    return;
    }


/*
 *
 *  module    :  VCAR
 *
 *  auteur    :  MICHEL GRENIER    DDO    AOUT 1988
 *
 *  revision  :  V0.0                     AOUT 1988
 *               V1.0                     NOV  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  call      :  mvchr = VCAR ( chr )
 *
 *  arguments :  chr    LE CARACTERE DONT ON DESIRE LES VECTEURS 
 *               mvchr  LES VECTEURS DU CARACTERES
 *
 *  objet     :  CE MODULE VA CHERCHER LES VECTEURS D'UN CARACTERE DANS
 *               LES FONTES BINAIRES. LES VECTEURS SONT TRANSFORMES
 *               POUR REPONDRE A LA POSITION, AU CENTRAGE ET A LA
 *               TAILLE DEMANDES.
 *
 */

 extern Vcar *
 vcar ( cx, cy, car )
 int  cx;
 int  cy;
 char car;
    {
    float    dx,dy, loc_a, loc_b, loc_c, loc_d, mapx, mapy;
    int      angle, pen, x, y;
    int      cntrx, corx,  cntry, len;
    PointP2 *vec;
    Vcar    *mvcar;

    register Vcar    *carp;
    register PointP2 *mvec;
    register int i;

/*
 *  liste des vecteurs du caractere
 */
    carp = &fontes[(int)car-32][vcarop.fonte];
    vec = carp->vecteurs;

/*
 *  echelle
 */
    mapx = (float)vcarop.largeur / fontes[(int)car-32][vcarop.fonte].largeur;
    mapy = (float)vcarop.hauteur / attrib[2][vcarop.fonte]; 

/*
 *  cas speciale ou l'origine est le coin inferieur gauche
 */
    if( vcarop.ymiroir != 0 )
      {
      mapy = -mapy;
      angle = vcarop.angle;
      vcarop.angle = abs(360-vcarop.angle);
      }

/*
 *  transformation matricielle pour l'angle du caractere
 */

    loc_a =  mapx * cos(vcarop.angle*M_PI/180.0);
    loc_b = -mapx * sin(vcarop.angle*M_PI/180.0);
    loc_c =  mapy * sin(vcarop.angle*M_PI/180.0);
    loc_d =  mapy * cos(vcarop.angle*M_PI/180.0);

    if( vcarop.ymiroir != 0 ) vcarop.angle = angle;

/*
 *  centrage
 */
    dx = 0.0;
    dy = -16.0;

    cntrx =  vcarop.centre % 10;
    cntry =  vcarop.centre / 10;

    corx = 0;
    if( cntry == 3  || cntry == 4 ) corx = 4;
    len = fontes[(int)car-32][vcarop.fonte].largeur;

    switch(cntrx)
          {
          case 0: dx = -len/2.0;       break;
          case 1: dx = -len/2.0;       break;
          case 2: dx =  corx   ;       break;
          case 3: dx = -len/2.0;       break;
          case 4: dx = -len - corx ;   break;
          }

    dy += attrib[cntry][vcarop.fonte];
    if( attrib[cntry][vcarop.fonte] == 0 && cntry == 0 ) dy += -8;

    x = loc_a*dx + loc_c*dy + cx;
    y = loc_b*dx + loc_d*dy + cy;

/*
 *  reserve de l'espace pour le caractere cree
 */
    mvcar = (Vcar *) malloc ( sizeof(Vcar) );
    mvec = (PointP2 *) calloc ( carp->npnts+1 , sizeof(PointP2) );

/*
 *  si le premier point est deja avec pen == 1
 *  on devra le preceder avec le point (0,baselin)
 */
   len = 0;
   pen = 1;
    if( vec->pen != 0 )
      {
      mvec[len].pen = 0;
      mvec[len].x   = loc_a*0 + loc_c*16 + x;
      mvec[len].y   = loc_b*0 + loc_d*16 + y;
      len = 1;
      pen = 0;
      }

/*
 *  transfert du reste des points des vecteurs des caracteres
 */
    corx = len;
    for( i = corx ; i < carp->npnts+corx; i++, vec++)
       {
       if( pen == 0 && vec->pen == 0 ) len--;
       mvec[len].pen = vec->pen;
       mvec[len].x   = loc_a*vec->x + loc_c*vec->y + x;
       mvec[len].y   = loc_b*vec->x + loc_d*vec->y + y;
       len++;
       pen = vec->pen;
      }

/*
 *  mets les informations correctements
 */
    mvcar->largeur  = vcarop.largeur;
    mvcar->npnts    = len;
    mvcar->vecteurs = mvec;

/*
 *  retourne l'informations
 */

    return( mvcar );
    }
