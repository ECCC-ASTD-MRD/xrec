/*
 *
 *  file      :  VCAR.H
 *
 *  auteur    :  MICHEL GRENIER    DDO    AOUT 1988
 *
 *  revision  :  V0.0                     AOUT 1988
 *               V1.0                     NOV  1991
 *
 *  status    :  DEVELOPPEMENT
 *
 *  langage   :  C
 *
 *  objet     :  CE FICHIER CONTIENT LES DECLARATIONS NECESSAIRES POUR
 *               L'UTILISATION DE vcar.o
 *
 */

#ifndef  vcar_DEFINED
#define  vcar_DEFINED

 enum    VCAR_OPTIONS
                { ANGLE = 1,
                  CENTRE,
                  FONTE,
                  HAUTEUR,
                  LARGEUR,
                  YMIROIR
                };

 typedef struct { int      angle;
                  int      centre;
                  int      fonte;
                  int      hauteur;
                  int      largeur;
                  int      ymiroir;
                } Vcarop;

 typedef struct { int      pen;
                  int      x;
                  int      y;
                } PointP2;

 typedef struct { int      largeur;
                  int      npnts;
                  PointP2 *vecteurs;
                } Vcar;

/* extern  void     get_vcar(char *chaine, ...);*/
 extern  void     gen_vcar();
 extern  void     lir_vcar();
/* extern  void     set_vcar(char *chaine);*/
 extern  Vcar    *vcar();

#endif
