#ifdef IS_ANDROID
  #define _(s) s
  #define N_(s) s
  #define gettext(s) s
#endif

/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Define if you want to disable the SSE cpu test */
#define DISABLE_SSE_TEST 1

/* always defined to indicate that i18n is enabled */
#define ENABLE_NLS 1

/* The package name, for gettext */
#define GETTEXT_PACKAGE "gnubg"

/* Define if you wish to use glib threads */
#define GLIB_THREADS 1

/* Define if you want to disable deprecated gtk functions */
/* #undef GTK_DISABLE_DEPRECATED */

/* Define if you want to disable deprecated glib functions */
/* #undef G_DISABLE_DEPRECATED */

/* Define if you have Apple CoreAudio */
/* #undef HAVE_APPLE_COREAUDIO */

/* Define if you have Apple QuickTime */
/* #undef HAVE_APPLE_QUICKTIME */

/* Define to 1 if you have the `bind_textdomain_codeset' function. */
#define HAVE_BIND_TEXTDOMAIN_CODESET 1

/* Define if you want to use libcairo */
#define HAVE_CAIRO 0
#undef HAVE_CAIRO
/* Define if you want to use gstreamer */
/* #undef HAVE_CANBERRA */

/* Define to 1 if you have the `dcgettext' function. */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the declaration of `signbit', and to 0 if you
   don't. */
#define HAVE_DECL_SIGNBIT 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if you have freetype2 */
#define HAVE_FREETYPE 0
#undef HAVE_FREETYPE

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define to 1 if you have the <GL/glx.h> header file. */
#define HAVE_GL_GLX_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the `gmp' library (-lgmp). */
/* #undef HAVE_LIBGMP */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define if you have libpng */
#define HAVE_LIBPNG 0
#undef HAVE_LIBPNG

/* Define if you have libreadline */
#define HAVE_LIBREADLINE 0
#undef HAVE_LIBREADLINE


/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if you want to use pangocairo */
#define HAVE_PANGOCAIRO 0
#undef HAVE_PANGOCAIRO

/* Define to 1 if you have the `random' function. */
#define HAVE_RANDOM 1

/* Define to 1 if you have the `setpriority' function. */
#define HAVE_SETPRIORITY 1

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

/* Define to 1 if you have the `sigvec' function. */
/* #undef HAVE_SIGVEC */

/* Define if the system supports AF_LOCAL sockets. */
#define HAVE_SOCKETS 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strptime' function. */
#define HAVE_STRPTIME 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "gnubg"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "bug-gnubg@gnu.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GNU Backgammon"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GNU Backgammon 0.90.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gnubg"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://www.gnu.org/software/gnubg/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.90.0"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if using Apple OpenGL */
/* #undef USE_APPLE_OPENGL */

/* Define if you want to use the 3d boards */
/* #undef USE_BOARD3D */

/* Define if you want extended MatchID support */
#define USE_EXTENDEDMATCHID 1

/* Define if you want to use the gtk gui */
#define USE_GTK 0
#undef USE_GTK

/* Define if you want to have multithread support */
#define USE_MULTITHREAD 0
#undef USE_MULTITHREAD

/* Define if you want to use Python inside gnubg */
#define USE_PYTHON 0
#undef USE_PYTHON

/* Define if you want to use sqlite */
#define USE_SQLITE 0
#undef USE_SQLITE

/* Define if you want to compile with SSE2 support */
#define USE_SSE2 0
#undef USE_SSE2

/* Define if you want to compile with SSE support */
#define USE_SSE_VECTORIZE 0
#undef USE_SSE_VECTORIZE

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Version number of package */
#define VERSION "0.90.0"

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
