/* #ident "CMC/DOE: %W% %G%" */

/******************************************************************************
*			REPERTOIRE.H						*
******************************************************************************/

#define REPERTOIRE_EN_COURS	0
#define REPERTOIRE_FINIE	1
#define NB_DIRECTORYS          15

static char *application_repertoire[2] = { "Liste des repertoires", "Directory list" };


static char *char_repertoire_ok[2] = {"Ok", "Ok" };
static char *char_repertoire_annuler[2] = { "Annuler", "Deselect" };
static char *char_repertoire_detruire[2] = {"Detruire", "Delete"};
static char *char_repertoire_sauver[2] = {"Sauver", "Save"};
static char *char_repertoire_inserer[2] = {"Inserer", "Insert"};

typedef struct
{
int     statut_repertoire;      /* = REPERTOIRE_FINIE quand on appuie close. */
XEvent  theEvent;               /* Evenment X.          */
Widget  toplevel;                       /* Application Shell */
Widget          message;                /* XmMessageDialog */
Widget          form;                   /* XmForm */
Widget                  actionform;     /* XmRowColumn */
Widget                          ok;             /* XmPushButtonGadget */
Widget                          annuler;        /* XmPushButtonGadget */
Widget                          inserer;        /* XmPushButtonGadget */
Widget                          sauver;         /* XmPushButtonGadget */
Widget                          detruire;       /* XmPushButtonGadget */
Widget                  liste;          /* XmScrolledList */
} RepertoireStruct;
