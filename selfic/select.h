/* #ident "CMC/DOE: %W% %G%" */

/******************************************************************************
*			SELECT.C						*
******************************************************************************/

#define CREATION        True
#define MODIF           False
#define SELECT_EN_COURS	0
#define SELECT_FINIE	1
#define SELECT_DONE     1

static char *label_message[2] = { "Avertissement", "Warning" };

static char *char_select_ok[2] = {"Ok", "Ok" };
static char *char_select_clear[2] = { "Desactiver\nselection", "Clear\nSelection" };
static char *char_select_add[2] = {"Ajouter a\nla liste", "Add \nto List"};
static char *char_select_edit_list[2] = {"Editer\nliste", "Edit\nList"};
static char *char_select_path_list[2] = {"Repertoire", "Path\nList"};
static char *select_tx_message[2] = {
"Nombre de fichiers selectionnes > maximum,\nmaximum de fichiers est retourne",
"Number of selected files > maximum,\n maximum number of files returned" };
typedef struct
{
int     statut_select;          /* = SELECT_FINIE quand on appuie close. */
XEvent  theEvent;               /* Evenment X.          */
Widget  toplevel;                       /* Application Shell */
Widget          message;                /* XmMessageDialog */
int                     etat;   /* indique si message a ete lu */
Widget          form;                   /* XmForm */
Widget                  current_path;           /* XmText */
Widget                  menu_bar;               /* XmMenuBar */
Widget                          pulldown;       /* XmPulldownMenu */
Widget                                  *dir;   /* XmPushButtonGadget */
Widget                          cascade;        /* XmCascadeButton */
Widget                  liste;          /* XmScrolledList */
Widget                  actionform;     /* XmRowColumn */
Widget                          ok;             /* XmPushButtonGadget */
Widget                          clear;          /* XmPushButtonGadget */
Widget                          add_list;       /* XmPushButtonGadget */
Widget                          edit_list;      /* XmPushButtonGadget */
Widget                          path_list;      /* XmPushButtonGadget */
} SelectStruct;
