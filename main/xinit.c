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

/**
 ** fichier xinit.c
 **
 ** Auteur: Yves Chartier (RPN)
 **
 ** Creation: Avril 1990
 ** Dernieres modifications: 28 mai 1990
 **/

#include <xinit.h>
#include <ctype.h>
#include <rpnmacros.h>

#define DEFAULT_RESOURCE_DIR "/usr/local/env/armnlib/data"
#define INITIALISATION_COMPLETEE 101

static String RessourcesDeDefaut[] = {  NULL, };
SuperWidgetStruct SuperWidget = { NULL, NULL };
Widget   xglTopLevel, xglBox, xglCoreWidget;
static char *defaultResourceDir = NULL;
  
/**
************************************************************
************************************************************
**/

int f77name(xinit)(nomApplication, lenNomApplication)
char nomApplication[];
int  lenNomApplication;
{
   char copieNomApplication[256];
   
   strncpy(copieNomApplication, nomApplication, lenNomApplication);
   copieNomApplication[lenNomApplication] = '\0';

   Xinit(copieNomApplication);
    
   }

/**
************************************************************
************************************************************
**/

int Xinit(nomApplication)
char nomApplication[];
{
   Arg args[10];
   static char **argv;  
   static char *classeApplication;  
   static int   statutInitialisation;
   static char xapplresdir[128];
   static char *envvar;
   int i; 
   int argc = 1;

   if (SuperWidget.topLevel != NULL)
      return 1;
   else
      {
      envvar = (char *) getenv("ARMNLIB");
      if (NULL == envvar)
	 {
	 printf("La variable d'environnement ARMNLIB n'est pas definie...\n Impossible de continuer\n");
	 exit(-1);
	 }
      
      strcpy(xapplresdir, "XAPPLRESDIR=");
      strcat(xapplresdir, (char *) (getenv("ARMNLIB")));
      strcat(xapplresdir, "/data");
	
      envvar = (char *) xapplresdir;
      putenv(envvar);

      argv = (char **) calloc(1, sizeof(char *));
      argv[0] = (char *) calloc(strlen(nomApplication)+1, sizeof(char));
      strcpy(argv[0], nomApplication);
      
      classeApplication = (char *) calloc(strlen(nomApplication)+1, sizeof(char));
      strcpy(classeApplication, nomApplication);
      
      classeApplication[0] = (char)(toupper(classeApplication[0]));
      classeApplication[1] = (char)(toupper(classeApplication[1]));
      
      i = 0;
      SuperWidget.topLevel = XtAppInitialize(&(SuperWidget.contexte), 
					     classeApplication, NULL, 0, 
					     &argc, argv, RessourcesDeDefaut, 
					     args, i);
      free(argv[0]);
      free(argv);
      free(classeApplication);
      statutInitialisation = INITIALISATION_COMPLETEE;
      return 1;
      }
   }


