/* Define if you want to disable the SIMD CPU instruction test */
#define DISABLE_SIMD_TEST 1

/* always defined to indicate that i18n is enabled */
#undef ENABLE_NLS

/* The package name, for gettext */
#define GETTEXT_PACKAGE "gnubg"

/* Define if you wish to use glib threads */
#define GLIB_THREADS 1

/* Define to 1 if you have the `bind_textdomain_codeset' function. */
#define HAVE_BIND_TEXTDOMAIN_CODESET 1

/* Define to 1 if you have the `dcgettext' function. */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the declaration of `signbit', and to 0 if you don't. */
#define HAVE_DECL_SIGNBIT 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define if you have libreadline */
#define HAVE_LIBREADLINE 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the <mcheck.h> header file. */
#define HAVE_MCHECK_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mtrace' function. */
#define HAVE_MTRACE 1

/* Define to 1 if you have the `random' function. */
#define HAVE_RANDOM 1

/* Define to 1 if you have the `setpriority' function. */
#define HAVE_SETPRIORITY 1

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

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

/* maximum number of evaluation threads */
#define MAX_NUMTHREAD 8

/* Name of package */
#define PACKAGE "gnubg"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "bug-gnubg@gnu.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GNU Backgammon"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GNU Backgammon 1.03.000"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gnubg"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://www.gnu.org/software/gnubg/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.03.000"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if you want extended MatchID support */
#define USE_EXTENDEDMATCHID 1

/* Define if you want to use the gtk gui */
/* #undef USE_GTK */

/* Define if you want to have multithread support */
#define USE_MULTITHREAD 1

/* Define if you want to use sqlite */
#undef USE_SQLITE

/* Version number of package */
#define VERSION "1.03.000"

#if IS_ANDROID
#define _(s) (s)
#define gettext(s) (s)
#define N_(s) (s)
#endif
