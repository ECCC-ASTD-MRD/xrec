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

/* #ident "CMC/DOE: %W% %G%" */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

 void freelist ();
 int  isdir    ();
 int  isfile   ();
 int  lsdir    ();
 void pathinfo ();
 int  scan_dir ();
 int  strmatch ();
 int  strsplit ();

 static char   *match;
 static char   *rejet;

 static int    selection();
 static int    triage   ();


/*
 *
 *  module    :  FREELIST
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  freelist ( list )
 *               char ***list;
 *
 *  objet     :  CE MODULE FAIT UNE DESALLOCATION DE MEMOIRE DE LA LISTE
 *
 *  note      :  LA PREMIERE ENTREE DE LA LISTE EST L'ADRESSE MERE
 *               DE TOUS LES ELEMENTS DE LA LISTE
 *
 */

 void freelist ( list )
 char ***list;
     {
     if( *list == NULL ) return;
     free((*list)[-1]); 
     free(--(*list));
     *list = NULL;
     }


/*
 *
 *  module    :  ISDIR
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  isdir ( path )
 *               char *path;
 *
 *  objet     :  CE MODULE DETERMINE SI LE PATH EST UN REPERTIORE
 *
 */

 int isdir ( path )
 char *path;
     {
     struct stat buf;

     if( path == NULL ) return(0);
     if( stat(path,&buf) != 0 ) return(0);
     if( (buf.st_mode & S_IFMT) != S_IFDIR ) return(0);

     return(1);
     }


/*
 *
 *  module    :  ISFILE
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  isfile ( path )
 *               char *path;
 *
 *  objet     :  CE MODULE DETERMINE SI LE PATH EST UN FICHIER
 *
 */

 int isfile ( path )
 char *path;
     {
     struct stat buf;

     if( path == NULL ) return(0);
     if( stat(path,&buf) != 0 ) return(0);
     if( (buf.st_mode & S_IFMT) == S_IFDIR ) return(0);

     return(1);
     }


/*
 *
 *  module    :  LSDIR
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  nb = lsdir ( path, reject, select, list )
 *
 *  arguments :  int  nb;       * nombre d'elements dans la liste *
 *               char *path;    * le path du repertoire           *
 *               char *reject;  * une chaine wild de rejet        *
 *               char *select;  * une chaine wild de preference   *
 *               char ***list;  * la liste des elements           *
 *
 *  objet     :  CE MODULE FAIT UN LS D'UN REPERTIORE
 *
 *  note      :  LA PREMIERE ENTREE DE LA LISTE EST L'ADRESSE MERE
 *               DE TOUS LES ELEMENTS DE LA LISTE
 *
 */

 int lsdir ( path, reject, select, list )
 char *path;
 char *reject;
 char *select;
 char ***list;
     {
     int    triage();
     int    selection();
     int    nb, test;
/*
 *  determine s'il y aura besoin de selection
 */
     test = reject != NULL || select != NULL;

/*
 *  si on a un menage a faire sur les fichiers on l'indique
 */
    match = select;
    rejet = reject;

/*
 *  va chercher la liste des elements dans le repertoire
 */
     nb = scan_dir(path, list, test ? selection : NULL, triage);

     return(nb);
     }


/*
 *
 *  module    :  PATHINFO
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  pathinfo ( path, nword, words, nfile, list )
 *
 *  arguments :  char *path;    * le path du repertoire           *
 *               int  *nword;   * le nombre de mots du path       *
 *               char ***words; * les mots constituant le path    *
 *               int  *nfile;   * le nombre de fichiers dans path *
 *               char ***list;  * les noms de fichiers dans path  *
 *
 *  objet     :  CE MODULE EXTRAIT DES INFORMATIONS DU REPERTOIRE PATH
 *
 */

 void pathinfo ( path, nword, words, nfile, list )
 char *path;
 int  *nword;
 char ***words;
 int  *nfile;
 char ***list;
      {
      char *pos;
      char *sel;
      char select[256];
      char work[256];
      int  i,j;
/*
 *  initialisation
 */
      *nword = 0;
      *nfile = 0;
      freelist( words );
      freelist( list );
      sel = NULL;
/*
 * si le path a des wild cards au nom de fichiers
 * on conserve le nom de fichier dans select
 */
      pos = strrchr(path,(int)'/');
      if( pos != NULL && 
        ( strrchr(pos,(int)'?') != NULL || strrchr(pos,(int)'*') != NULL ))
        {
        strcpy(select,&pos[1]);
        sel = select;
        if( pos != path ) pos[0] = '\0';
        if( pos == path ) pos[1] = '\0';
        }

/*
 *  construit une chaine de travail
 */
      work[0] = '\0';
      if( path[0] == '/' ) strcat( work, "a" );
      strcat( work, path );

/*
 *  on commence par briser le path en ses mots
 */
      *nword = strsplit( work, "/ ", words );
      if( path[0] == '/' ) (*words)[0][0] = path[0];

/*
 *  si le path est un fichier on a plus rien a faire
 *  et le dernier element de words devrait etre null
 */
      if( isfile(path) == 1 )
        {
        *nword--;
        return;
        };
/*
 *  si le path est un repertoire on extrait ses descendants
 */
      if( isdir(path) != 1 ) return;
      *nfile = lsdir( path, ".*", sel, list );

/*
 *  si le descendant du path est un repertoire on ajoute un / a son nom
 */
      for( i=0; i< *nfile; i++ )
         {
         work[0] = '\0';
         strcat( work, path );
         strcat( work, "/" );
         strcat( work, (*list)[i] );
         if( isdir(work) == 1 )
           {
           j = strlen( (*list)[i] );
           (*list)[i][j]   = '/';
           (*list)[i][j+1] = '\0';
           };
         };

      return;
      }


/*
 *
 *  module    :  SCAN_DIR
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  scan_dir ( dir, list, select, compar )
 *               char *dir;
 *               char ***list;
 *               int (*select)();
 *               int (*compar)();
 *
 *  objet     :  CE MODULE FAIT UN SCANDIR SUR UN REPERTOIRE
 *
 */
 int
 scan_dir(dir, list, select, compar)
 char *dir;
 char ***list;
 int (*select)();
 int (*compar)();
     {
     char  *elems;
     char  *prev;
     int    esize, i, ipos, nb = 0;
     struct dirent *ent;
     DIR   *dirp;

/*
 *  ouverture du repertoire
 */
     if( (dirp = opendir(dir)) == NULL ) return(0);

/*
 *  si la liste est non NULL on la vide
 */
     if( *list != NULL ) freelist( list );

/*
 *  la premiere entree de la liste sera l'adresse mere
 *  des elements de la liste
 */
     *list = (char **) malloc( (nb+2)*sizeof(char *) );
     elems = (char * ) malloc( sizeof(int) );
     (*list)[0] = elems;
     prev = elems;
     ipos = 0;

/*
 *  boucle sur toutes les entrees du repertoire
 */
     ent = readdir(dirp);
     while (ent != NULL)
          {
/*
 *  verifie si l'entree fait partie de la selection
 */
          if (!(*select)(ent)) continue;

/*
 *  reallocation de la memoire pour la liste d'elements
 */
          nb += 1;
          *list = (char **) realloc( *list, (nb+2)*sizeof(char *) );
/*
 *  reallocation de la memoire pour le prochain element
 */
          esize = ipos + sizeof(int) + ent->d_reclen ;
          elems = (char *) realloc( elems, esize );
/*
 *  si l'adresse a change on mets notre liste a jour
 */
         if( elems != prev )  
           {  
           for(i=0;i<=nb;(*list)[i]+=elems-prev,i++);
           prev = elems;
           }  
/*
 *  conserve l'element dans notre adresse mere
 */
         strncpy( &elems[ipos], ent->d_name, ent->d_reclen );
         elems[ipos+ent->d_reclen] = '\0';
         (*list)[nb] = &elems[ipos];
         ipos += ent->d_reclen + sizeof(int);

         ent = readdir(dirp);
         }

/*
 *  fermeture du repertoire
 */
    closedir(dirp);

/*
 *  le dernier element sera NULL
 */
     (*list)[nb+1] = NULL;

/*
 *  falcifie l'adresse de la liste pour rendre
 *  l'adresse mere des elements transparents
 */
     (*list)++;

/*
 *  si on a pas trouve d'entree on libere l'espace pris
 */
    if( nb == 0 ) 
      freelist( list );

/*
 * si non on procede au classement
 */
    else
      qsort( *list, nb, sizeof(char **), compar);

    return(nb);
    }


/*
 *
 *  module    :  SELECTION
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  ierr = selection ( d )
 *
 *  arguments :  int  ierr;         * ierr = 0  alors pas selectionne *
 *               struct dirent *d;  * la chaine a comparer            *
 *
 *  objet     :  CE MODULE FAIT LA COMPARAISON D'UNE CHAINE AVEC DEUX
 *               EXPRESSIONS  REJET ET MATCH . DEPENDAMENT SI LE FICHIER
 *               REPONDS A L'UNE OU L'AUTRE DES EXPRESSIONS LE FICHIER
 *               SERA REJETE OU CONSERVER.
 *
 */

 static int
 selection(d)
 struct dirent *d;
    {
return((int)d->d_name);
    if( rejet != NULL && strmatch(d->d_name,rejet)==1 ) return(0);
    if( match != NULL && strmatch(d->d_name,match)==1 ) return((int)d->d_name);
    if( match == NULL ) return((int)d->d_name);

    return(0);
    }

/*
 *
 *  module    :  STRMATCH
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  ierr = strmatch ( str, wild )
 *
 *  arguments :  int  ierr;     * ierr !=0 str corresponds a wild *
 *               char *str;     * la chaine a comparer            *
 *               char *wild;    * la chaine contenant des wild car*
 *
 *  objet     :  CE MODULE FAIT LA COMPARAISON D'UNE CHAINE AVEC UNE 
 *               EXPRESSION CONTENANT DES CARACTERES DE REMPLACEMENT.
 *               LES WILD CHAR SONT * POUR N'IMPORTE QUELLE CARACTERES
 *               OU ? POUR EXACTEMENT UN CARACTERE
 *
 *  note      :  DANS UN PREMIER TEMPS IN SEPARE LA CHAINE WILD EN SES 
 *               CARACTERES SIGNIFICATIFS. ON CHERCHERA A SAVOIR SI LA
 *               CHAINE POSSEDENT TOUS CES ELEMENTS. PUIS ON VERIFIRA
 *               S'IL CORRESPONDS EXACTEMENT A LA DEFINITION DEMANDEE.
 *
 */

 int strmatch ( str, wild )
 char *str;
 char *wild;
      {
      int  i=0,ierr=0,nb;
      char *pstr  = str;
      char *pwil  = wild;
      char *last  = wild;
      char **list = NULL;

/*
 *  verifie la validite des 2 chaines
 */
      if( str  == NULL || strlen(str)  == 0 ||
          wild == NULL || strlen(wild) == 0  ) return(ierr);

/*
 *  verifie si les deux chaines sont identiques
 */
      if( strcmp(str,wild) == 0 ) return(1);

/*
 *  extraction des sous chaines significatives de wild
 */
      nb = strsplit( wild, "*?", &list );

/*
 *  pas de sous chaine a verifier 
 *  alors * match n'importe quoi
 *  alors ? match une longueur exacte
 */
      if( nb == 0 )
        {
        if( strchr(wild,'*') != NULL ) return(1);
        if( strlen(str) == strlen(wild)) return(1);
        goto exit;
        }

/*
 *  boucle sur la chaine: wild
 */
      while( pwil < &wild[strlen(wild)] )
        {
        switch( *pwil )
          {
/*
 *  le caractere de wild est ? on saute le caractere dans str et wild
 */
          case '?' : last = pwil; pstr++; pwil++; break;
/*
 *  le caractere de wild est * on le saute
 */
          case '*' : for(;*pwil=='*' || *pwil=='?';pwil++); 
                     if( (pstr = strstr(pstr,list[i])) == NULL ) goto exit;
                     last = pwil-1;
                     break;
/*
 *  tente de trouver une chaine significatif de wild dans str
 */
          default  : if( strncmp(pstr,list[i],strlen(list[i])) != 0  ||
                         *last == '*' && strstr(pstr,list[i]) == NULL )
                         goto exit;
                     last = pwil;
                     pstr += strlen(list[i]);
                     pwil += strlen(list[i++]);
                     break;
           };
        };

/*
 *  jusqu'ici tout est correcte et on verifie si on en est a la fin de str
 */
        if( pstr != &str[strlen(str)] && *last!='*' ) goto exit;
        ierr = 1;
/*
 *  ici on a le resultat du test
 */
      exit:

      if( list != NULL ) freelist( &list );
      return(ierr);
      }


/*
 *
 *  module    :  STRSPLIT
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  nb = strsplit ( str, delim, list )
 *
 *  arguments :  int  nb;       * nombre d'elements dans la liste *
 *               char *str;     * la chaine a separer             *
 *               char *delim;   * la liste des separateurs        *
 *               char ***list;  * la liste des elements           *
 *
 *  objet     :  CE MODULE FAIT LA SEPARATION D'UNE CHAINE AU POINT DE
 *               DE RENCONTRE AVEC UN DES CARACTERES DE DELIM. LES CHAINES
 *               RESULTANTES SONT PLACEES DANS LIST.
 *
 *  note      :  LA PREMIERE ENTREE DE LA LISTE SERA DE FACON TRANSPARENTE
 *               L'ADRESSE MERE DE TOUS LES ELEMENTS DE LA LISTE
 *
 *               dup = L'ADRESSE MERE DES ELEMENTS
 *
 */

 int strsplit ( str, delim, list )
 char *str;
 char *delim;
 char ***list;
      {
      int   n;
      char *dup;

/*
 *  si la liste est non NULL on la vide
 */
     if( *list != NULL ) freelist( list );

/*
 *  place l'adresse mere des elements au debut de la liste
 */
      n = 1;
      dup = strdup(str);
      *list = (char **) malloc ( sizeof(char *) );
      (*list)[0] = dup;

/*
 *  separe la chaine
 */

      do {
         *list = (char **) realloc ( *list, (n+1)*sizeof(char *) );
         (*list)[n] = strtok( n==1?dup:NULL, delim );
         }
      while( (*list)[n++] != NULL );

/*
 *  falcifie l'adresse de la liste pour rendre
 *  l'adresse mere des elements transparents
 */
     (*list)++;

/*
 *  retourne le nombre d'elements 
 */
      return(n-=2);
      }

/*
 *
 *  module    :  TRIAGE
 *
 *  auteur    :  MICHEL GRENIER
 *
 *  revision  :  V0.0                 
 *
 *  status    :  DEVELOPPEMENT        
 *
 *  langage   :  C
 *
 *  call      :  equal = triage ( d1, d2 )
 *
 *  arguments :  char **d1;     * la 1ere  chaine a comparer          *
 *               char **d2;     * la 2ieme chaine des separateurs     *
 *
 *  objet     :  CE MODULE FAIT LA COMPARAISON DE DEUX CHAINES
 *
 */
 static int
 triage(d1, d2)
 char **d1, **d2;
     {
     return( strcmp(*d1,*d2) );
     }
