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

unsigned char  x_invertChar(unsigned char nombre);
unsigned short x_invertShort(unsigned short nombre);
unsigned int   x_iInvertInt(unsigned int nombre);


unsigned char  x_invertChar(unsigned char nombre)
{
   char temp, invertedNombre;
   int  i, j;

   temp = nombre;
   invertedNombre = (temp & 1);
   for (j=0; j < 7; j++)
      {
      temp = temp >> 1;     
      invertedNombre = (invertedNombre << 1) | (temp & 1);
      }
   return (unsigned char) invertedNombre;
   }

unsigned short  x_invertShort(unsigned short nombre)
{
   short temp, invertedNombre;
   int  i, j;

   temp = nombre;
   invertedNombre = (temp & 1);
   for (j=0; j < 15; j++)
      {
      temp = temp >> 1;     
      invertedNombre = (invertedNombre << 1) | (temp & 1);
      }
   return (unsigned short) invertedNombre;
   }

unsigned int  x_invertInt(unsigned int nombre)
{
   int temp, invertedNombre;
   int  i, j;

   temp = nombre;
   invertedNombre = (temp & 1);
   for (j=0; j < 31; j++)
      {
      temp = temp >> 1;     
      invertedNombre = (invertedNombre << 1) | (temp & 1);
      }
   return (unsigned int) invertedNombre;
   }

