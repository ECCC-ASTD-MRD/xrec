#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <rec.h>

#define DEBUG(x) printf(x)


/* External variable for validating DTD */

extern int xmlDoValidityCheckingDefaultValue;

int nbChampsDict = 0;
_InfoChamps *infoChamps = NULL;

int main(int argc, char **argv) 
{
  
  xmlDoValidityCheckingDefaultValue = 1;
  
  /* COMPAT: Do not generate nodes for formatting spaces */
  
  LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

  infoChamps = (_InfoChamps *)calloc(16, sizeof(_InfoChamps));

  
  if ( LireDictionnaireCMC(argv[1]) ) 
    {
    printf("Failure : \n");
    return(1);
    }
  
  return(0);
}
