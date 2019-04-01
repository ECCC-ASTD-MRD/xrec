#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <rpnmacros.h>
#include <rec.h>
#include <rec_functions.h>
#include <rpnmacros.h>

#define DEBUG(x) printf(x)

/*
 * A metvar record
 */

typedef struct Metvar 
{
  char *Nomvar;
  char *Description;
  char *Use;
  char *Mesure;
} Metvar, *MetvarPtr;

/*
 * And the code needed to parse it
 */
MetvarPtr parseMetvar(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur) 
{
  MetvarPtr ret = NULL;
  xmlNodePtr Trotteur ;
  
  /* COMPAT xmlChildrenNode is a macro unifying libxml1 and libxml2 names */
  
  printf("nomvar: %s\n",xmlGetProp(cur,"nomvar"));
  
  cur = cur->children;  
  while (cur != NULL) 
    {
    
    /*
      ---------------
      DESCRIPTION
      ---------------
    */
    
    if ( !strcmp((char *)cur->name, "description") ) 
      {
      printf("\tdesc fr:%s\n", xmlGetProp(cur,"fr"));
      printf("\tdesc en:%s\n", xmlGetProp(cur,"en"));
      if ( xmlGetProp(cur,"Originator") )
	printf("\tOriginator:%s\n",xmlGetProp(cur,"Originator"));
      if ( xmlNodeListGetString(doc, cur->children, 1)) 
	printf("\tdesc EXHAUSTIVE: %s\n",xmlNodeListGetString(doc, cur->children, 1) );
      }
    
    /*
      ---------------
      USE
      ---------------
    */
    if ( !strcmp((char *)cur->name, "use") ) 
      {
      /*   ret->Use = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);       */
      if ( xmlGetProp(cur,"usage") )
	printf("\tuse:%s\n",xmlGetProp(cur,"usage"));
      if ( xmlGetProp(cur,"date") )
	printf("\tdate:%s\n",xmlGetProp(cur,"date"));
      }
    
    /*
      ---------------
      DESCRIPTORUSAGE
      ---------------
    */
    
    if ( !strcmp((char *)cur->name,"descriptorUsage") ) 
      {
      Trotteur = cur->children;  /* parcourt tous les enfants  */
      while ( Trotteur )
	{
	if ( !strcmp( (char *)Trotteur->name,"Typvar") ) 
	  {
	  if ( xmlGetProp(Trotteur,"value") )
	    printf("\tdescriptorUsage->TypeVar:%s\n",xmlGetProp(Trotteur,"value"));
	  Trotteur = Trotteur->next;
	  continue;
	  }
	
	if ( !strcmp( (char *) Trotteur->name,"IP1") ) 
	  {
	  if ( xmlGetProp(Trotteur, "value") )
	    printf("\tdescriptorUsage->IP1:%s\n",xmlGetProp(Trotteur,"value"));
	  if ( xmlNodeListGetString(doc, Trotteur->children, 1)) 
	    printf("\tdescriptorUsage->IP1(extra info): %s\n",\
		   xmlNodeListGetString(doc, Trotteur->children,1));
	  Trotteur = Trotteur->next;
	  continue;
	  }
	
	if ( !strcmp( (char *) Trotteur->name,"IP2") ) 
	  {
	  if ( xmlGetProp(Trotteur,"value") ) 
	    printf("\tdescriptorUsage->IP2:%s\n",xmlGetProp(Trotteur,"value"));
	  if ( xmlNodeListGetString(doc, Trotteur->children, 1)) 
	    printf("\tdescriptorUsage->IP2(extra info): %s\n",\
		   xmlNodeListGetString(doc, Trotteur->children,1));
	  Trotteur = Trotteur->next;
	  continue;
	  }
	
	if ( !strcmp( (char *) Trotteur->name,"IP3") ) 
	  {
	  if ( xmlGetProp(Trotteur,"value") ) 
	    printf("\tdescriptorUsage->IP3:%s\n",xmlGetProp(Trotteur,"value"));
	  if ( xmlNodeListGetString(doc, Trotteur->children, 1)) 
	    printf("\tdescriptorUsage->IP3(extra info): %s\n",\
		   xmlNodeListGetString(doc, Trotteur->children,1));
	  Trotteur = Trotteur->next;
	  continue;
	  }
	
	
	}/* end while  */ 
      }
    
    if ( !strcmp( (char *) cur->name, "measure") ) 
      {
      /*
	-------------
	integer
	-------------
      */
      if ( !strcmp( (char *)cur->xmlChildrenNode->name,"integer") ) 
	{
	printf("\tmesure->integer->units:%s\n",xmlGetProp(cur->xmlChildrenNode,"units"));
	if ( xmlGetProp(cur->xmlChildrenNode,"magnitude") )
	  printf("\tmesure->integer->magnitude:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"magnitude"));
	if ( xmlGetProp(cur->xmlChildrenNode,"min") )
	  printf("\tmesure->integer->min:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"min"));
	if ( xmlGetProp(cur->xmlChildrenNode,"max") )
	  printf("\tmesure->integer->max:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"max"));
	}
      /*
	-------------
	real
	-------------
      */
      if ( !strcmp( (char *)cur->xmlChildrenNode->name,"real") ) 
	{
	printf("\tmesure->real->units:%s\n",xmlGetProp(cur->xmlChildrenNode,"units"));
	if ( xmlGetProp(cur->xmlChildrenNode,"magnitude") )
	  printf("\tmesure->real->magnitude:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"magnitude"));
	if ( xmlGetProp(cur->xmlChildrenNode,"min") )
	  printf("\tmesure->real->min:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"min"));
	if ( xmlGetProp(cur->xmlChildrenNode,"max") )
	  printf("\tmesure->real->max:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"max"));
	}
      /*
	-------------
	logical
	-------------
      */
      if ( !strcmp( (char *)cur->xmlChildrenNode->name,"logical") ) 
	{
	
	Trotteur = cur->children;
	while ( Trotteur ) 
	  {
	  printf("\tlogical->value:%s",xmlGetProp(Trotteur,"value"));
	  printf("\tlogical->meaning:%s\n",xmlGetProp(Trotteur,"meaning"));
	  Trotteur = Trotteur->next;
	  }
	}
      /*
	-------------
	code
	-------------
      */
      if ( !strcmp( (char *)cur->xmlChildrenNode->name,"code") ) 
	{
	
	Trotteur = cur->children;
	while ( Trotteur ) 
	  {
	  printf("\tlogical->value:%s",xmlGetProp(Trotteur,"value"));
	  printf("\tlogical->meaning:%s\n",xmlGetProp(Trotteur,"meaning"));
	  Trotteur = Trotteur->next;
	  }
	}
      
      
      }/* end if mesure */
    
    cur = cur->next;
    }
  
  
  return(ret);
}


int  parseJobFile(char *filename) 
{
  xmlDocPtr    doc;
  xmlNsPtr     ns;
  xmlNodePtr   cur;
  MetvarPtr    Metvar;
  xmlDtdPtr    dtd ;
  xmlValidCtxt ctxt;
  FILE *fh;
  
  
  /*
   * build an XML tree from the file;
   */
  doc = xmlParseFile(filename);
  if (doc == NULL) return(1);
  
  /* -----------------------------------------  */
  /*  Check the document is of the right kind   */
  /* ------------------------------------------ */
  
  
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL ) 
    {
    fprintf(stderr,"empty document\n");
    xmlFreeDoc(doc);
    return(1);
    }
  
  
  printf("root:%s\n",cur->name);
  
  /* ---------------------- */
  /*  Parse the DTD!   */
  /* ---------------------- */
  
  dtd = xmlParseDTD(NULL,"/usr/local/env/afsisio/datafiles/constants/dict.dtd"); 
  if (! dtd ) 
    { 
    printf("Error Parsing DTD\n");
    return (1);
    }
  else
    printf ("dtd->name:%s\n",dtd->name);  
  
  /* ----------------------------------------------------------- */
  /*   Set up xmlValidCtxt for error reporting when validating   */
  /* ----------------------------------------------------------- */
  
  ctxt.userData = stderr; 
  ctxt.error    = (xmlValidityErrorFunc) fprintf;   /* register error function */ 
  ctxt.warning  = (xmlValidityWarningFunc) fprintf; /* register warning function */ 
  
  
  if ( xmlValidateDtd(&ctxt,doc,dtd) )
    printf("=========Validation Ok!============\n");
  else 
    {
    printf("========Validation error!===========\n");
    return (1);
    }
  
  
  /*
   * Now, walk the tree.
   */
  
  
  cur = cur->children; 
  while (cur != NULL) {
  Metvar = parseMetvar(doc, ns, cur);  	   
  cur = cur->next;
  }
  
  
  /* Free all */
  
  xmlFreeDtd(dtd);
  xmlFreeDoc(doc);
  return(0);
}

