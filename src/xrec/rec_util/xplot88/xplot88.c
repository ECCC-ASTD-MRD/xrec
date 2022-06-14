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
 *  fichier   :  XPLOT88.C
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  CE FICHIER CONTIENT TOUS LES MODULES POUR IMITER
 *               LES FONCTIONS DE PLOT88 DE RMNLIB
 *
 */

#include <math.h>
/**#include <stdlib.h> **/
#include <string.h>
#include <sys/types.h>
#include <vcar.h>
#include <wgl.h>

 static int  lst_x;
 static int  lst_y;
 static int  pos_x;
 static int  pos_y;

char *bidon = NULL;

 static char optn[28][3] =
          { "AN ","BC ","BO ","CA ","CF ",
            "CH ","CL ","CN ","CO ","CW ",
            "DC ","DP ","DR ","EN ","FC ",
            "HS ","IN ","OR ","PA ","PO ",
            "PU ","RE ","SI ","SL ","TH ",
            "UP ","VS ","" };

 static char bdoptn [33][27][9];
 static int  bdvals [33][27];
 static int  nvals  [33] = { 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0  };



/*
 *
 *  module    :  XBNDLSET_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION BNDLSET DANS RMNLIB
 *
 */

 int f77name(xbndlset) ( xoptn, iopval, n, nbdl, flen )
 char *xoptn;
 int  *iopval;
 int  *n;
 int  *nbdl;
 F2Cl flen;
     {
     char *ptr = xoptn;
     int i;
     int len=flen;

     nvals[*nbdl] = *n;

     for( i=0; i<*n; ptr+=len, i++ )
        {
        strncpy(bdoptn[*nbdl][i],ptr,len);
        bdvals[*nbdl][i] = iopval[i];
        }

     }


/*
 *
 *  module    :  XBUNDLE_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION BUNDLE DANS RMNLIB
 *
 */

 int f77name(xbundle) ( nbdl )
 int  *nbdl;
     {
     int i;

     for( i = 0 ; i < nvals[*nbdl] ; i ++ )
        f77name(xop1set)( bdoptn[*nbdl][i], &bdvals[*nbdl][i], (F2Cl) 9 );

     }


/*
 *
 *  module    :  XFL2INT_
 *
 *  auteurs   :  MICHEL GRENIER    CMCOG
 *            :  Yves Chartier     RPN
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION FL2INT DANS RMNLIB
 *
 */

int f77name(xfl2int)(x, y, i, j)
int  *x,  *y;
int  *i,  *j;
{
   float *x1, *y1;

   if (*x > 0 && *x < 32768 && *y > 0 && *y < 32768)
      {
      *i = *x;
      *j = *y;
      }
   else
      {
      x1 = (float *) x;
      y1 = (float *) y;
      c_wglxai(i, j, *x1, *y1);
      }
   }


/*
 *
 *  module    :  XGETSET_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION GETSET DANS RMNLIB
 *
 */

 int f77name(xgetset)( ixmin, ixmax, iymin, iymax, x1, x2, y1, y2, ltype )
 int   *ixmin, *ixmax, *iymin, *iymax, *ltype ;
 float *x1,    *x2,    *y1,    *y2;
     {
     c_wglgsp(x1, y1, x2, y2, ixmin, iymin, ixmax, iymax);

     *ltype = 1;

     return;
     }


/*
 *
 *  module    :  XMAPTRN_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION MAPTRN DANS RMNLIB
 *
 */

 int f77name(xmaptrn)( lat, lon, u, v )
 float *lat, *lon;
 float  *u,  *v;
     {
     float x1, y1, x2, y2;

     c_wglgvx(&x1, &y1, &x2, &y2);
     if( *u < x1 || *u > x2 ||
         *v < y1 || *v > y2  ) return(0);

     return(1);
     }


/*
 *
 *  module    :  XMXMY_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION MXMY DANS RMNLIB
 *
 */

 int f77name(xmxmy)( x, y )
 int *x, *y;
     {
     *x = lst_x;
     *y = lst_y;
     return;
     }


/*
 *
 *  module    :  XOP1GET_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION OP1GET DANS RMNLIB
 *
 */

 f77name(xop1get)( xoptn, iopval, flen )
 char *xoptn;
 int  *iopval;
 F2Cl flen;
     {
     char  dup[3];
     char  *ptr;
     int   ipos;
     int   len=flen;

/*
 *  l'option est dupliquee parce que en fortan il n'y a pas de \0 a la fin
 */
     if( len > 9 ) return;
     strncpy( dup, xoptn, 2 );
     dup[2] = '\0';

/*
 *  fait la recherche de l'option dans la liste
 */
     ptr = strstr ( optn[0], dup );
     ipos = ((int)(ptr-optn[0])) / 3 ;

/*
 *  va chercher la valeur de l'option demandee
 */

     switch( ipos )
           {
           case  0 : get_vcar(bidon,  ANGLE,   iopval, 0 ); break;
           case  3 : set_vcar(bidon,  FONTE,   iopval, 0 ); break;
           case  5 : get_vcar(bidon,  HAUTEUR, iopval, 0 ); break;
           case  7 : get_vcar(bidon,  CENTRE,  iopval, 0 ); break;
           case  8 : c_wglcol( *iopval ) ;           break;
           case  9 : get_vcar(bidon,  LARGEUR, iopval, 0 ); break;
           case 14 : c_wglcol( *iopval ) ;           break;
           case 16 : c_wgllwi( *iopval ) ;           break;
           case 22 : get_vcar(bidon,  HAUTEUR, iopval,
                               LARGEUR, iopval, 0 ); break;
           case 24 : c_wgllwi( *iopval );            break;

/*
 *  options a developper
 */
           case  1 : /* BCAR       */                break;
           case  2 : /* BOX        */                break;
           case  4 : /* CFILL      */                break;
           case  6 : /* CLR        */                break;
           case 10 : /* DCHAR      */                break;
           case 11 : /* DPOLY      */                break;
           case 12 : /* DRAW       */                break;
           case 15 : /* HSPACE     */                break;
           case 17 : /* ORN        */                break;
           case 18 : /* PAT        */                break;
           case 19 : /* POP        */                break;
           case 20 : /* PUSH       */                break;
           case 21 : /* REVERSE    */                break;
           case 23 : /* SLANT      */                break;
           case 25 : /* UPPERC     */                break;
           case 26 : /* VSPACE     */                break;
           }

     }


/*
 *
 *  module    :  XOP1SET_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION OP1SET DANS RMNLIB
 *
 */

 f77name(xop1set)( xoptn, iopval, flen )
 char *xoptn;
 int  *iopval;
 F2Cl flen;
     {
     char  dup[3];
     char  *ptr;
     int   ipos;
     int   len=flen;

/*
 *  l'option est dupliquee parce que en fortan il n'y a pas de \0 a la fin
 */
     if( len > 9 ) return;
     strncpy( dup, xoptn, 2 );
     dup[2] = '\0';

/*
 *  fait la recherche de l'option dans la liste
 */
     ptr = strstr ( optn[0], dup   );
     ipos = ((int)(ptr-optn[0])) / 3 ;

/*
 *  fait l'affectataion de l'option
 */
     switch( ipos )
           {
           case  0 : set_vcar(bidon,  ANGLE,   *iopval, 0 ); break;
           case  3 : set_vcar(bidon,  FONTE,   *iopval, 0 ); break;
           case  5 : set_vcar(bidon,  HAUTEUR, *iopval, 0 ); break;
           case  7 : set_vcar(bidon,  CENTRE,  *iopval, 0 ); break;
           case  8 : c_wglcol( *iopval ) ;            break;
           case  9 : set_vcar(bidon,  LARGEUR, *iopval, 0 ); break;
           case 14 : c_wglcol( *iopval ) ;            break;
           case 16 : c_wgllwi( *iopval ) ;            break;
           case 22 : set_vcar(bidon,  HAUTEUR, *iopval,
                               LARGEUR, *iopval, 0 ); break;
           case 24 : c_wgllwi( *iopval );             break;

/*
 *  options a developper
 */
           case  1 : /* BCAR       */                 break;
           case  2 : /* BOX        */                 break;
           case  4 : /* CFILL      */                 break;
           case  6 : /* CLR        */                 break;
           case 10 : /* DCHAR      */                 break;
           case 11 : /* DPOLY      */                 break;
           case 12 : /* DRAW       */                 break;
           case 13 : /* ENHANCE    */                 break;
           case 15 : /* HSPACE     */                 break;
           case 17 : /* ORN        */                 break;
           case 18 : /* PAT        */                 break;
           case 19 : /* POP        */                 break;
           case 20 : /* PUSH       */                 break;
           case 21 : /* REVERSE    */                 break;
           case 23 : /* SLANT      */                 break;
           case 25 : /* UPPERC     */                 break;
           case 26 : /* VSPACE     */                 break;
           }

     }


/*
 *
 *  module    :  XOPNGET_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION OPNGET DANS RMNLIB
 *
 */

 f77name(xopnget)( xoptn, iopval, n, flen )
 char *xoptn;
 int  *iopval;
 int  *n;
 F2Cl flen;
     {
     char *ptr = xoptn;
     int i;
     int len=flen;

     for( i=0; i<*n; ptr+=len, i++ )
        f77name(xop1get)( ptr, &iopval[i], flen );

     }


/*
 *
 *  module    :  XOPNSET_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION OPNSET DANS RMNLIB
 *
 */

 f77name(xopnset)( xoptn, iopval, n, flen )
 char *xoptn;
 int  *iopval;
 int  *n;
 F2Cl flen;
     {
     char *ptr = xoptn;
     int i;
     int len=flen;

     for( i=0; i<*n; ptr+=len, i++ )
        f77name(xop1set)( ptr, &iopval[i], flen );

     }


/*
 *
 *  module    :  XPLOTIT_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *            :  Yves Chartier     RPN
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION PLOTIT DANS RMNLIB
 *
 */

f77name(xplotit)(i, j, flag)
int *i, *j, *flag;
{
   float *x, *y;
   int i1, j1;
   float x1, y1;
   int largeurFenetre, hauteurFenetre;

   if (*i > 0 && *i < 32768 && *j > 0 && *j < 32768)
      {
      f77name(xplotit2)(i, j, flag);
      }
   else
      {
      x = (float *) i;
      y = (float *) j;
      c_wglxai(&i1, &j1,*x,*y);
      f77name(xplotit2)(&i1, &j1, flag);
      }

   }

 int f77name(xplotit2)( x, y, down )
 int *x, *y, *down;
     {
     lst_x = *x;
     lst_y = *y;
     if( *down == 0 ) c_wglmvi(*x,*y);
     if( *down == 1 ) c_wgldri(*x,*y);
     return;
     }


/*
 *
 *  module    :  XPWRS_
 *
 *  auteur    :  MICHEL GRENIER    CMCOG
 *
 *  revision  :  V0.0                     DEC  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  DUPLICATION DE LA FONCTION PWRS DANS RMNLIB
 *
 */

 int f77name(xpwrs)( chr, nchr, icoded, flen )
 char *chr;
 int  *nchr;
 int  *icoded;
 F2Cl flen;
     {
     float    delta;
     int      angle, centre, haut, i, j, len;
     int      larg, nxt_x, nxt_y, ymiroir;
     PointP2 *mvec;
     Vcar    *mvcar;

     len = *nchr;

/*
 *  les vents doivent etre doubles  mais pas interactivement
 */
     if( icoded && len>=3 && strncmp(chr,"@9B",3) == 0 )
       {
       get_vcar(bidon,  LARGEUR, &larg,  HAUTEUR, &haut,  0);
       set_vcar(bidon,  LARGEUR, larg/2, HAUTEUR, haut/2, 0);
       }

/*
 *  position de depart de la chaine
 */
     nxt_x = lst_x;
     nxt_y = lst_y;

/*
 *  si la chaine est centree horizontalement on calcule
 *  sa longueur reelle et sa positions de depart en x
 */
     get_vcar(bidon,  ANGLE, &angle, CENTRE, &centre, LARGEUR, &larg, 0 );
     for( i=0 ; icoded && i<*nchr; i++ ) if( chr[i] == '@' ) len -= 2;
     if( len != 1 )
       {
       delta = 0.0;
       if( centre%10 == 1 ) delta =(float)larg*(float)len/2.0;
       if( centre%10 == 3 ) delta =(float)larg*((float)len-0.5);
       if( centre%10 == 4 ) delta =(float)larg*(float)len;
       nxt_x -=(int)(delta*cos(angle*M_PI/180.0));
       }

/*
 *  boucle sur chacun des caracteres
 */
     i = 0;
     while( i < *nchr )
        {

/*
 *  si c'est une chaine codee et que l'on a un caractere
 *  de controle alors on active l'option correspondante
 */
        if( *icoded && chr[i] == '@' )
          {
          i++;
          switch(chr[i++])
                {
                case 'J' : set_vcar(bidon,  FONTE, 0, 0 ); break;
                case 'A' : set_vcar(bidon,  FONTE, 1, 0 ); break;
                case 'B' : set_vcar(bidon,  FONTE, 2, 0 ); break;
                case 'C' : set_vcar(bidon,  FONTE, 3, 0 ); break;
                case 'D' : set_vcar(bidon,  FONTE, 4, 0 ); break;
                case 'E' : set_vcar(bidon,  FONTE, 5, 0 ); break;
                case 'F' : set_vcar(bidon,  FONTE, 6, 0 ); break;
                case 'G' : set_vcar(bidon,  FONTE, 7, 0 ); break;
                case 'H' : set_vcar(bidon,  FONTE, 8, 0 ); break;
                case 'I' : set_vcar(bidon,  FONTE, 9, 0 ); break;

                case '0' : set_vcar(bidon,  FONTE, 0, 0 ); break;
                case '1' : set_vcar(bidon,  FONTE, 1, 0 ); break;
                case '2' : set_vcar(bidon,  FONTE, 2, 0 ); break;
                case '3' : set_vcar(bidon,  FONTE, 3, 0 ); break;
                case '4' : set_vcar(bidon,  FONTE, 4, 0 ); break;
                case '5' : set_vcar(bidon,  FONTE, 5, 0 ); break;
                case '6' : set_vcar(bidon,  FONTE, 6, 0 ); break;
                case '7' : set_vcar(bidon,  FONTE, 7, 0 ); break;
                case '8' : set_vcar(bidon,  FONTE, 8, 0 ); break;
                case '9' : set_vcar(bidon,  FONTE, 9, 0 ); break;

/*
 *  options a developper
 */
                case 'P' : /*  start enhanced printing */ break;
                case 'Q' : /*  stop  enhanced printing */ break;
                case 'R' : /*  start erase before print*/ break;
                case 'S' : /*  stop  erase before print*/ break;

                case 'X' : /*  intensity = 1           */ break;
                case 'Y' : /*  intensity = intensity+1 */ break;
                case 'Z' : /*  intensity = intensity-1 */ break;

                case '[' : /*  start of box            */ break;
                case ']' : /*  end   of box            */ break;

                case '\'' : /*  start underlining       */ break;
                case '&' : /*  stop  underlining       */ break;

                case '"' : /*  reverse print           */ break;
                case '%' : /*  normal  print           */ break;

                case '<' : /*  size = size * 0.7071    */ break;
                case '>' : /*  size = size * 1.4142    */ break;
                case '\\' : /*  backspace               */ break;
                case '@' : /*  @ sign                  */ break;

                }
          }


/*
 *  va chercher les vecteurs du caracteres a dessiner
 */
        mvcar = vcar(nxt_x,nxt_y,chr[i]);

/*
 *  pour chacun des points on dessine le vecteur si le crayon
 *  est en mode de dessin
 */
        for( j = 0 ; j < mvcar->npnts; j++ )
           {
           if( mvcar->vecteurs[j].pen == 0 )
               c_wglmvi(mvcar->vecteurs[j].x, mvcar->vecteurs[j].y );
           else
               c_wgldri(mvcar->vecteurs[j].x, mvcar->vecteurs[j].y );
           }

/*
 *  libere la memoire utilisee par les vecteurs du caractere
 */
         free( mvcar->vecteurs );
         free( mvcar );

/*
 *  calcule la position du caractere suivant
 */
         get_vcar(bidon,  ANGLE,   &angle,
                   HAUTEUR, &haut,
                   LARGEUR, &larg, 0 );

         nxt_x +=(int)(larg*cos(angle*M_PI/180.0));
         nxt_y +=(int)(haut*sin(angle*M_PI/180.0));

         i++;
         }

/**     set_vcar(bidon,  FONTE, 0, 0 ); **/

     return;
     }



f77name(xpwrit)(ix, iy, chr, nchr, size, anott, icent, flen)
int *ix, *iy;
char chr[];
int *nchr;
int *size, *anott, *icent;
F2Cl flen;
{
   float *x, *y;
   int i,i1, j1;
   int largeurFenetre, hauteurFenetre;
   int verite = 0;
   int longueur;
   int car, dimensionFonte;
   int len=flen;

   int anot = 45;

   int caractereTrouve = 0;


   i = 0;
   while (i < len && !caractereTrouve)
      {
      if (chr[i]!=' ' || chr[i]!= '\0')
   caractereTrouve = 1;
      i++;
      }

   if (!caractereTrouve)
      return;

   dimensionFonte = (int)(*size * 0.667);
   dimensionFonte = (int)(*size);

   set_vcar(bidon,  HAUTEUR, dimensionFonte,
             LARGEUR, dimensionFonte,
       ANGLE,   *anott,
       CENTRE,   *icent,
             0 );
   c_wglgwz(&largeurFenetre, &hauteurFenetre);
/**   c_wgllwi(1); **/

   if (*ix > 0 && *ix < 32768 && *iy > 0 && *iy < 32768)
      {
      i1 = *ix;
      j1 = *iy;
      }
   else
      {
      x = (float *) ix;
      y = (float *) iy;
      c_wglxai(&i1, &j1, *x, *y);
      }

   f77name(xplotit)(&i1, &j1, &verite);
   f77name(xpwrs)(chr, nchr, &verite, (F2Cl) 4);
   }
