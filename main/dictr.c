/*
 * dict.c 
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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
MetvarPtr parsMetvar(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur) 
{
  MetvarPtr ret = NULL;
  xmlNodePtr Trotteur ;
  
  printf("nomvar: %s\n",xmlGetProp(cur,"nomvar"));
  
  cur = cur->children;  
  while (cur != NULL) 
    {
    if ( !strcmp(cur->name, "description") ) 
      {
      /*  ret->Description = xmlNodeListGetString(doc, cur->children, 1); */
      printf("\tdesc fr:%s\n", xmlGetProp(cur,"fr"));
      printf("\tdesc en:%s\n", xmlGetProp(cur,"en"));
      if ( xmlNodeListGetString(doc, cur->children, 1)) 
	{
	printf("\tdesc EXHAUSTIVE: %s\n",xmlNodeListGetString(doc, cur->children, 1) );
	}
      }
    
    if ( !strcmp(cur->name, "use") ) 
      {
      /*   ret->Use = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);       */
      if ( xmlGetProp(cur,"usage") )
	printf("\tuse:%s\n",xmlGetProp(cur,"usage"));
      if ( xmlGetProp(cur,"date") )
	printf("\tdate:%s\n",xmlGetProp(cur,"date"));
      }
    
    if ( !strcmp(cur->name,"descriptorUsage") ) 
      {
      
      if ( cur->children )
	{
	if ( xmlGetProp(cur->children,"TypeVar") )
	  printf("\tdescriptorUsage->TypeVar:%s\n",xmlGetProp(cur->children,"TypeVar"));
	Trotteur = cur->children->children;
	while ( Trotteur ) 
	  {
	  if ( !strcmp(Trotteur->name,"IP1") ) 
	    {
	    if ( xmlGetProp(Trotteur, "ip1") )
	      printf("\tdescriptorUsage->TypeVar->IP1:%s\n",xmlGetProp(Trotteur,"ip1"));
	    if ( xmlNodeListGetString(doc, Trotteur->children, 1)) 
	      printf("\tdescriptorUsage->TypeVar->IP1(extra info): %s\n",\
		     xmlNodeListGetString(doc, Trotteur->children,1));
	    }
	  
	  if ( !strcmp(Trotteur->name,"IP2") )
	    {
	    if ( xmlGetProp(Trotteur,"ip2") )
	      printf("\tdescriptorUsage->TypeVar->IP2:%s\n",xmlGetProp(Trotteur,"ip2"));
	    if ( xmlNodeListGetString(doc, Trotteur->children, 1)) 
	      printf("\tdescriptorUsage->TypeVar->IP2(extra info): %s\n",\
		     xmlNodeListGetString(doc, Trotteur->children,1));
	    }
	  
	  if ( !strcmp(Trotteur->name,"IP3") ) 
	    {
	    if ( xmlGetProp(Trotteur,"ip3") )
	      printf("\tdescriptorUsage->TypeVar->IP3:%s\n",xmlGetProp(Trotteur,"ip3"));
	    if ( xmlNodeListGetString(doc, Trotteur->children, 1)) 
	      printf("\tdescriptorUsage->TypeVar->IP3(extra info): %s\n",\
		     xmlNodeListGetString(doc, Trotteur->children,1));
	    }
	  Trotteur=Trotteur->next;
	  }
	}
      }
    
    if ( !strcmp(cur->name, "mesure") ) 
      {
      /*
	-------------
	UNIT
	-------------
      */
      if ( !strcmp(cur->xmlChildrenNode->name,"unit") ) 
	{
	printf("\tmesure->unit->unit_value:%s\n",xmlGetProp(cur->xmlChildrenNode,"unit_value"));
	if ( xmlGetProp(cur->xmlChildrenNode,"decimal_factor") )
	  printf("\tmesure->unit->decimal_factor:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"decimal_factor"));
	if ( xmlGetProp(cur->xmlChildrenNode,"min") )
	  printf("\tmesure->unit->min:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"min"));
	if ( xmlGetProp(cur->xmlChildrenNode,"max") )
	  printf("\tmesure->unit->max:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"max"));
	}
      /*
	-------------
	NO_UNIT
	-------------
      */
      if ( !strcmp(cur->xmlChildrenNode->name,"No_unit") ) 
	{ 
	printf("\tmesure->No_unit->validityRange:%s\n", xmlGetProp(cur->xmlChildrenNode,"validityRange")); 
	printf("\tmesure->No_unit->ref:%s\n", xmlGetProp(cur->xmlChildrenNode,"ref")); 
	
	if ( xmlGetProp(cur->xmlChildrenNode,"objectOfRatio") )
	  printf("\tmesure->No_unit->objectOfRatio:%s\n", 
		 xmlGetProp(cur->xmlChildrenNode,"objectOfRatio"));
	
	if ( !strcmp(xmlGetProp(cur->xmlChildrenNode,"validityRange"),"integerRange" ) ||
	     !strcmp(xmlGetProp(cur->xmlChildrenNode,"validityRange"),"realRange" ) ) 
	  {
	  printf("\tmesure->No_unit->min:%s\n", xmlGetProp(cur->xmlChildrenNode,"min"));
	  printf("\tmesure->No_unit->max:%s\n", xmlGetProp(cur->xmlChildrenNode,"max"));
	  
	  } 
	else if (!strcmp(xmlGetProp(cur->xmlChildrenNode,"validityRange"),"logicalValues")) 
	  {
	  Trotteur = cur->children->children;
	  while ( Trotteur ) 
	    {
	    printf("\tlogical->val1:%s",xmlGetProp(Trotteur,"val1"));
	    printf("\tlogical->val2:%s\n",xmlGetProp(Trotteur,"val2"));
	    Trotteur = Trotteur->next;
	    }
	  
	  } 
	else if (!strcmp(xmlGetProp(cur->xmlChildrenNode,"validityRange"),"codedValues")) 
	  {
	  Trotteur = cur->children->children;
	  while ( Trotteur ) 
	    {
	    printf("\tcode->val:%s",xmlGetProp(Trotteur,"val"));
	    Trotteur = Trotteur->next;
	    }
	  printf("\n");                       
	  }
	}/* end if no_unit */
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
  
  dtd = xmlParseDTD(NULL,"./dict.dtd"); 
  if (! dtd ) 
    { 
    printf("Error Parsing DTD\n");
    return (1);
    }
  else
    {
    printf ("dtd->name:%s\n",dtd->name);  
    }
  /*
    fh=fopen ( "outf","w");
    xmlDebugDumpDTD (fh,dtd);
    fclose(fh);
  */
  /* ----------------------------------------------------------- */
  /*   Set up xmlValidCtxt for error reporting when validating   */
  /* ----------------------------------------------------------- */
  
  ctxt.userData = stderr; 
  ctxt.error    = (xmlValidityErrorFunc) fprintf;   /* register error function */ 
  ctxt.warning  = (xmlValidityWarningFunc) fprintf; /* register warning function */ 
  
  
  if ( xmlValidateDtd(&ctxt,doc,dtd) )
    {
    printf("=========Validation Ok!============\n");
    }
  else 
    {
    printf("========Validation error!===========\n");
    return (1);
    }
  
  
  /*
   * Now, walk the tree.
   */
  
  
  cur = cur->children; 
  while (cur != NULL) 
    {
    Metvar = parsMetvar(doc, ns, cur);  	   
    cur = cur->next;
    }
  
  xmlFreeDtd(dtd);
  xmlFreeDoc(doc);
  return(0);
}


/* External variable for validating DTD */

extern int xmlDoValidityCheckingDefaultValue;



int main(int argc, char **argv) 
{
  xmlDoValidityCheckingDefaultValue = 1;
  
  /* COMPAT: Do not generate nodes for formatting spaces */
  
  LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);
  
  if ( parseJobFile(argv[1]) ) 
    {
    printf("Failure\n");
    return(1);
    }
  return(0);
}
