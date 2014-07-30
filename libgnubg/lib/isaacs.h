/*
 * ------------------------------------------------------------------------------
 * Standard definitions and types, Bob Jenkins
 * Modified for inclusion with GNU Backgammon by Gary Wong
 * $Id: isaacs.h,v 1.4 2013/06/16 02:16:24 mdpetch Exp $
 * ------------------------------------------------------------------------------
 */
#ifndef ISAACS_H
#define ISAACS_H
typedef unsigned long int ub4;  /* unsigned 4-byte quantities */
#define UB4MAXVAL 0xffffffff
typedef signed long int sb4;
#define SB4MAXVAL 0x7fffffff
typedef unsigned short int ub2;
#define UB2MAXVAL 0xffff
typedef signed short int sb2;
#define SB2MAXVAL 0x7fff
typedef unsigned char ub1;
#define UB1MAXVAL 0xff
typedef signed char sb1;        /* signed 1-byte quantities */
#define SB1MAXVAL 0x7f
typedef int word;               /* fastest type available */

#endif
