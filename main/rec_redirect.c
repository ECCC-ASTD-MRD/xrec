#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpnmacros.h>

// Redirige stdio dans le fichier "nom_fichier"
// Yves Chartier, mars 2014

f77name(rec_redirect)(char *nom_fichier, F2Cl flen)
{
int i,bak, new;
int len=flen;
char nomfich[256];
strcpy(nomfich, nom_fichier);
i = len - 1;
while(isspace(nomfich[i]) && i > 0)
   {
   nomfich[i] = '\0';
   i--;
   }

printf("Redirection STDIO -> %s\n", nomfich);
fflush(stdout);
bak = dup(1);
new = open(nomfich, O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
dup2(new, 1);
}


