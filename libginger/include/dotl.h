/*
Checksum: 3491537725      (dotl.h)
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

/* */
extern int dot_parse (void);
extern int dot_lex (void);

/* lexer */
extern int dot_lexer_init (void);
extern int dot_lexer_deinit (void);
extern void *dot_lexer_malloc (size_t n);
extern void dot_lexer_free (void *p);
extern void *dot_lexer_realloc (void *p, size_t n);
extern char *dot_lexer_version (void);
extern int dot_input (void *ptr, size_t size, size_t nmemb);
extern void dot_set_stream (FILE *stream, int debugflag);
extern int dot_wrap (void);

/* End. */
