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

#include <wgl.h>
#include <gd_wgl.h>
#include <x_wgl.h>

f77name(wglscon)(int *contexte)
{
  wglscon(int *contexte);
}

wglscon(int contexte)
{
  switch (contexte)
    {
    case x_wgl:
      wglscon_x();
      break;
      
    case gl_wgl:
      wglscon_gl();
      break;
      
    case gd_wgl:
      wglscon_gd();
      break;
      
    case ps_wgl:
      wglscon_ps();
      break;
    }
}
