/*
Checksum: 3937907475      (uniqstr.c)
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

/* Forward declarations. */
static void splayinit_syms (void);
static char *splaystrdupsym (char *s);

/* */
int
uniqstr_init (void)
{

  uniqstrings = pool_delete (uniqstrings);
  uniqstrings = pool_create ();

  splayinit_syms ();

  return (0);
}

/* */
int
uniqstr_deinit (void)
{

  uniqstrings = pool_delete (uniqstrings);

  return (0);
}

/* */
char *
uniqstr (char *s)
{

  if (s == (char *) 0)
    {
      return ((char *) 0);
    }

  if (strlen (s) == 0)
    {
      return ("");
    }

  return ((char *) splaystrdupsym (s));
}

/* */
int
uniqstr_check (FILE * stream)
{
  char *p = (char *) 0;
  char *s = (char *) 0;
  char *t = (char *) 0;
  uniqstr_init ();
  p = uniqstr ("abc");
  s = uniqstr ("abc");
  fprintf (stream, "%p versus %p should be same\n", s, p);
  t = uniqstr ("def");
  fprintf (stream, "%p versus %p should be different\n", s, t);
  uniqstr_deinit ();
  fflush (stream);
  return (0);
}

/* */
#undef SMALLUNIQSTRING
#define SMALLUNIQSTRING 7

/* */
#define UNIQSTRHASHSIZ 5003

/* */
static struct splay_node *hashtab1[UNIQSTRHASHSIZ];
static struct splay_node *hashtab2[UNIQSTRHASHSIZ];

/*
 * DERIVED FROM:
 * lookup2.c, by Bob Jenkins, December 1996, Public Domain.
 * hash(), hash2(), hash3, and mix() are externally useful functions.
 * Routines to test the hash are included if SELF_TEST is defined.
 * You can use this free for any purpose.  It has no warranty.
 *
 * mix -- mix 3 32-bit values reversibly.
 * For every delta with one or two bit set, and the deltas of all three
 *   high bits or all three low bits, whether the original value of a,b,c
 *   is almost all zero or is uniformly distributed,
 * * If mix() is run forward or backward, at least 32 bits in a,b,c
 *   have at least 1/4 probability of changing.
 * * If mix() is run forward, every bit of c will change between 1/3 and
 *   2/3 of the time.  (Well, 22/100 and 78/100 for some 2-bit deltas.)
 * mix() was built out of 36 single-cycle latency instructions in a 
 *   structure that could supported 2x parallelism, like so:
 *       a -= b; 
 *       a -= c; x = (c>>13);
 *       b -= c; a ^= x;
 *       b -= a; x = (a<<8);
 *       c -= a; b ^= x;
 *       c -= b; x = (b>>13);
 *       ...
 *   Unfortunately, superscalar Pentiums and Sparcs can't take advantage 
 *   of that parallelism.  They've also turned some of those single-cycle
 *   latency instructions into multi-cycle latency instructions.  Still,
 *   this is the fastest good hash I could find.  There were about 2^^68
 *   to choose from.  I only looked at a billion or so.
 *
 * same, but slower, works on systems that might have 8 byte hashval_t's 
 */
#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<< 8); \
  c -= a; c -= b; c ^= ((b&0xffffffff)>>13); \
  a -= b; a -= c; a ^= ((c&0xffffffff)>>12); \
  b -= c; b -= a; b = (b ^ (a<<16)) & 0xffffffff; \
  c -= a; c -= b; c = (c ^ (b>> 5)) & 0xffffffff; \
  a -= b; a -= c; a = (a ^ (c>> 3)) & 0xffffffff; \
  b -= c; b -= a; b = (b ^ (a<<10)) & 0xffffffff; \
  c -= a; c -= b; c = (c ^ (b>>15)) & 0xffffffff; \
}

/*
 * hash() -- hash a variable-length key into a 32-bit value
 *   k     : the key (the unaligned variable-length array of bytes)
 *   len   : the length of the key, counting by bytes
 *   level : can be any 4-byte value
 * Returns a 32-bit value.  Every bit of the key affects every bit of
 * the return value.  Every 1-bit and 2-bit delta achieves avalanche.
 * About 36+6len instructions.
 * 
 * The best hash table sizes are powers of 2.  There is no need to do
 * mod a prime (mod is sooo slow!).  If you need less than 32 bits,
 * use a bitmask.  For example, if you need only 10 bits, do
 *   h = (h & hashmask(10));
 * In which case, the hash table should have hashsize(10) elements.
 * 
 * If you are hashing n strings (ub1 **)k, do it like this:
 *   for (i=0, h=0; i<n; ++i) h = hash( k[i], len[i], h);
 * 
 * By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
 * code any way you wish, private, educational, or commercial.  It's free.
 * 
 * See http://burtleburtle.net/bob/hash/evahash.html
 * Use for hash table lookup, or anything where one collision in 2^32 is
 * acceptable.  Do NOT use for cryptographic purposes.
 * [and see the gcc source]
 */

typedef void *PTR;
typedef unsigned int hashval_t;

/* 
 * iterative_hash
 * const PTR k_in ( == the key )
 * size_t length ( == the length of the key )
 * hashval_t initval ( == previous hash, or an arbitrary value )
 */

static hashval_t
iterative_hash (const PTR k_in, size_t length, hashval_t initval)
{
  const unsigned char *k = (const unsigned char *) k_in;
  hashval_t a = (hashval_t) 0;
  hashval_t b = (hashval_t) 0;
  hashval_t c = (hashval_t) 0;
  hashval_t len = (hashval_t) 0;

  /* Set up the internal state */
  len = length;
  a = b = 0x9e3779b9;		/* the golden ratio; an arbitrary value */
  c = initval;			/* the previous hash value */

  /*---------------------------------------- handle most of the key */
  while (len >= 12)
    {
      a =
	a + (k[0] + ((hashval_t) k[1] << 8) + ((hashval_t) k[2] << 16) +
	     ((hashval_t) k[3] << 24));
      b =
	b + (k[4] + ((hashval_t) k[5] << 8) + ((hashval_t) k[6] << 16) +
	     ((hashval_t) k[7] << 24));
      c =
	c + (k[8] + ((hashval_t) k[9] << 8) + ((hashval_t) k[10] << 16) +
	     ((hashval_t) k[11] << 24));
      mix (a, b, c);
      k = k + 12;
      len = len - 12;
    }
  /*------------------------------------- handle the last 11 bytes */
  c = c + length;
  switch (len)			/* all the case statements fall through */
    {
    case 11:
      c = c + ((hashval_t) k[10] << 24);
    case 10:
      c = c + ((hashval_t) k[9] << 16);
    case 9:
      c = c + ((hashval_t) k[8] << 8);
      /* the first byte of c is reserved for the length */
    case 8:
      b = b + ((hashval_t) k[7] << 24);
    case 7:
      b = b + ((hashval_t) k[6] << 16);
    case 6:
      b = b + ((hashval_t) k[5] << 8);
    case 5:
      b = b + k[4];
    case 4:
      a = a + ((hashval_t) k[3] << 24);
    case 3:
      a = a + ((hashval_t) k[2] << 16);
    case 2:
      a = a + ((hashval_t) k[1] << 8);
    case 1:
      a = a + k[0];
      /* case 0: nothing left to add */
    default:
      /* Pacify. */
      break;
    }
  mix (a, b, c);
  /*-------------------------------------------- report the result */
  return ((hashval_t) c);
}

/* */
static unsigned int
hashingcode (char *s)
{
  unsigned int hashval = 0;
  hashval =
    (unsigned int) iterative_hash ((const PTR) s, (size_t) strlen (s),
				   (hashval_t) 0xbeefbeef);
  return ((unsigned int) (hashval % UNIQSTRHASHSIZ));
}

/* */
struct splay_node
{
  struct splay_node *left;
  struct splay_node *right;
  void *key;
};

/* */
static struct splay_node *splayroot = (struct splay_node *) 0;

/**
 * rebalance tree
 *
 * @param i key
 * @param t tree
 * @return tree
 **/
static struct splay_node *
splay (void *i, struct splay_node *t)
{
  struct splay_node tmp = {
    (struct splay_node *) 0,	/* left */
    (struct splay_node *) 0,	/* right */
    (void *) 0			/* key */
  };
  struct splay_node *l = (struct splay_node *) 0;
  struct splay_node *r = (struct splay_node *) 0;
  struct splay_node *y = (struct splay_node *) 0;
  int comparevalue = 0;

  /* no root. return 0 */
  if ((struct splay_node *) 0 == t)
    {
      return ((struct splay_node *) t);
    }

  tmp.left = (struct splay_node *) 0;
  tmp.right = (struct splay_node *) 0;

  l = &tmp;
  r = &tmp;

label1:

  comparevalue = strcmp (i, t->key);

  if (comparevalue < 0)
    {

      if (!t->left)
	{
	  goto label2;
	}

      if (strcmp (i, t->left->key) < 0)
	{

	  y = t->left;
	  t->left = y->right;
	  y->right = t;
	  t = y;

	  if (!t->left)
	    {
	      goto label2;
	    }
	}

      /* */
      r->left = t;

      r = t;
      t = t->left;

    }
  else if (comparevalue > 0)
    {

      if (!t->right)
	{
	  goto label2;
	}

      if (strcmp (i, t->right->key) > 0)
	{

	  y = t->right;
	  t->right = y->left;
	  y->left = t;
	  t = y;

	  if (!t->right)
	    {
	      goto label2;
	    }
	}

      /* */
      l->right = t;
      l = t;
      t = t->right;

    }
  else
    {
      goto label2;
    }

  goto label1;

label2:

  l->right = t->left;
  r->left = t->right;
  t->left = tmp.right;
  t->right = tmp.left;

  return ((struct splay_node *) t);
}


/**
 * insert string as key, return previous one if found, else newly malloced
 *
 * @param i key
 * @param t tree
 * @return tree
 **/
static struct splay_node *
splayinsert (void *i, struct splay_node *t)
{
  void *x = (void *) 0;
  struct splay_node *new_node = (struct splay_node *) 0;

  if (t)
    {
      /* */
      t = splay (i, t);

      /* if found return older copy */
      if (0 == strcmp (i, t->key))
	{
	  return ((struct splay_node *) t);
	}
    }

  /* Not found: add new. */
  new_node =
    (struct splay_node *) pool_malloc (uniqstrings,
				       (sizeof (struct splay_node)));

  /* Only at first time t==0 */
  if (!t)
    {
      new_node->left = (struct splay_node *) 0;
      new_node->right = (struct splay_node *) 0;
    }
  else
    {

      if (strcmp (i, t->key) < 0)
	{
	  new_node->left = t->left;
	  new_node->right = t;
	  t->left = (struct splay_node *) 0;
	}
      else
	{
	  /* >0 or == 0 */
	  new_node->right = t->right;
	  new_node->left = t;
	  t->right = (struct splay_node *) 0;
	}
    }

  /* New copy. */
  new_node->key =
    (void *) pool_malloc (uniqstrings, (strlen ((char *) i) + 1));

  /* */
  x =
    (void *) memmove ((char *) new_node->key, (char *) i,
		      strlen ((char *) i));

  return ((struct splay_node *) new_node);
}

/* */
static void
splayinit_syms (void)
{
  int i = 0;
  splayroot = (struct splay_node *) 0;
  /* clear tables */
  for (i = 0; i < UNIQSTRHASHSIZ; i++)
    {
      hashtab1[i] = (struct splay_node *) 0;
      hashtab2[i] = (struct splay_node *) 0;
    }
  return;
}

/* */
static void
getsplayroot (size_t i, size_t ix)
{

  if (i < SMALLUNIQSTRING)
    {
      /* Small strings, like "abc" or "123" */
      splayroot = hashtab1[ix];
    }
  else
    {
      /* Bigger strings. */
      splayroot = hashtab2[ix];
    }

  return;
}

/**
 * switch root
 *
 * @ param i length of string
 * @param ix index into table
 **/
static void
setsplayroot (size_t i, size_t ix)
{

  if (i < SMALLUNIQSTRING)
    {
      /* "abc" or "123" */
      hashtab1[ix] = splayroot;
    }
  else
    {
      /* larger */
      hashtab2[ix] = splayroot;
    }

  return;
}

/**
 * like strdup
 *
 * @param s
 * @return allocated string
 **/
static char *
splaystrdupsym (char *s)
{
  char *ret = (char *) 0;
  unsigned int i = 0;
  size_t len = 0;
  len = strlen (s);
  i = hashingcode (s);
  getsplayroot (len, i);
  splayroot = splayinsert ((void *) s, splayroot);
  setsplayroot (len, i);
  ret = ((char *) splayroot->key);
  return ((char *) ret);
}

/* End. */
