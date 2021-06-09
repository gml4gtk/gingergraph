/* $Id: str.c,v 1.6 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 1-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* ---  various useful string manipulation functions  --- */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "gingerNew.h"
#include "str.h"

/* case in-sensitive substring-match; is t a substring of s ? */
int cistrstr(char *s, char *t) {  
  int si, ti, ss;
  si=0; ti=0; ss=0;
  
  if( *(t+ti)==0 )
    return 1;

  if( *(s+si)==0 )
    return 0;
  
  while( *(s+ss)!=0 ) {
    si=ss;
    while(*(s+si)!=0 && tolower(*(s+si))!=tolower(*(t+ti)))
      si++;
    
    if( *(s+si)==0 )
      return 0;

    while(tolower(*(s+si))==tolower(*(t+ti)) && *(t+ti)!=0 ) {
      si++; ti++;
    }
    
    if( *(t+ti)==0 )
      return 1;
    
    ti=0;
    ss++;
  }
  return 0;
}

char *nocomma(char *s) {   /* replace ','  with '.' in s */
  int i=0;
  while(*(s+i)) {
    switch(*(s+i)) {
    case ',':
      *(s+i)='.';
      break;
    default:
      break;
    }
    i++;
  }
  return s;
}

char *makePlain(char *s) {   /* replace all other chars than a-zA-Z0-9
				in s with '_' */
  int i=0;
  char c;
  while(c=*(s+i)) {
    if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') )
      { }
    else
      *(s+i)='_';
    i++;
  }
  return s;
}

/* copy at most max-1 characters from src to dst. Make sure
   the destination string is properly zero-terminated */
void zeroAddStrncpy(char *dst, char *src, int max) {
  strncpy(dst,src,max-1);
  dst[max-1]='\0';
}

/* convert strings like "123.45" or "10,07" to the corresponding
   floating point numbers. Note both '.' and ',' may be used as 
   decimal point */
double strToFloat(char *s) {
  char buf[50];
  zeroAddStrncpy(buf,s,49);
  return atof(nocomma(buf));
}

/* remove the last char in s, unless s is empty */
void chop(char *s) {  
  int l;
  l=strlen(s);
  if( l>0 )
    *(s+l-1)='\0';
}

/* allocate and return a copy of s where any '"' chars are removed. */
char *makeCleanString(char *s) { 

  char *res;
  int i,j;
  res=GingerNewString(s);
  
  for(i=0, j=0; *(s+i); i++)
    if( *(s+i)!='"' ) {
      res[j++]=*(s+i);
    }
  res[j] = '\0';

  return res;
}

void sortStrings(char *array[], int array_size) {
  int i,j;
  char *tmp;
  
  for(i=1; i<array_size; i++) 
    for(j=i; j>0 && strcmp(array[j-1],array[j])>0; j--) {
      tmp=array[j-1];
      array[j-1]=array[j];
      array[j]=tmp;
    }
}

