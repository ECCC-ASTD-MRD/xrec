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

#include <rec.h>
#include <rpnmacros.h>
#include <memory.h>
#include <math.h>
#include <wgl.h>

extern FILE *fdict;

f77name(c_xrecgrille)(gridname,gridtype,ni,nj,ig1,ig2,ig3,ig4,flennomgrille)
char gridname[];
int *gridtype, *ni, *nj, *ig1, *ig2, *ig3, *ig4;
F2Cl flennomgrille;
{
  char nomgrille[64];
  int lennomgrille=flennomgrille;
  int un = 1;
  int i, dimnomgrille;
  int *itemp;
  float *fig1, *fig2, *fig3, *fig4;
  char *tab = "   ";
  char hemisphere[16];
  
  fprintf(fdict, "<grid>\n");
  strncpy(nomgrille, gridname,lennomgrille);
  strclean(nomgrille);

   fig1 = (float *) ig1;
   fig2 = (float *) ig2;
   fig3 = (float *) ig3;
   fig4 = (float *) ig4;
   
   fprintf(fdict, "%s<ident>%s</ident>\n", tab, nomgrille);
   switch (*gridtype)
      {
      case 1:
	fprintf(fdict, "%s<grtyp>A</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<ig1>%d</ig1>\n", tab, *ig1);
	fprintf(fdict, "%s<ig2>%d</ig2>\n", tab, 0);
	fprintf(fdict, "%s<ig3>%d</ig3>\n", tab, 0);
	fprintf(fdict, "%s<ig4>%d</ig4>\n", tab, 0);
	switch (*ig1)
	  {
	  case 0:
	    strcpy(hemisphere, "global");
	    break;
	    
	  case 1:
	    strcpy(hemisphere, "north");
	    break;
	    
	  case 2:
	    strcpy(hemisphere, "south");
	    break;
	  }
	fprintf(fdict, "%s<hemisphere>%s</hemisphere>\n", tab, hemisphere);
        break;	

        
      case 2:
	fprintf(fdict, "%s<grtyp>L</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<swlat>%f</swlat>\n", tab, *fig1);
	fprintf(fdict, "%s<swlon>%f</swlon>\n", tab, *fig2);
	fprintf(fdict, "%s<dlat>%f</dlat>\n", tab, *fig3);
	fprintf(fdict, "%s<dlon>%f</dlon>\n", tab, *fig4);
    break;
        
      case 3:
	fprintf(fdict, "%s<grtyp>N</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<pi>%f</pi>\n", tab, *fig1);
	fprintf(fdict, "%s<pj>%f</pj>\n", tab, *fig2);
	fprintf(fdict, "%s<d60>%f</d60>\n", tab, *fig3);
	fprintf(fdict, "%s<dgrw>%f</dgrw>\n", tab, *fig4);
        break;
        
      case 4:
	fprintf(fdict, "%s<grtyp>S</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<pi>%f</pi>\n", tab, *fig1);
	fprintf(fdict, "%s<pj>%f</pj>\n", tab, *fig2);
	fprintf(fdict, "%s<d60>%f</d60>\n", tab, *fig3);
	fprintf(fdict, "%s<dgrw>%f</dgrw>\n", tab, *fig4);
        break;
        
      case 5:
	fprintf(fdict, "%s<grtyp>G</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<ig1>%d</ig1>\n", tab, *ig1);
	fprintf(fdict, "%s<ig2>%d</ig2>\n", tab, 0);
	fprintf(fdict, "%s<ig3>%d</ig3>\n", tab, 0);
	fprintf(fdict, "%s<ig4>%d</ig4>\n", tab, 0);
	switch (*ig1)
	  {
	  case 0:
	    strcpy(hemisphere, "global");
	    break;
	    
	  case 1:
	    strcpy(hemisphere, "north");
	    break;
	    
	  case 2:
	    strcpy(hemisphere, "south");
	    break;
	  }
	fprintf(fdict, "%s<hemisphere>%d</hemisphere>\n", tab, hemisphere);
        break;
        
      case 6:
	fprintf(fdict, "%s<grtyp>B</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<ig1>%d</ig1>\n", tab, *ig1);
	fprintf(fdict, "%s<ig2>%d</ig2>\n", tab, 0);
	fprintf(fdict, "%s<ig3>%d</ig3>\n", tab, 0);
	fprintf(fdict, "%s<ig4>%d</ig4>\n", tab, 0);
	switch (*ig1)
	  {
	  case 0:
	    strcpy(hemisphere, "global");
	    break;
	    
	  case 1:
	    strcpy(hemisphere, "north");
	    break;
	    
	  case 2:
	    strcpy(hemisphere, "south");
	    break;
	  }
	fprintf(fdict, "%s<hemisphere>%d</hemisphere>\n", tab, hemisphere);
        break;
        
      case 8:
      case 10:
      case 11:
	fprintf(fdict, "%s<grtyp>T</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<td60>%f</td60>\n", tab, *fig1);
	fprintf(fdict, "%s<tdgrw>%f</tdgrw>\n", tab, *fig2);
	fprintf(fdict, "%s<clat>%f</clat>\n", tab, *fig3);
	fprintf(fdict, "%s<clon>%f</clon>\n", tab, *fig4);
        break;
        
      case 9:
        break;
        
      case 0:
	fprintf(fdict, "%s<grtyp>G</grtyp>\n", tab);
	fprintf(fdict, "%s<ni>%d</ni>\n", tab, *ni);
	fprintf(fdict, "%s<nj>%d</nj>\n", tab, *nj);
	fprintf(fdict, "%s<ig1>%d</ig1>\n", tab, *ig1);
	fprintf(fdict, "%s<ig2>%d</ig2>\n", tab, 0);
	fprintf(fdict, "%s<ig3>%d</ig3>\n", tab, 0);
	fprintf(fdict, "%s<ig4>%d</ig4>\n", tab, 0);
	switch (*ig1)
	  {
	  case 0:
	    strcpy(hemisphere, "global");
	    break;
	    
	  case 1:
	    strcpy(hemisphere, "north");
	    break;
	    
	  case 2:
	    strcpy(hemisphere, "south");
	    break;
	  }
	fprintf(fdict, "%s<hemisphere>%d</hemisphere>\n", tab, hemisphere);
        break;
        
      }


   fprintf(fdict, "</grid>\n");
}


/**
 ******************************************************************************
 ** Routine strclean
 ** Objet:  Enleve les espaces entourant un string
 ** Ex:     "   Skippy le kangourou   " -> "Skippy le kangourou"
 ******************************************************************************
 **/

int strclean(str)
char str[];
{
   short i, j, iinit, jinit;

   iinit = 0;
   i = iinit;
   while (str[i] == ' ' && i < strlen(str))
      i++;

   if (i != iinit)
      strcpy(str, str+i);

   jinit = strlen(str)-1;
   j = jinit;

   while (str[j] == ' ' && j >= 0)
      j--;

   if (j != jinit)
      str[j+1] = '\0';
   }
/**
******************************************************************************
******************************************************************************
**/
