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

static char *labelOpAnul[] = {"\n\
 Operation refusee.\n\
 Cette operation detruirait un champ que vous avez modifie.\n\
 Pour continuer, vous devez:\n\
   a) enregistrer le champ modifie, ou\n\
   b) annuler toutes vos modifications\n\n", "\
 Operation refused.\n\
 This operation would destroy a field that you have edited.\n\
 To continue, you can:\n\
    a) save the edited field, or\n\
    b) cancel all your modifications\n\n"};

RefuserOperation()
{
   int lng;
   
   lng = c_getulng();

   MessageAvertissement(labelOpAnul[lng], AVERTISSEMENT);
   }
