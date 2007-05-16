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
#include <rpnmacros.h>
#include <gmp.h>
#include <rec.h>
#include <rec_functions.h>
#include <xinit.h>
#include <rpnmacros.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define DEBUG(x) printf(x)


typedef struct Metvar 
{
  char *Nomvar;
  char *Description;
  char *Use;
  char *Measure;
} Metvar, *MetvarPtr;




typedef struct
{
  char nomVar[8];
  char identifVar[2][64];
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
} _XMLInfoChamps, *_XMLinfoChampsPtr;

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

_XMLinfoChampsPtr parseMetvar(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur) 
{
  _XMLinfoChampsPtr ret = NULL;
  xmlNodePtr Trotteur ;
  char strFacteurMult[32];
  
  printf("nomvar: %s\n",xmlGetProp(cur,"nomvar"));
  strcpy(infoChamps[nbChampsDict].nomVar,xmlGetProp(cur,"nomvar"));
  
  cur = cur->children;  
  while (cur != NULL) 
    {
    if ( !strcmp(cur->name, "description") ) 
      {
      /*  ret->Description = xmlNodeListGetString(doc, cur->children, 1); */
      strcpy(infoChamps[nbChampsDict].identifVar[FRANCAIS], xmlGetProp(cur,"fr"));
      strcpy(infoChamps[nbChampsDict].identifVar[ENGLISH], xmlGetProp(cur,"en"));
      printf("\tdesc fr:%s\n", infoChamps[nbChampsDict].identifVar[FRANCAIS]);
      printf("\tdesc en:%s\n", infoChamps[nbChampsDict].identifVar[ENGLISH]);
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
	strcpy(infoChamps[nbChampsDict].unitesVar, xmlGetProp(cur->xmlChildrenNode,"units"));
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
  
  nbChampsDict++;
  if (0 == (nbChampsDict % 16))
    {
    infoChamps = (_InfoChamps *)realloc(infoChamps, sizeof(_InfoChamps)*(nbChampsDict+16));
    }

  
  return(ret);
}

  


int LireDictionnaireCMC(char *fichierXML)
{
int i, j, k;
 
 char buf[BUFSIZ];
 char *home;
 char dtdfile[256];
 int done;
 int inlen, outlen;
 unsigned char *inbuffer,*outbuffer;
 
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

 inbuffer=(unsigned char *) malloc(256*sizeof(unsigned char)); 
 outbuffer=(unsigned char *) malloc(256*sizeof(unsigned char)); 

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
 
 home = (char *) getenv ("AFSISIO");
 strcpy(dtdfile, home);
 strcat(dtdfile, "/datafiles/constants/dict.dtd");
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

 for (i=0; i < nbChampsDict; i++)
   {
   printf("*--------------------------------------------------------------\n");
   strcpy(inbuffer, infoChamps[i].identifVar[FRANCAIS]);
   inlen = xmlUTF8Strsize(inbuffer, 256);
   outlen=256;
   UTF8Toisolat1(outbuffer,&outlen,inbuffer ,&inlen);
   outbuffer[outlen]='\0';
   strcpy(infoChamps[i].identifVar[FRANCAIS], outbuffer);
   printf("Champ %03d de %03d\n", i, nbChampsDict);
   printf("nomvar: %s\n", infoChamps[i].nomVar);
   printf("\tdesc fr:%s\n", infoChamps[i].identifVar[FRANCAIS]);
   printf("\tdesc en:%s\n", infoChamps[i].identifVar[ENGLISH]);
   printf("\tunites:%s\n",  infoChamps[i].unitesVar);
   
   }
 
 xmlFreeDtd(dtd);
 xmlFreeDoc(doc);
 return(0);
 
}


#endif
