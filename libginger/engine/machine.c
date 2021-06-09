/*
Checksum:  447069034      (machine.c)
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
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "gingerprotos.h"

/* */
static int justone = 1;
static int justzero = 0;

/* */
static void
float_signal (int signo)
{
  fputs ("SIGFPE signal\n", stderr);
  fflush (stderr);
  exit (33);
}

/* */
static void
segv_signal (int signo)
{
  fputs ("SIGSEGV signal\n", stderr);
  fflush (stderr);
  exit (33);
}

/* */
int
machine_init (int argc, char **argv)
{
  int tmp = 0;
  struct rlimit rlim;
  char *p = (char *) 0;

  /*
   * from /bin/bash
   * pid_t pid = 0;
   * if ((pid = fork ()) < 0) {
   *   machine_has_too_high_cpu_load();
   * }
   * if (pid) { (parent) }
   * pid == 0 (child)
   */

  /* from GNU m4
   * measure stack direction at runtime.
   * struct rlimit rl;
   * rlim_t stack_len;
   * int grows_upward = 0;
   * int tmp = 0;
   * char *stackbot = (char *)0; this must be the **argv from commandline 
   * tmp = getrlimit (RLIMIT_STACK, &rl);
   * if (tmp < 0)
   * {
   *   return (0);
   * }
   * stack_len = (rl.rlim_cur < rl.rlim_max ? rl.rlim_cur : rl.rlim_max);
   * stackbot = (char *) argv;
   * grows_upward = ((char *) &stack_len > stackbot);
   * if (grows_upward)
   * {
   *   Grows "upward" (toward increasing addresses)
   * }
   * else
   * {
   *    Grows "downward" (toward decreasing addresses)
   * }
   */

  /* set program name */
  p = argv[0] + strlen (argv[0]);

  while (p != argv[0] && p[-1] != '/')
    {
      p--;
    }

  progname = p;

  /* set signal handler */
  if (justone)
    {
      /* Pacify. */
      justzero = 0;
      signal (SIGFPE, float_signal);
      signal (SIGSEGV, segv_signal);
    }

  /* Pacify. */
  if (justzero)
    {
      justone = 1;
    }

  errno = 0;

  /* safer */
  tmp = nice (18);

  if (errno || tmp < 0)
    {
      return (1);
    }

  errno = 0;

  /* set stack limit maximal */
  tmp = getrlimit (RLIMIT_STACK, &rlim);

  if (errno || tmp < 0)
    {
      return (1);
    }

  rlim.rlim_cur = rlim.rlim_max;

  errno = 0;

  tmp = setrlimit (RLIMIT_STACK, &rlim);

  if (errno || tmp < 0)
    {
      return (1);
    }

  /*
   * #ifdef MPI_FEATURE
   * MPI_Comm comm;
   * MPI_Init(&argc, &argv);
   * MPI_Comm_rank(MPI_COMM_WORLD, &me);
   * MPI_Comm_size(MPI_COMM_WORLD, &tasks);
   * MPI_Barrier(comm);
   * MPI_Comm_free(&comm);
   * MPI_Finalize();
   */

  /* random generator ginger_randinit (); */

  return (0);
}

/* */
int
machine_deinit (void)
{
  /* */
  return (0);
}

/* */
int
machine_check (FILE * stream)
{

  fprintf (stream, "sizeof(int) is %d (%d)\n", (int) sizeof (int),
	   (int) machine_intbits ());
  fprintf (stream, "sizeof(long) is %d (%d)\n", (int) sizeof (long),
	   (int) machine_longsize ());
  fprintf (stream, "sizeof(short) is %d (%d)\n", (int) sizeof (short),
	   (int) machine_shortsize ());
  fprintf (stream, "endianess %d\n", (int) machine_endian ());

#ifdef MPI
#include <mpi.h>
  fprintf (stream,
	   "local area multicomputing mpi version %d.%d is compiled-in\n",
	   MPI_VERSION, MPI_SUBVERSION);
#endif

#ifdef GIFLIB
  fprintf (stream, "giflib version %s is compiled-in\n", GIF_LIB_VERSION);
#endif

#ifdef PPM			/* */
  fprintf (stream, "ppm version P6 is compiled-in\n");
#endif

#ifdef PNG
#include "png.h"
  fprintf (stream, "png version %s is compiled-in\n", PNG_LIBPNG_VER_STRING);
#endif

#ifdef MNG
  fprintf (stream, "mng version %s is compiled-in\n", MNG_VERSION_TEXT);
#endif

#ifdef JPG
#include "jpeglib.h"
  fprintf (stream, "jpg version %d is compiled-in\n", JPEG_LIB_VERSION);
#endif

#ifdef ZLIB
#include "zlib.h"
  fprintf (stream, "zlib version %s is compiled-in\n", ZLIB_VERSION);
#endif

/* bzlib has no version info */
#ifdef BZLIB
#include "bzlib.h"
#ifdef BZ_NO_STDIO
  fprintf (stream,
	   "bzlib version is compiled-in without stdio feature (BZ_NO_STDIO defined)\n");
#endif
#endif

  fprintf (stream, "__GNUC__ version is %d.%d\n", __GNUC__, __GNUC_MINOR__);

#ifdef __USE_ISOC99
  fprintf (stream, "__USE_ISOC99 is defined\n");
#endif

#ifdef	__USE_POSIX
  fprintf (stream, "__USE_POSIX is defined\n");
#endif

#ifdef	__USE_POSIX2
  fprintf (stream, "__USE_POSIX2 is defined\n");
#endif

#ifdef	__USE_XOPEN
  fprintf (stream, "__USE_XOPEN is defined\n");
#endif

#ifdef __USE_POSIX199506
  fprintf (stream, "__USE_POSIX199506 is defined\n");
#endif

#ifdef __USE_UNIX98
  fprintf (stream, "__USE_UNIX98 is defined\n");
#endif

#ifdef __USE_BSD
  fprintf (stream, "__USE_BSD is defined\n");
#endif

#ifdef __USE_XOPEN_EXTENDED
  fprintf (stream, "__USE_XOPEN_EXTENDED is defined\n");
#endif

#ifdef HAVE_SIGSEGV_RECOVERY
  fprintf (stream, "HAVE_SIGSEGV_RECOVERY is defined\n");
#endif

#ifdef HAVE_STACK_OVERFLOW_RECOVERY
  fprintf (stream, "HAVE_STACK_OVERFLOW_RECOVERY is defined\n");
#endif

#if __WORDSIZE == 64
  fprintf (stream, "__WORDSIZE == 64\n");
#endif

#if __WORDSIZE == 32
  fprintf (stream, "__WORDSIZE == 32\n");
#endif

  fflush (stream);

  return (0);
}

/* approx size of a memory block. */
size_t
machine_memchunk (void)
{
  if (machine_intbits () == 32)
    {
      return 4 * 1024;
    }
  else
    {
      /* 64bits machine. */
      return 8 * 1024;
    }
}

/**
 * endian of machine
 *
 * 1=big, 0=little
 *
 * @return endianness
 **/
int
machine_endian (void)
{
  int ret = 0;
  int tmpi = 0;
  char endian = 0;

  tmpi = 1;
  endian = !(*((char *) (&tmpi)));

  if (endian)
    {
      /* big endian */
      ret = 1;
    }
  else
    {
      /* little endian */
      ret = 0;
    }

  return (ret);
}

/**
 * how many bits in a int datatype 
 *
 * @return number of bits
 **/
int
machine_intbits (void)
{
  int i = 0;
  unsigned int ch = 1;
  unsigned int oldch = 0;

  while (ch > oldch)
    {
      oldch = ch;
      ch <<= 1;
      i = i + 1;
    }

  return i;
}

/**
 * how many bits in a long datatype 
 *
 * @return number of bits
 **/
int
machine_longsize (void)
{
  int i = 0;
  unsigned long ch = 1;
  unsigned long oldch = 0;

  while (ch > oldch)
    {
      oldch = ch;
      ch <<= 1;
      i = i + 1;
    }

  return i;
}

/**
 * how many bits in a short datatype 
 *
 * @return number of bits
 **/
int
machine_shortsize (void)
{
  int i = 0;
  unsigned short ch = 1;
  unsigned short oldch = 0;

  while (ch > oldch)
    {
      oldch = ch;
      ch <<= 1;
      i = i + 1;
    }

  return i;
}

/* End. */
