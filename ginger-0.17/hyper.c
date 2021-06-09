/* $Id: hyper.c,v 1.3 1996/03/22 10:04:10 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* A simple hypertext engine */

#include <stdio.h>
#include <string.h>

#include "clist.h"
#include "allocmem.h"
#include "hyper.h"
#include "str.h"

/* ---  private declarations  --- */

static CLIST *node_list;       /* linked list of hypertext nodes */

static void readIndex(FILE *f, FILE *htf);
static char *readBodyTextFromFile(int lnum, int size, FILE *f);
static hypertext_node *nodeFind(char *name);

/* ---  public functions  --- */

/* initialize the hypertext structure (reading the hypertext from file) */
void hyperInit(char *hypertextfile) {
  FILE *f;
  FILE *htf;
  
  char htfile[256];       /* hypertext file name */
  char buf[256];
  int l;
  
  node_list=newlist(STRUCT,sizeof(hypertext_node)); /* create linked list */
  
  /* construct hypertext index filename by appending ".index" to 
     hypertextfile */
  zeroAddStrncpy(htfile,hypertextfile,256);
  zeroAddStrncpy(buf,hypertextfile,256);
  l=strlen(buf);
  zeroAddStrncpy(buf+l,".index",256-l);
  
  if( (f=fopen(buf,"r"))==NULL ) {
    return;
  }
  
  if( (htf=fopen(htfile,"r"))==NULL ) {
    fclose(f);
    return;
  }
  
  /* read the hypertext from htf, using the index file f */
  readIndex(f,htf);
  fclose(htf);
  fclose(f);
}

/* return a pointer to the body text of the node nodename 
   return "nomatch", if no matching node is found */
char *bodyText(char *nodename) {
  hypertext_node *n;
  n=nodeFind(nodename);
  if(!n)
    return "nomatch";
  return n->body;
}

/* return the name of the link at "position" in "bodyText", if any.
   otherwise return "nolink" */
char *linkAtPosition(char *bodyText, int position) {
  int i,j,k;
  int done=0;
  char *p="nolink";
  static char item[LINK_MAX_LEN];

  if(position>strlen(bodyText)) {
    return p;
  }
  for(i=position; i>0 && !done; i--) {
    if( *(bodyText+i)=='_' && *(bodyText+i-1)=='_' ) {
      for(j=i+1, k=0; k<LINK_MAX_LEN && *(bodyText+j); j++, k++) {
	if( *(bodyText+j)=='_' && *(bodyText+j+1)=='_' ) {
	  item[k]=0;
	  return item;
	}
	item[k]=*(bodyText+j);
      }
      done=1;
    }
  }
  return p;
}

/* return the name of the logical "next" node, given a node name.
   return "nolink", if no such node is found */
char *nextNode(char *nodename) {
  hypertext_node *n;
  n=nodeFind(nodename);
  if(!n)
    return "nolink";
  return n->nextNode;
}

/* ---  private functions  --- */

static hypertext_node *nodeFind(char *name) {
  hypertext_node *n;
  int t;

  for(t=1; t<=node_list->size; t++) {
    n=(hypertext_node *)retrieve(t,node_list);
    /* printf("%s %d %d\n", n->name, n->linenum, n->size); */
    if(!strcmp(name,n->name))
      return n;
  }
  return 0;
}

static void readIndex(FILE *index_file, FILE *hypertext_file) {
  char buf[256];
  hypertext_node n;
  
  while(!feof(index_file)) {
    if( fgets(buf,255,index_file) )   {
      n.name=(char *)AllocateMemory(strlen(buf)+1);
      strcpy(n.name,buf);
      n.name[strlen(n.name)-1]=0;
      
      fgets(buf,255,index_file);
      n.nextNode=(char *)AllocateMemory(strlen(buf)+1);
      strcpy(n.nextNode,buf);
      n.nextNode[strlen(n.nextNode)-1]=0;
      
      fgets(buf,255,index_file);
      n.linenum=atoi(buf);

      fgets(buf,255,index_file);
      n.size=atoi(buf);
      
      n.body=readBodyTextFromFile(n.linenum, n.size, hypertext_file);
      /* printf("%s == %s", n.name, n.body); */ 
      insert_last((BYTE *)&n,node_list);
    }
  }
}

static char *readBodyTextFromFile(int lnum, int size, FILE *f) {
  static int prevline=0;
  int totallen=0, old;
  int delta, i;
  char buf[256];
  char *res;

  delta=lnum-prevline;

  for(i=0; i<delta; i++) {
    prevline++;
    fgets(buf,255,f);
    /* printf("skip %s\n", buf); */
  }
  res=(char *)AllocateMemory(size+5);
  res[0]=0;

  while( fgets(buf,255,f) ) {
    prevline++;
    /* printf("use %s\n", buf); */
    if(buf[0]==':')
      return res;
    old=totallen;
    totallen+=strlen(buf);
    if(totallen<size+1)
      /* strcat(res,buf); */
      strcpy(res+old,buf);
  }
  return res;
}

