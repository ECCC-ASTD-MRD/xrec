
#define LIGNE   86  /* nb de car entrant ds une ligne de texte */
#define NB_MAX_RECS 4000    /* nb max. de records permis. */


/* Les constantes suivantes sont utilisees pour
   construire le tableau d'enregistrements */

#define LENGTH  0               /* indice de longueur des chaines de 
			   	   car d` un descripteur donne */
#define OFFSET  1               /* indice de la distance (en bytes) entre
                                   chaque element d'un descripteur */
#define ADRESSE 2               /* indice de l'adresse du premier element
                                                 d'un descripteur */

/**
 **  Boutons du selecteur d'enregistrements
 **/

static char
 *label_desact[] = {"Effacer\nselection", "Clear\nselection"},
 *label_efface[] = {"Effacer crit.\nselection", "Erase sel.\ncriteria"},
 *label_nb[] = {"n: ", "n: "},
 *label_retour[] = {"Retour\n", "Return\n"};

