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
#ifndef HP
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <malloc.h>
#include <rec.h>
#include <rpnmacros.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define DEBUG(x) printf(x)


typedef struct 
{
  int ind_dict;
  int ind_tag;
  int is_active;
  char *current_tag;
} _taginfo;


typedef struct Metvar 
{
  char *Nomvar;
  char *Description;
  char *Use;
  char *Measure;
} Metvar, *MetvarPtr;




typedef struct
{
  char nomVar[5];
  char identifVar[64];
  char unitesVar[32];
  char paletteVar[32];
  int  indIntervalleDeDefaut;
  float intervalleDeDefaut;
  float facteurDeConversion;
  float intervallesDeContour[32][32];
  int   nbMenuItems;
  int   nbIntervalles[32];
  float missingValue;
  float userMin,userMax;
} _XMLInfoChamps;

void XML_AjouterNomvar(_XMLInfoChamps XMLInfo);


#define NOMVAR          0
#define FR_DESC         1
#define ENG_DESC        2
#define UNITS           3
#define CONTOUR         4
#define DEFAULT_CONTOUR 5

   
static _XMLInfoChamps XMLInfo;
extern _InfoChamps *infoChamps;
extern int nbChampsDict;

MetvarPtr parseMetvar(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur) 
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
    
    if ( !strcmp(cur->name, "measure") ) 
      {
      /*
	-------------
	UNIT
	-------------
      */
      if ( !strcmp(cur->xmlChildrenNode->name,"unit") ) 
	{
	printf("\tmeasure->unit->unit_value:%s\n",xmlGetProp(cur->xmlChildrenNode,"unit_value"));
	if ( xmlGetProp(cur->xmlChildrenNode,"decimal_factor") )
	  printf("\tmeasure->unit->decimal_factor:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"decimal_factor"));
	if ( xmlGetProp(cur->xmlChildrenNode,"min") )
	  printf("\tmeasure->unit->min:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"min"));
	if ( xmlGetProp(cur->xmlChildrenNode,"max") )
	  printf("\tmeasure->unit->max:%s\n",
		 xmlGetProp(cur->xmlChildrenNode,"max"));
	}
      /*
	-------------
	NO_UNIT
	-------------
      */
      if ( !strcmp(cur->xmlChildrenNode->name,"No_unit") ) 
	{ 
	printf("\tmeasure->No_unit->validityRange:%s\n", xmlGetProp(cur->xmlChildrenNode,"validityRange")); 
	printf("\tmeasure->No_unit->ref:%s\n", xmlGetProp(cur->xmlChildrenNode,"ref")); 
	
	if ( xmlGetProp(cur->xmlChildrenNode,"objectOfRatio") )
	  printf("\tmeasure->No_unit->objectOfRatio:%s\n", 
		 xmlGetProp(cur->xmlChildrenNode,"objectOfRatio"));
	
	if ( !strcmp(xmlGetProp(cur->xmlChildrenNode,"validityRange"),"integerRange" ) ||
	     !strcmp(xmlGetProp(cur->xmlChildrenNode,"validityRange"),"realRange" ) ) 
	  {
	  printf("\tmeasure->No_unit->min:%s\n", xmlGetProp(cur->xmlChildrenNode,"min"));
	  printf("\tmeasure->No_unit->max:%s\n", xmlGetProp(cur->xmlChildrenNode,"max"));
	  
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
      }/* end if measure */
    
    
    
    cur = cur->next;
    }
  
  
  return(ret);
}




/*
  static void
  startElement(void *userData, const char *name, const char **atts)
  {
  int i;
  _taginfo *taginfo = userData;
  taginfo->is_active = 0;
  taginfo->ind_tag = -1;
  
  if (0 == strcmp(name, "metvar"))
  {
  taginfo->is_active = 1;
  }
  
  if (0 == strcmp(name, "nomvar"))
  {
  taginfo->ind_tag = NOMVAR;
  taginfo->is_active = 1;
  }
  
  if (0 == strcmp(name, "francais"))
  {
  taginfo->ind_tag = FR_DESC;
  taginfo->is_active = 1;
  }
  
  if (0 == strcmp(name, "english"))
  {
  taginfo->ind_tag = ENG_DESC;
  taginfo->is_active = 1;
  }
  
  if (0 == strcmp(name, "units"))
  {
  taginfo->ind_tag = UNITS;
  taginfo->is_active = 1;
  }
  
  if (0 == strcmp(name, "contour"))
  {
  taginfo->ind_tag = CONTOUR;
  taginfo->is_active = 1;
  }
}

*/

/*
  static void
  charElement(void *userData, const char *name, int len)
  {
  int i;
  char *texte;
  _taginfo *taginfo = userData;
  
  if (len == 0)
  return;
  
  
  if (taginfo->is_active == 0) return;
  
  
  
  texte = (char *) malloc ((len+1) * sizeof(char));
  strncpy(texte, name, len);
  texte[len] = '\0';
  i = taginfo->ind_dict;
  
  switch (taginfo->ind_tag)
  {
  case NOMVAR:
  strcpy(XMLInfo.nomVar,texte);
  break;
  
  case FR_DESC:
  strcpy(XMLInfo.identifVar, texte);
  break;
  
  case ENG_DESC:
  strcpy(XMLInfo.identifVar, texte);
  break;
  
  case UNITS:
  strcpy(XMLInfo.unitesVar, texte);
  break;
  
  case CONTOUR:
  sscanf(texte, "%f", &(XMLInfo.intervallesDeContour[XMLInfo.nbMenuItems][0]));
  XMLInfo.nbMenuItems++;
  break;
  
  default:
  break;
  
  }
  
  }
  
*/

/*
  static void
  endElement(void *userData, const char *name)
  {
  int i;
  _taginfo *taginfo = userData;
  if (0 == strcmp(name, "metvar"))
  {
  XML_AjouterNomvar(XMLInfo);
  memset(&XMLInfo, (int)NULL, sizeof(_XMLInfoChamps));
  }
  taginfo->is_active = 0;
  }
  
*/

int LireDictionnaireXML(char *fichierXML)
{
int i, j, k;
_taginfo taginfo;
 
 char buf[BUFSIZ];
 char *home;
 char dtdfile[256];
 int done;
 
 extern int xmlDoValidityCheckingDefaultValue;


xmlDocPtr    doc;
xmlNsPtr     ns;
xmlNodePtr   cur;
MetvarPtr    Metvar;
xmlDtdPtr    dtd ;
xmlValidCtxt ctxt;
FILE *fh;

LIBXML_TEST_VERSION xmlKeepBlanksDefault(0);

/*
 * build an XML tree from the file;
 */
doc = xmlParseFile(fichierXML);
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
 
 home = (char *) getenv ("HOME");
 strcpy(dtdfile, home);
 strcat(dtdfile, "/.rec/dict.dtd");
 dtd = xmlParseDTD(NULL,dtdfile); 
 if (! dtd ) 
   { 
   printf("Error Parsing DTD\n");
   return (1);
   }
 else
   {
   printf ("dtd->name:%s\n",dtd->name);  
   }

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
   Metvar = parseMetvar(doc, ns, cur);  	   
   cur = cur->next;
   }
 
 xmlFreeDtd(dtd);
 xmlFreeDoc(doc);
 return(0);
 
}

/*
  void XML_AjouterNomvar(_XMLInfoChamps XMLInfo)
  {
  int i;
  
  if (0 == (nbChampsDict % 16))
  infoChamps = (_InfoChamps *)realloc(infoChamps, sizeof(_InfoChamps)*(nbChampsDict+16));
  
  infoChamps[nbChampsDict].facteurDeConversion = 1.0;
  infoChamps[nbChampsDict].indIntervalleDeDefaut =  4;

  strcpy(infoChamps[nbChampsDict].nomVar, XMLInfo.nomVar);
  strcpy(infoChamps[nbChampsDict].identifVar, XMLInfo.identifVar);
  strcpy(infoChamps[nbChampsDict].unitesVar, XMLInfo.unitesVar);
  
  infoChamps[nbChampsDict].nbMenuItems = XMLInfo.nbMenuItems;
  infoChamps[nbChampsDict].nbIntervalles = (int *)calloc(XMLInfo.nbMenuItems, sizeof(int));
  infoChamps[nbChampsDict].intervallesDeContour = (float **)calloc(XMLInfo.nbMenuItems, sizeof(float *));
  
  for (i=0; i < XMLInfo.nbMenuItems; i++)
  {
  infoChamps[nbChampsDict].nbIntervalles[i] = 1;
  infoChamps[nbChampsDict].intervallesDeContour[i] = (float *)calloc(1, sizeof(float));
  infoChamps[nbChampsDict].intervallesDeContour[i][0] = XMLInfo.intervallesDeContour[i][0];
  }
  DictMgrSetMinMaxMode(XMLInfo.nomVar,nbChampsDict,AUTO);
  nbChampsDict ++;
  
  }
*/


/*
 * And the code needed to parse it
 */
#endif
