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

#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <rpnmacros.h>
#include <vcar.h>

static int fontSize = 30;

/*
 *
 *  module    :  hl_find
 *
 *  revision  :  V0.1   Michel Grenier
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  char  *hllst;
 *               float *vect;
 *		 int    ni, nj;
 *	         hllst = hl_find( vect, ni, nj, scale );
 *
 *  object    :  THIS MODULE FINDS ALL THE HIGHS LOWS ON A FIELD
 *               AND RETURNS THE LIST
 *
 */

hl_find (vect, ni, nj, scale, hilo,hlcount,hlnmax)
float *vect;
int ni, nj;
float scale;
Hilo hilo[];
int *hlcount, hlnmax;
    {
    float zm, zn, zv ;
    int   crois  ;
    int   ichk, jchk ;
    int   is, js, jt, it ;
    int   x, y;
    register int ip, jp, ik, jk ;

    *hlcount = 0;

/*
 *  ichk and jchk indicates the covering area
 *  under which the extrema is to be evaluated
 */
    ichk = ni/8.0 ; ichk = (2>=ichk ? 2:ichk) ; ichk = (15<=ichk ? 15:ichk) ;
    jchk = nj/8.0 ; jchk = (2>=jchk ? 2:jchk) ; jchk = (15<=jchk ? 15:jchk) ;

/*
 *  loop in all the lines
 */
    for( ip=0; ip < ni; ip++ )
       {
/*
 *  as long as the function is increasing
 *  or decreasing move along the y axe
 */
       zm = vect[ip] ;
       crois = ( zm <= vect[ip+1*ni] ) ;
       for( jp=0; jp < nj-1; jp++ )
          {
          if( jp != nj-1 )
            {
            zn = vect[ip+(jp+1)*ni] ;
            if( zm==zn || ((zm<zn) && crois ) || ((zm>zn) && !crois ) ) 
              {
              zm = zn ;
              continue ;
              };
            } ;
/*
 *  check if it is really a max or a min over the area
 */
          js = ( 0>=jp-jchk ? 0:jp-jchk ) ;
          jt = ( nj-1<=jp+jchk ? nj-1:jp+jchk ) ;
          is = ( 0>=ip-ichk ? 0:ip-ichk ) ;
          it = ( ni-1<=ip+ichk ? ni-1:ip+ichk ) ;

          for( jk=js; jk <=jt; jk++)
             {
             for( ik=is; ik <=it; ik++)
                {
                if( ik != ip || jk != jp ) 
                  {
                  zv = vect[ik+jk*ni] ;
                  if(((zv>=zm) && crois ) || ((zv<=zm) && !crois )) goto nexty; 
                  };
                } ;
             } ;
   
/*
 *  an extrema was found
 */

	  if (*hlcount < hlnmax)
	     {
	     hilo[*hlcount].x = (float)ip+1;
	     hilo[*hlcount].y = (float)jp+1;
	     hilo[*hlcount].value = zm;
	     hilo[*hlcount].symbol = 'L';
	     if(crois) hilo[*hlcount].symbol = 'H';
	     *hlcount = *hlcount + 1;
	     }

/*
 *  continues the search
 */
          nexty: crois = !crois ;
          zm = zn ;
          } ;
       } ;

    return;
    }


hl_print (hilo,hlcount,scale,fore,back)
Hilo hilo[];
int hlcount;
float scale;
int fore;
{
   int i,size;
   float xx, yy;
   char str[16];
   int ii,jj,iw;
   int idebut,jdebut,ifin,jfin,len;
   int isym,icen,inum,jsym,jcen,jnum;
   int anott = 0;
   int icent;



   set_vcar( FONTE, 3, 0);
   c_wglgvi(&idebut,&jdebut,&ifin,&jfin);

   for (i=0; i < hlcount; i++)
      {
      c_xy2fxfy(&xx, &yy, hilo[i].x,hilo[i].y);
      c_wglxai(&ii, &jj, xx, yy);
      
      ii = ii <= 0 ? -9999999 : ii;
      jj = jj <= 0 ? -9999999 : jj;

      if (ii > idebut && ii < ifin && jj > jdebut && jj < jfin)
	 {
	 isym = ii;
	 jsym = jj+6;

	 icen = ii+1;
	 jcen = jj-3;

	 c_wgllwi(2);
	 
	 c_wglcol(fore);

	 icent = 31;
	 sprintf(str,"%c",hilo[i].symbol);
	 len = strlen(str);
	 /*	 f77name(xpwrit)(&isym,&jsym,str,&len,&fontSize,&anott,&icent,len); */
	 
	 size=ROUND(0.6*fontSize);
	 icent = 1;
	 sprintf(str,"%s","^");
	 len = strlen(str);
	 f77name(xpwrit)(&icen,&jcen,str,&len,&size,&anott,&icent,len);
	 
	 size=ROUND(0.6*fontSize);
	 inum = ii+ROUND(0.5*size);
	 jnum = jj-2;
	 
	 /*	 icent = 41;
	 if (hilo[i].value > 0.0)
	    {
	    sprintf(str,"%-6d",ROUND(hilo[i].value/scale));
	    }
	 else
	    {
	    sprintf(str,"%-6d",-(ROUND(fabs(hilo[i].value/scale))));
	    }
	 */
	 strclean(str);
	 len = strlen(str);
	 if (len == 1)
	    {
	    inum=ii;
	    }
	 f77name(xpwrit)(&inum,&jnum,str,&len,&size,&anott,&icent,len);
	 }
      }
   set_vcar( FONTE, 3, 0);
   }

hl_setFontSize(size)
int size;
{
   fontSize = size;
   }

hl_getFontSize()
{
   return fontSize;
   }
