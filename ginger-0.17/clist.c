/* $Id: clist.c,v 1.2 1996/02/12 13:31:05 lupus Exp $ */

/* circular linked list

   to be "instantiated" with the size of the data elements it is supposed 
   to hold, in the call to newlist(), except if the data elements are text
   strings, in which case the list determines the length of each element 
   as it is inserted.

   The list keeps its own copy of the inserted elements, so it's safe to 
   deallocate an object when it is inserted in the list.
*/

#include <stdlib.h>
#include <string.h>

#include "clist.h"
#include "allocmem.h"
/*#define AllocateMemory(a) malloc(a) */

int getsize(CLIST *l)
{
   return l->size;
}

CLIST *newlist(BYTE value_type, int value_size)
/* value_type should be either STRUCT or CL_STRING */
{
   CLIST *res;
   res=(CLIST *)AllocateMemory(sizeof(CLIST));
   res->retrieve=NULL;
   res->retrievepos=0;
   res->size=0;
   res->cursor=0;
   res->value_type=value_type;
   res->value_size=value_size;
   return res;
}

static BYTE *kopier(BYTE *src, CLIST *l)
{
   BYTE *dst;
   register int t;
   if( l->value_type!=CL_STRING ) {
      dst=(BYTE *)AllocateMemory(l->value_size); /* g›r plads til dataindhold */
      memcpy(dst,src,l->value_size);
   }
   else {
      t=strlen((char *)src);
      dst=(BYTE *)AllocateMemory(t+1); 		/* g›r plads til dataindhold */
      t=0;
      strcpy(dst,src);
   }
   return dst;
}

static void init(BYTE *x, CLIST *l)
{
   l->last_element=(struct list_element *)AllocateMemory(sizeof(struct list_element));
   l->last_element->value=kopier(x,l);
   l->last_element->next=l->last_element;
}

static struct list_element *first_element(CLIST *l)
{
   return l->last_element->next;
}

void insert_first(BYTE *x, CLIST *l)
{
   struct list_element *temp;
   if( l->size==0 )
      init(x,l);
   else {
      temp=(struct list_element *) AllocateMemory(sizeof(struct list_element));
      temp->value=kopier(x,l);
      temp->next=first_element(l);
      l->last_element->next=temp;
   }
   l->size++;
   l->cursor++;
}

void insert_last(BYTE *x, CLIST *l)
{
   struct list_element *temp;
   if( l->size==0 )
      init(x,l);
   else {
      temp=(struct list_element *) AllocateMemory(sizeof(struct list_element));
      temp->value=kopier(x,l);
      temp->next=first_element(l);
      l->last_element->next=temp;
      l->last_element=temp;
   }
   l->size++;
}

void delete_first(CLIST *l)
{
   struct list_element *temp;
   if( l->size>0 ) {
     l->retrieve=NULL;
     l->retrievepos=0;
     temp=first_element(l);
     l->last_element->next=temp->next;
     free(temp->value);
     free(temp);
     l->size--;
     l->cursor--;
   }
}

void delete_all(CLIST *l)
{
   struct list_element *temp;
   l->retrieve=NULL;
   l->retrievepos=0;
   while( l->size>0 ) {
      temp=first_element(l);
      l->last_element->next=temp->next;
      free(temp->value);
      free(temp);
      l->size--;
      l->cursor--;
   }
}

static struct list_element *find_i_th(int i,CLIST *l)
{
   struct list_element *temp;
   int j;
   if(i==0)                  		/* element 0 =last */
      return l->last_element;
   for(j=1, temp=first_element(l); j<i; j++)
      temp=temp->next;
   return temp;
}

void insert_ith(int i, BYTE *x, CLIST *l)
{
   struct list_element *temp, *insafter;
   if( i<1 || i>l->size+1 )
      return;
   if( i==1 ) {
      insert_first(x,l);
      return;
   }
   if( i==l->size+1 ) {
      if( l->cursor==l->size )
	 l->cursor++;
      insert_last(x,l);
      return;
   }

   temp=(struct list_element *) AllocateMemory(sizeof(struct list_element));
   temp->value=kopier(x,l);

   insafter=find_i_th(i-1,l);
   temp->next=insafter->next;
   insafter->next=temp;
   l->size++;
   if( i<=l->cursor )
      l->cursor++;
}

void delete_ith(int i, CLIST *l)
{
   struct list_element *temp, *delete;
   l->retrieve=NULL;
   l->retrievepos=0;
   if( i<1 || i>l->size )
      return;
   if( i==1 ) {
      delete_first(l);
      return;
   }
   if( i==l->size ) {
      delete_last(l);
      return;
   }

   temp=find_i_th(i-1,l);
   delete=temp->next;
   temp->next=delete->next;
   free(delete->value);
   free(delete);
   l->size--;
   if( i<=l->cursor )
      l->cursor--;
}

static struct list_element *second_last(CLIST *l)
{
   if( l->size<2 )
      return l->last_element;
   else
      return find_i_th(l->size-1,l);
}

void delete_last(CLIST *l)
{
   struct list_element *temp;
   if( l->size>0 ) {
     l->retrieve=NULL;
     l->retrievepos=0;
     temp=second_last(l);
     temp->next=first_element(l);
     free(l->last_element->value);
     free(l->last_element);
     l->last_element=temp;
     if( l->cursor==l->size )
       l->cursor--;
     l->size--;
   }
}

BYTE *get_first(CLIST *l) {
  return retrieve(1,l);
}

BYTE *get_last(CLIST *l) {
  return l->last_element->value;
}

BYTE *retrieve(int i,CLIST *l)
{
   if( l->size>0 ) {
     /* l->retrieve=find_i_th(i,l); */ /* lupus:  */
     /* return l->retrieve->value;  */ /* disse to linier er alternative! */
      if( i<3 || l->retrieve==NULL || (i!=(l->retrievepos+1) && i!=l->retrievepos) ) {
/*	 printf("Searching...");
	 getch();*/
	 l->retrieve=find_i_th(i,l);
	 l->retrievepos=i;
      }
      else if(i==(l->retrievepos+1)) {
/*	 printf("Yep!\n");
	 getch();*/
	 l->retrieve=l->retrieve->next;
	 l->retrievepos++;
      }
      return l->retrieve->value;
   }
}

void store(int i, BYTE *src, CLIST *l)
{
   struct list_element *temp;
   BYTE *dst;
   register int t;
   if( l->size>0 ) {
      temp=find_i_th(i,l);
      if( l->value_type!=CL_STRING ) {
	 dst=temp->value;
	 /* for(t=0; t<l->value_size; t++)	kopi‚r dataindhold 
	    *(dst+t)=*(src+t); */
	 memcpy(dst, src, l->value_size);
      }
      else {
	 t=strlen((char *)src);
	 free(temp->value);
	 dst=(BYTE *)AllocateMemory(t+1); 		/* g›r plads til dataindhold */
	 temp->value=dst;
	 t=0;
	 strcpy(dst,src);
	 /* do
	    *(dst+t)=*(src+t);		* kopi‚r dataindhold *
	 while( *(src+t++)!='\0' );
	 */
      }
   }
}

void moveForth(CLIST *l)
{
   if( l->cursor<l->size )
      l->cursor++;
}

void moveBack(CLIST *l)
{
   if( l->cursor>1 )
      l->cursor--;
}

void moveStart(CLIST *l)
{
   l->cursor=0;
}

void moveEnd(CLIST *l)
{
   l->cursor=l->size;
}

BYTE *item(CLIST *l)		/* item at cursor pos. */
{
   struct list_element *temp;
   if( l->size>0 ) {
      temp=find_i_th(l->cursor,l);
      return temp->value;
   }
}


void insert(BYTE *x, CLIST *l)
{
   struct list_element *temp, *insafter;
   int i;
   i=l->cursor;
   if( i==0 ) {
      insert_first(x,l);
      return;
   }
   if( i==l->size ) {
      l->cursor++;
      insert_last(x,l);
      return;
   }

   temp=(struct list_element *) AllocateMemory(sizeof(struct list_element));
   temp->value=kopier(x,l);

   insafter=find_i_th(i,l);
   temp->next=insafter->next;
   insafter->next=temp;
   l->size++;
   l->cursor++;
}

void delete(CLIST *l)
{
   struct list_element *temp, *delete;
   int i;
   i=l->cursor;
   if( i<1 || i>l->size )
      return;
   l->retrieve=NULL;
   l->retrievepos=0;
   if( i==1 ) {
      delete_first(l);
      return;
   }
   if( i==l->size ) {
      delete_last(l);
      return;
   }

   temp=find_i_th(i-1,l);
   delete=temp->next;
   temp->next=delete->next;
   free(delete);
   l->size--;
}

static int neighbour(int i, int j)
{
   if( i+1==j || j+1==i )
      return 1;
   return 0;
}

static int first_last_swap(int i, int j, CLIST *l)
{
   return( i==1 && j==l->size || j==1 && i==l->size );
}

void swapelem(int i,int j, CLIST *l)
{
   struct list_element *foran_i, *foran_j, *efter_i, *efter_j, *temp;
   int tmp;

   if( i==j || i<1 || j<1 || i>l->size || j>l->size )
      return;
   if( i>j ) {		/* vi insisterer i det f›lgende p†, at i<j */
      tmp=i;
      i=j;
      j=tmp;
   }

   foran_i=find_i_th(i-1,l);
   foran_j=find_i_th(j-1,l);
   efter_i=foran_i->next->next;
   efter_j=foran_j->next->next;
   temp=foran_i->next;		/* temp = element i */
   foran_i->next=foran_j->next; /* element i erstattes med element j  */
   foran_j->next=temp;		/* element j erstattes med temp */

   if( !first_last_swap(i,j,l) )
      foran_j->next->next=efter_j;
   else
      foran_j->next->next=l->last_element;

   if( !neighbour(i,j) )
      foran_i->next->next=efter_i;
   else
      foran_i->next->next=temp;

   if( j==l->size )
      if( !neighbour(i,j) )
	 l->last_element=foran_j->next;
      else
	 l->last_element=temp;
}

void iterate(funct f, CLIST *l) {   /* perform f() on each element in l */
  int j;
  struct list_element *temp;

  if(l->size==0)
    return;
  for(j=1, temp=first_element(l); j<=l->size; j++) {
    (*f)(temp->value);
    temp=temp->next;
  }
}

BYTE *search(BYTE *val, cmpfunct compareFunc, CLIST *l) {
/* return the value of the first list element for which 
   compareFunc(val,value) is true. If no elements match, return 0
*/
  int j;
  struct list_element *temp;

  if(l->size==0)
    return 0;
  for(j=1, temp=first_element(l); j<=l->size; j++) {
    if( (*compareFunc)(val, temp->value) )
      return temp->value;
    temp=temp->next;
  }
  return 0;
}

void delete_item(BYTE *val, CLIST *l) {
  int j;
  struct list_element *temp;
  
  if(l->size<1)
    return;
  l->retrieve=NULL;
  l->retrievepos=0;
  for(j=1, temp=first_element(l); j<=l->size; j++) {
    if( val==temp->value ) {
      delete_ith(j,l);
      return;
    }
    temp=temp->next;
  }
}

#if CLIST_MAIN

void printElem(BYTE *s) {
  printf("%s\n", s );
}

int lengthIsEqual(BYTE *s1, BYTE *s2) {
  return (strlen(s1)==strlen(s2));
}

void prs(CLIST *l)
{
   int t;
   char *s;
   printf("BEGIN\n");
   iterate(printElem,l);
/*
   s=search("xxx", lengthIsEqual, l);
   if(s)
     printf("First element of length 3: %s\n", s);
*/
   printf("END\n");
}

main()
{
   CLIST *liste;
   int t;

   liste=newlist(CL_STRING,0);
   insert_last("Hund", liste);
   insert_last("Kat", liste);
   insert_last("Gris", liste);
   insert_last("Ko", liste);
   insert_last("Kalv", liste);
   insert_first("Tyr", liste);

   prs(liste);
/*   printf(get_last(liste));
   printf(get_first(liste));
*/
   delete_last(liste);
   prs(liste);
   delete_first(liste);
   prs(liste);
   store(2,"!",liste);
   prs(liste);
}

#endif

