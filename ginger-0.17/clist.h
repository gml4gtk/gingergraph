/* $Id: clist.h,v 1.1 1996/02/07 11:52:43 lupus Exp $ */

/* CLIST.H */

#ifndef CLIST_H
#define CLIST_H

#define CLIST_MAIN 0

#define CL_STRING 0
#define STRUCT 1
#define SIMPLE 1

typedef unsigned char BYTE;
typedef void (*funct)(BYTE *);  /* funct is a pointer-to-void-funtion type */
typedef int (*cmpfunct)(BYTE *, BYTE *);  /* cmpfunct is a pointer-to-int-funtion type */

struct list_element {
   BYTE *value;
   struct list_element  *next;
};

struct circular_list {
   struct list_element *last_element;
   struct list_element *retrieve;
   int retrievepos;
   int size;
   int cursor;
   int value_size;
   BYTE value_type;
};

typedef struct circular_list CLIST;

CLIST *newlist(BYTE value_type, int value_size);
int getsize(CLIST *l);
void insert_first(BYTE *value, CLIST *l);
void insert_last(BYTE *value, CLIST *l);
void delete_first(CLIST *l);
void delete_last(CLIST *l);
void delete_item(BYTE *val, CLIST *l);
void delete_all(CLIST *l);
BYTE *get_first(CLIST *l);
BYTE *get_last(CLIST *l);
BYTE *retrieve(int i, CLIST *l);
void store(int i, BYTE *newvalue, CLIST *l);
void insert_ith(int i, BYTE *value, CLIST *l);
void delete_ith(int i, CLIST *l);
void moveForth(CLIST *l);           /* move cursor */
void moveBack(CLIST *l);            /* move cursor */
void moveStart(CLIST *l);           /* move cursor */
void moveEnd(CLIST *l);             /* move cursor */
BYTE *item(CLIST *l);		/* item at cursor pos. */
void insert(BYTE *x, CLIST *l); /* item at cursor pos. */
void delete(CLIST *l);          /* item at cursor pos. */
void swapelem(int i,int j, CLIST *l);
void iterate(funct, CLIST *l);
BYTE *search(BYTE *value, cmpfunct, CLIST *l);

#endif
