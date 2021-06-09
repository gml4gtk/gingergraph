/*
Checksum: 2855550449      (clist.c) 
*/

/*
 *    This file is part of:
 *
 *     Ginger -- an INteractive Graph EditoR for the X Window System.
 *                          COPYRIGHT (C) 1996 
 *
 *    Martin K. Molz and Niels C. Larse, Aalborg University, Denmark.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Bison; see the file COPYING.  If not, write to
 *    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gingerprotos.h"

/* 
 * struct list_element 
 * {
 *   unsigned char *value;
 *   struct list_element *next;
 * };
 * 
 * struct circular_list 
 * {
 *   struct list_element *last_element;
 *   struct list_element *retrieve;
 *   int retrievepos;
 *   int size;
 *   int cursor;
 *   int value_size;
 *   unsigned char value_type;
 * };
 * 
 * typedef struct circular_list clist;
 */

/* */
struct circular_list *
newlist (unsigned char value_type, int value_size)
{
  struct circular_list *r = (struct circular_list *) 0;
  /* r = (struct circular_list *) mmalloc (sizeof (struct circular_list)); */
  r =
    (struct circular_list *) pool_malloc (clistpool,
					  sizeof (struct circular_list));
  r->last_element = (struct list_element *) 0;
  r->retrieve = (struct list_element *) 0;
  r->retrievepos = (int) 0;
  r->size = (int) 0;
  r->cursor = (int) 0;
  r->value_size = (int) value_size;
  r->value_type = (unsigned char) value_type;
  return ((struct circular_list *) r);
}

/* */
int
getsize (struct circular_list *l)
{
  return ((int) l->size);
}

/* current position in list */
int
retrieve_pos (struct circular_list *l)
{
  return (l->retrievepos);
}

/* */
unsigned char *
retrieve (int i, struct circular_list *l)
{
  if (l->size > 0)
    {
      if ((i < 3) || (l->retrieve == (struct list_element *) 0)
	  || (i != (l->retrievepos + 1) && i != l->retrievepos))
	{
	  l->retrieve = find_i_th (i, l);
	  l->retrievepos = i;
	}
      else if (i == (l->retrievepos + 1))
	{
	  l->retrieve = l->retrieve->next;
	  l->retrievepos = l->retrievepos + 1;
	}
      else
	{
	  /* */
	}
      return ((unsigned char *) l->retrieve->value);
    }
  else
    {
      return ((unsigned char *) 0);
    }
}

/* */
struct list_element *
find_i_th (int i, struct circular_list *l)
{
  struct list_element *temp = (struct list_element *) 0;
  int j = 0;
  if (i == 0)
    {
      return ((struct list_element *) l->last_element);
    }
  temp = first_element (l);
  for (j = 1; j < i; j++)
    {
      temp = temp->next;
    }
  return ((struct list_element *) temp);
}

/* */
struct list_element *
first_element (struct circular_list *l)
{
  return ((struct list_element *) l->last_element->next);
}

/* */
void
insert_last (unsigned char *x, struct circular_list *l)
{
  struct list_element *t = (struct list_element *) 0;
  if (l->size == 0)
    {
      init (x, l);
    }
  else
    {
      /* t = (struct list_element *) mmalloc (sizeof (struct list_element)); */
      t =
	(struct list_element *) pool_malloc (clistpool,
					     sizeof (struct list_element));
      t->value = kopier (x, l);
      t->next = first_element (l);
      l->last_element->next = t;
      l->last_element = t;
    }
  l->size = l->size + 1;
  return;
}

/* */
void
init (unsigned char *x, struct circular_list *l)
{
  /* l->last_element = (struct list_element *) mmalloc (sizeof (struct list_element)); */
  l->last_element =
    (struct list_element *) pool_malloc (clistpool,
					 sizeof (struct list_element));
  l->last_element->value = kopier (x, l);
  l->last_element->next = l->last_element;
  return;
}

/* */
unsigned char *
kopier (unsigned char *src, struct circular_list *l)
{
  char *x = (char *) 0;
  void *y = (void *) 0;
  unsigned char *dst = (unsigned char *) 0;
  int t = 0;
  if (l->value_type != CL_STRING)
    {
      /* structure */
      /* dst = (unsigned char *) mmalloc (l->value_size); */
      dst = (unsigned char *) pool_malloc (clistpool, l->value_size);
      y = (void *) memcpy (dst, src, l->value_size);
    }
  else
    {
      /* string */
      t = strlen ((char *) src);
      /* dst = (unsigned char *) mmalloc (t + 1); */
      dst = (unsigned char *) pool_malloc (clistpool, (t + 1));
      x = (char *) strcpy (dst, src);
    }
  return ((unsigned char *) dst);
}

/* */
void
iterate (funct f, struct circular_list *l)
{
  int j = 0;
  struct list_element *t = (struct list_element *) 0;
  if (!l)
    {
      return;
    }
  if (l->size == 0)
    {
      return;
    }
  t = first_element (l);
  for (j = 1; j <= l->size; j++)
    {
      (*f) (t->value);
      t = t->next;
    }
  return;
}

/* 
 * void
 * delete_all (struct circular_list *l)
 * {
 *   struct list_element *t = (struct list_element *) 0;
 *   l->retrieve = (struct list_element *) 0;
 *   l->retrievepos = 0;
 *   while (l->size > 0)
 *     {
 *       t = first_element (l);
 *       l->last_element->next = t->next;
 *       mmfree (t->value);
 *       mmfree (t);
 *       l->size = l->size - 1;
 *       l->cursor = l->cursor - 1;
 *     }
 *   return;
 * }
 */

/* */
unsigned char *
get_last (struct circular_list *l)
{
  return ((unsigned char *) l->last_element->value);
}

/* End. */
