/******************************************************************************
*			EDIT.C						*
******************************************************************************/

#define CREATION        True
#define MODIF           False
#define EDIT_EN_COURS	0
#define EDIT_FINIE	1

static char *application[2] = { "Editeur de liste", "List editor" };

static char *char_edit_ok[2] = {"Ok", "Ok" };
static char *char_edit_clear[2] = { "Annuler", "Deselect" };
static char *char_edit_del[2] = {"Enlever", "Delete"};
static char *char_edit_delall[2] = {"Effacer", "Clear"};

typedef struct
{
int     statut_edit;            /* = EDIT_FINIE quand on appuie close. */
XEvent  theEvent;               /* Evenment X.          */
Widget  toplevel;                       /* Application Shell */
Widget          form;                   /* XmForm */
Widget                  actionform;     /* XmRowColumn */
Widget                          ok;             /* XmPushButtonGadget */
Widget                          clear;          /* XmPushButtonGadget */
Widget                          enlever;        /* XmPushButtonGadget */
Widget                          enlever_tout;   /* XmPushButtonGadget */
Widget                  liste;          /* XmScrolledList */
} EditStruct;
