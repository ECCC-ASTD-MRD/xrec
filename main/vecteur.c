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
#include <rec.h>
#include <rec_functions.h>
#include <rpnmacros.h>
#include <memory.h>
#include <math.h>
#include <wgl.h>

static int nbVecteurs = 1;
static char vecteurs[64][3][5];
static int once = 1;
static int vectable_pleine = 0;


void f77name(vecteur)(char nom_uu[], char nom_vv[], char nom_ww[], int len_nom_uu, 
  int len_nom_vv, int len_nom_ww)
{
   char lcl_uu[5], lcl_vv[5], lcl_ww[5];
   wordint current_vec;
   
   if (vectable_pleine == 1)
    {
    return; 
    }
      
   if (once == 1)
    {
    strcpy(vecteurs[0][0], "UU  ");
    strcpy(vecteurs[0][1], "VV  ");
    strcpy(vecteurs[0][2], "WW  ");
    once = 0;
    nbVecteurs++;
    }
   
   strcpy(lcl_uu, "    ");
   strcpy(lcl_vv, "    ");
   strcpy(lcl_ww, "    ");
   strncpy(lcl_uu, nom_uu, 4);
   strncpy(lcl_vv, nom_vv, 4);
   strncpy(lcl_ww, nom_ww, 4);
   
   current_vec = nbVecteurs-1;
   strncpy(vecteurs[current_vec][0], lcl_uu, 4);
   strncpy(vecteurs[current_vec][1], lcl_vv, 4);
   strncpy(vecteurs[current_vec][2], lcl_ww, 4);
   nbVecteurs++;
   
}

int c_checkIfVectorVar(char nomvar[])
  {
  int i, found;
  
  found = -1;
  
   if (once == 1)
    {
    strcpy(vecteurs[0][0], "UU  ");
    strcpy(vecteurs[0][1], "VV  ");
    strcpy(vecteurs[0][2], "WW  ");
    once = 0;
    }
  
  i = 0;
  while (i < nbVecteurs && found == -1)
    {
    if (0 == strncmp(nomvar, vecteurs[i][0], strlen(nomvar)))
      {
      return 0;
      }
    if (0 == strncmp(nomvar, vecteurs[i][1], strlen(nomvar)))
      {
      return 1;
      }
/*    if (0 == strncmp(nomvar, vecteurs[i][2], strlen(nomvar)))
      {
      return 2;
      }*/
    i++;
    }
  if (found == -1)
    {
    return -1;
    }
  }
  
int c_getVectorVars(char nomvar[], char uu[], char vv[], char ww[])
  {
  int i, found;
  found = -1;
  
  i = 0;
  while (i < nbVecteurs && found == -1)
    {
    if (0 == strncmp(nomvar, vecteurs[i][0], strlen(nomvar)))
      {
      found = 1;
      }
    if (0 == strncmp(nomvar, vecteurs[i][1], strlen(nomvar)))
      {
      found = 1;
      }
/*    if (0 == strncmp(nomvar, vecteurs[i][2], strlen(nomvar)))
      {
      found = 1;
      }*/
    i++;
    }
  if (found == -1)
    {
    return -1;
    }
  else
    {
    strcpy(uu, vecteurs[i-1][0]);
    strcpy(vv, vecteurs[i-1][1]);
    strcpy(ww, vecteurs[i-1][2]);
    }
  return 0;
  }
