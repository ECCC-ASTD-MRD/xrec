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
**
** Sous-programme  souris_
** Auteur:         Yves Chartier
** Creation:       Janvier 1990
** Modifications:  Avril 1990
**                 Ajout d'un menu lors du clic du bouton droit de 
**                 la souris.
** Langage:        C
** Env. graphiques: X11, GL
**
**/
#include <rpnmacros.h>

#ifdef X_WGL
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <wgl.h>
#endif

#ifdef GL_WGL
#include <gl.h>
#include <device.h>
#endif

#include <stdio.h>


/** 
** Constantes utilisees pour les evenements retournes par souris 
**/

#define NIL             0
#define CLIC            1
#define DRAG            2
#define MOVE            3
#define RESET           4
#define BACK_CLICK      5

#define ROUND(x)        (int)(x + 0.5)

/** 
** Constantes utilisees pour la gestion interne des evenements
**/

#define CONFIRMATION_RECUE      1
#define CONFIRMATION_NON_RECUE  0

#define RUBBER_RECT      10
#define RUBBER_ARROW     11
#define DeplacementMinimal      15

#ifdef X_WGL

/**
** Variables X globales provenant du fichier wgl_x.c
**/ 

extern  Window                   wglWin;
extern  Display                 *wglDisp;
extern  GC                       wglLineGC, wglFillGC;
extern  XEvent                   wglEvent;
#endif

/**
** Variables globales utilisees dans ce fichier
**/ 

static int ActualColor;
static int   backupx1 = 0, backupx2 = 0, backupy1 = 0, backupy2 = 0;
static int   backupobjet = 0;

/**
** Routines contenues dans ce fichier
**/

int  AnalyserDeplacement();
int  AttendreProchainClic();
int  ClicDansFenetre();
void DefCoordsFenetre();
void EnterOverlayMode();
void TracerObjet();
void initMenu();
int  strclean();
void RespecterAspectRatio();
void RestoreBackupPoints();
void RestoreNormalMode();
void SetBackupPoints();
int  f77name(souris)();
void trackmouse();

/**
 ******************************************************************************
 ** Routine AnalyserDeplacement
 ** Objet:  calcule la distance entre deux points pour determiner si la souris
 **         a ete clickee ou draggee
 ******************************************************************************
 **/

int AnalyserDeplacement(int *x1, int *y1, int *x2, int *y2)
{
   if (abs(*x2 - *x1) < DeplacementMinimal && abs(*y2 - *y1) < DeplacementMinimal)
      return CLIC;
   else
      return DRAG;
   }

/**
 ******************************************************************************
 ** Routine DefCoordsFenetre
 ** Objet:  initialise les variables suivantes:
 **            x0 et y0 aux coordonnees du coin superieur droit 
 **            de la fenetre (en pixels)
 **            xsize et ysize a la largeur et la hauteur de la fenetre
 **            (en pixels)
 **            AspectRatio, le rapport largeur/hauteur de la fenetre
 **            Ce rapport est utilise pour tracer les rectangles lors d'une
 **            commande ZOOM.
 ******************************************************************************
 **/

void DefCoordsFenetre(x0, y0, xsize, ysize, AspectRatio)
int *x0, *y0, *xsize, *ysize; 
float *AspectRatio;
{
   int w,h;

#ifdef X_WGL
   c_wglgwz(&w,&h);
   *xsize = w;
   *ysize = h;
   *x0    = 0;
   *y0    = *ysize;
#endif
   
#ifdef GL_WGL
   getorigin(x0, y0);
   getsize  (xsize, ysize);
#endif
   
   *AspectRatio = (float) (*xsize) / (float) (*ysize);
   }

/**
 ******************************************************************************
 ** Routine EnterOverlayMode
 ** Objet:  met l'affichage en mode XOR.
 ******************************************************************************
 **/

void EnterOverlayMode()
{
#ifdef X_WGL
   c_wgllwi(1);
   XSetFunction(wglDisp, wglLineGC, GXinvert);
   XSetFunction(wglDisp, wglFillGC, GXinvert);
#endif
   
#ifdef GL_WGL
   c_wgllwi(3);
   setpattern(0);
   drawmode(PUPDRAW);
/**
   color(BLACK);
   clear();
**/
   mapcolor (BLACK, 0, 0, 0);
   mapcolor (1, 0, 255, 255);
   color(1);
#endif
   }

/**
 ******************************************************************************
 ** Routine TracerObjet
 ** Objet:  Trace dans la fenetre une figure qui selon le bouton enfonce, 
 **         sera un rectangle (bouton gauche) ou une ligne (bouton du milieu)
 ******************************************************************************
 **/

void TracerObjet(x1, y1, x2, y2, AspectRatio, objet)
int *x1, *y1, *x2, *y2;
float *AspectRatio;
int objet;
{
   switch (objet)
      {
#ifdef GL_WGL
      case RUBBER_RECT:
      RespecterAspectRatio(x1, y1, x2, y2, AspectRatio);
      c_wglrli(*x1, *y1, *x2, *y2);
      break;
      
      case RUBBER_ARROW:
      c_wglmvi(*x1, *y1);
      c_wgldri(*x2, *y2);
      break;
#endif      
#ifdef X_WGL
      case RUBBER_RECT:
      RespecterAspectRatio(x1, y1, x2, y2, AspectRatio);
      c_wglrli(*x1, *y1, *x2, *y2);
      c_wglrli(*x1-1, *y1-1, *x2+1, *y2+1);
      c_wglrli(*x1+1, *y1+1, *x2-1, *y2-1);
      break;
      
      case RUBBER_ARROW:
      c_wglmvi(*x1, *y1);
      c_wgldri(*x2, *y2);

      c_wglmvi(*x1-1, *y1-1);
      c_wgldri(*x2-1, *y2-1);

      c_wglmvi(*x1+1, *y1+1);
      c_wgldri(*x2+1, *y2+1);
      break;
#endif      
      default:
      break;
      }
   }

/**
 ******************************************************************************
 ** Routine initMenu
 ** Objet:  initalise les chaines de caracteres utilisees pour batir le menu
 **         affiche quand le bouton de droite a ete clicke.
 ******************************************************************************
 **/

void initMenu(menustr, menutable, menulength, nmenus)
char *(*menustr);
char menutable[];
int   menulength, nmenus;
{
   char **menuItems;
   int i, j, longueurTotale;
   char *workStr;

   longueurTotale = 0;
   workStr = (char *) calloc(nmenus*menulength+1, sizeof(char));
   strncpy(workStr, menutable, nmenus*menulength);
   workStr[nmenus*menulength] = '\0';

   menuItems = (char **) calloc(nmenus, sizeof(char *));
   for (i=0; i < nmenus; i++)
      {
      workStr[i*menulength+(menulength-1)] = '\0';
      strclean(&(workStr[i*menulength]));
      menuItems[i] = (char *) calloc(strlen(&(workStr[i*menulength]))+5, 
                                     sizeof(char));
      strcpy(menuItems[i], &(workStr[i*menulength]));
   
      if (i == 0)
         strcat(menuItems[i], " %t|");
      else
         {
         if (i == (nmenus - 2))
            strcat(menuItems[i], " %l|");
         else
	    if (i != (nmenus - 1))
	       strcat(menuItems[i], " |");
	    else
	       strcat(menuItems[i], " ");
	 
         }
      
      longueurTotale += strlen(menuItems[i]);
      }
   
   *menustr = (char *) calloc(longueurTotale+1, sizeof(char));
   for (i=0; i < nmenus; i++)
      {
      strcat(*menustr, menuItems[i]);
      free(menuItems[i]);
      }
 
   free(workStr);
   free(menuItems);
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

void RespecterAspectRatio(x1, y1, x2, y2, AspectRatio)
int *x1, *y1, *x2, *y2;
float *AspectRatio;
{
   int  delx, dely;

   delx = *x2 - *x1;
   dely = *y2 - *y1;

   if (abs(dely) <= abs(delx))
      {
      if (dely > 0)
	 {
	 if (delx > 0)
	    *y2 = ROUND(*y1 + delx / *AspectRatio);
	 else
	    *y2 = ROUND(*y1 - delx / *AspectRatio);
	 }
      else
	 {
	 if (delx > 0)
	    *y2 = ROUND(*y1 - delx / *AspectRatio);
	 else
	    *y2 = ROUND(*y1 + delx / *AspectRatio);
	 }
      }
   else
      {
      if (delx > 0)
	 {
	 if (dely > 0)
	    *x2 = ROUND(*x1 + dely * *AspectRatio);
	 else
	    *x2 = ROUND(*x1 - dely * *AspectRatio);
	 }
      else
	 {
	 if (dely > 0)
	    *x2 = ROUND(*x1 - dely * *AspectRatio);
	 else
	    *x2 = ROUND(*x1 + dely * *AspectRatio);
	 }
      }
   }


/**
******************************************************************************
******************************************************************************
**/

void RestoreBackupPoints(x1,y1,x2,y2,backupx1,backupy1,backupx2,backupy2)
int  *x1, *y1, *x2, *y2, *backupx1, *backupy1, *backupx2, *backupy2;
{
   *x1 = *backupx1;
   *y1 = *backupy1;
   *x2 = *backupx2;
   *y2 = *backupy2;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

void RestoreNormalMode()
{
#ifdef X_WGL
   c_wgllwi(1);
   XSetFunction(wglDisp, wglLineGC, GXcopy);
   XSetFunction(wglDisp, wglFillGC, GXcopy);
#endif
   
#ifdef GL_WGL
   drawmode(NORMALDRAW);
   mapcolor (BLACK, 0, 0, 0);
   mapcolor (1,210, 0, 0);
   linewidth(1);
#endif
   }

/**
******************************************************************************
******************************************************************************
**/

void SetBackupPoints(backupx1, backupy1, backupx2, backupy2, x1, y1, x2, y2) 
int  *backupx1, *backupy1, *backupx2, *backupy2, *x1, *y1, *x2, *y2; 
{
*backupx1 = *x1;
*backupy1 = *y1;
*backupx2 = *x2;
*backupy2 = *y2;
}

/**
******************************************************************************
******************************************************************************
**/

int f77name(souris)(int *bouton, int *event, int *x0, int *y0, int *xsize, int *ysize, int *x1, int *y1, int *x2, int *y2, char menuTable[], int *nbmenus, int menulength)
{
   float AspectRatio;
   int   Deplacement, Confirmation;
   static char  *menuTitre, *infoLabel, **menuItems;
   char  *menustr;
   int   item, relx1, rely1;
   int menu;
   
   *event = NIL;
   Confirmation = CONFIRMATION_NON_RECUE;
   backupx1 = 0; backupx2 = 0; backupy1 = 0; backupy2 = 0;

/* YC -- Mars 2014 */
/* On force nbmenus a zero, il y a un bug de Motif qui empeche le menu contextuel apparaissant avec un clic du bouton droit de fonctionner correctement */
/* Ca donnait une erreur du genre "XtGrabPointer failed" */

   *nbmenus = 0;

/* Fin de la patche */

   if (*nbmenus > 0)
      {
      initMenu(&menustr, menuTable, menulength, *nbmenus);
      menu = defpup(menustr);
      }
   
   DefCoordsFenetre(x0, y0, xsize, ysize, &AspectRatio);
   EnterOverlayMode();
   
   while (Confirmation == CONFIRMATION_NON_RECUE && *event != RESET) 
      {
      *bouton = c_wglbti(x1, y1);
      
      switch (*bouton)
	 { 
	 case BGAUCH:
	 trackmouse(bouton, x1, y1, x2, y2, &AspectRatio, RUBBER_RECT);
	 Deplacement = AnalyserDeplacement(x1, y1, x2, y2);
	 switch (Deplacement)
	    {
	    case DRAG:
	    SetBackupPoints(&backupx1, &backupy1, &backupx2, &backupy2,
			    x1,        y1,        x2,        y2); 
	    *event = DRAG;
	    break;
	    
	    case CLIC:
	    if (*event == NIL)
	       *event = CLIC;
	    else
	       RestoreBackupPoints( x1,       y1,       x2,       y2,  
				   &backupx1,&backupy1,&backupx2,&backupy2);
	    Confirmation = CONFIRMATION_RECUE;
	    break;
	    }
         backupobjet = RUBBER_RECT;
	 break;
	 
	 case BMLIEU:
	 trackmouse(bouton, x1, y1, x2, y2, &AspectRatio, RUBBER_ARROW);
	 Deplacement = AnalyserDeplacement(x1, y1, x2, y2);
	 switch (Deplacement)
	    {
	    case DRAG:
	    SetBackupPoints(&backupx1, &backupy1, &backupx2, &backupy2,
			    x1,        y1,        x2,        y2); 
	    *event = MOVE;
	    break;
	    
	    case CLIC:
	    RestoreBackupPoints( x1,       y1,       x2,       y2,  
				&backupx1,&backupy1,&backupx2,&backupy2);
	    if (*event == NIL)
	       *event = BACK_CLICK;
	    
	    Confirmation = CONFIRMATION_RECUE;
	    break;
	    
	    default:
	    break;
	    }
         backupobjet = RUBBER_ARROW;
	 break;
	 
#ifdef X_WGL
	 case BDROIT:
	 switch (*nbmenus)
	    {
	    case 0:
	    *event = RESET;
	    break;

	    default:
	    item = dopopup(1, (int) (wglEvent.xbutton.x_root), (int) (wglEvent.xbutton.y_root));
	    item++;
	    if (item >= 1)
	       {
	       *event = RESET;
	       *x0 = item;
	       }
	    else
	       *event = NIL;
	    break;
	    }
	 break;
#endif
#ifdef GL_WGL
	 case BDROIT:
	 switch (*nbmenus)
	    {
	    case 0:
	    *event = RESET;
	    break;
	    
	    default:
	    item = dopup(menu);
	    if (item != *nbmenus && item >= 1)
	       {
	       *event = RESET; 
	       freepup(menu);
	       *x0 = item;
	       }
	    else
	       *event = NIL;
	    break;
	    }
	 break;
#endif
	 
	 case NIL:
	 *event = NIL;
	 Confirmation = CONFIRMATION_RECUE;
	 break;
	 }
      }
   
#ifdef GL_WGL
   color(BLACK);
   clear();
#endif
   RestoreNormalMode();
   
   if (*event != RESET && *event != NIL)
      {
      *x1 += *x0;
      *y1 += *y0;
      *x2 += *x0;
      *y2 += *y0;
      }
   return 1;
   }

/**
 ******************************************************************************
 ******************************************************************************
 **/

void trackmouse(bouton, x1, y1, x2, y2, AspectRatio, objet)
int *bouton, *x1, *y1, *x2, *y2;
float *AspectRatio;
int    objet;
{
   int StatutSouris;
   int oldx2, oldy2;
   int copiex2, copiey2;
   
#ifdef GL_WGL
   color(0);
   clear();
#endif
#ifdef X_WGL   
   c_wglcol(CYAN);
   TracerObjet(&backupx1, &backupy1, &backupx2, &backupy2, AspectRatio, backupobjet);
#endif

   *x2 = *x1;
   *y2 = *y1;
   oldx2 = *x2;
   oldy2 = *y2;
   
   StatutSouris = 1;
   while (StatutSouris == 1)
      {
      StatutSouris = c_wglbtpi(*bouton, x2, y2);
      if (*x2 != copiex2 || *y2 != copiey2)
	 {
#ifdef GL_WGL
	 color(BLACK);
#endif
	 TracerObjet(x1, y1, &oldx2, &oldy2, AspectRatio, objet);
#ifdef GL_WGL
	 color(1);
#endif
	 copiex2 = *x2; copiey2 = *y2;
	 TracerObjet(x1, y1, x2, y2, AspectRatio, objet); 
	 oldx2 = *x2;
	 oldy2 = *y2;
	 }
      }
   *x2 = oldx2;
   *y2 = oldy2;
   }



/**
******************************************************************************
******************************************************************************
**/

int f77name(minisouris)(bouton, event, x0, y0, xsize, ysize, x1, y1, x2, y2)
int *bouton, *event;
int *x0, *y0, *xsize, *ysize;
int *x1, *y1, *x2, *y2;
{
   float AspectRatio;
   int   Deplacement, Confirmation;
   static char  *menuTitre, *infoLabel, **menuItems;
   char  *menustr;
   int   item, relx1, rely1;
   int menu;
   
   *event = NIL;
   Confirmation = CONFIRMATION_NON_RECUE;
   backupx1 = 0; backupx2 = 0; backupy1 = 0; backupy2 = 0;
   
   DefCoordsFenetre(x0, y0, xsize, ysize, &AspectRatio);
   EnterOverlayMode();
   
   while (Confirmation == CONFIRMATION_NON_RECUE && *event != RESET) 
      {
      *bouton = c_wglbti(x1, y1);
      
      switch (*bouton)
	 { 
	 case BGAUCH:
	 trackmouse(bouton, x1, y1, x2, y2, &AspectRatio, RUBBER_ARROW);
	 Deplacement = AnalyserDeplacement(x1, y1, x2, y2);
	 switch (Deplacement)
	    {
	    case DRAG:
	    SetBackupPoints(&backupx1, &backupy1, &backupx2, &backupy2,
			    x1,        y1,        x2,        y2); 
	    *event = MOVE;
	    break;
	    
	    case CLIC:
	    if (*event == NIL)
	       *event = CLIC;
	    else
	       RestoreBackupPoints( x1,       y1,       x2,       y2,  
				   &backupx1,&backupy1,&backupx2,&backupy2);
	    Confirmation = CONFIRMATION_RECUE;
	    break;
	    }
         backupobjet = RUBBER_ARROW;
	 break;
	 
	 case BMLIEU:
	 trackmouse(bouton, x1, y1, x2, y2, &AspectRatio, RUBBER_ARROW);
	 Deplacement = AnalyserDeplacement(x1, y1, x2, y2);
	 switch (Deplacement)
	    {
	    case DRAG:
	    SetBackupPoints(&backupx1, &backupy1, &backupx2, &backupy2,
			    x1,        y1,        x2,        y2); 
	    *event = MOVE;
	    break;
	    
	    case CLIC:
	    RestoreBackupPoints( x1,       y1,       x2,       y2,  
				&backupx1,&backupy1,&backupx2,&backupy2);
	    if (*event == NIL)
	       *event = BACK_CLICK;
	    
	    Confirmation = CONFIRMATION_RECUE;
	    break;
	    
	    default:
	    break;
	    }
         backupobjet = RUBBER_ARROW;
	 break;
	 
	 case BDROIT:
	 *event = RESET;
	 break;
	 
	 case NIL:
	 *event = NIL;
	 Confirmation = CONFIRMATION_RECUE;
	 break;
	 }
      }
   
#ifdef GL_WGL
   color(BLACK);
   clear();
#endif
   RestoreNormalMode();
   
   if (*event != RESET && *event != NIL)
      {
      *x1 += *x0;
      *y1 += *y0;
      *x2 += *x0;
      *y2 += *y0;
      }
   return 1;
   }




