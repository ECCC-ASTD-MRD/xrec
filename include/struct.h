#include <Xm/Xm.h>
#include <string.h>


#define NB_MAX_DIRS        32

/**
 ** Structure utilisee dans le selecteur de fichiers 
 **/

typedef struct
{
   int  StatutSelection;
   char FichierSelectionne[256];
   XmString *FichiersSelectionnes;
   int  nbFichiersSelectionnes;
   char DirCourant[256];
   char **listeDesFichiers;
   char **ListeDir;
   char *message[2];
   int  nbFichiers, NbDirs;
   int  indTopListe;
   Widget topLevel, form, menu, liste, formListe, apply, apClose, close, editerListe;
   Widget labelMessage, labelDirCourant;
   Widget menuL, menuListe;
   Widget menuItems[NB_MAX_DIRS];
   Pixmap maison;
   XEvent theEvent;
   } XSelecteurFichierStruct;

