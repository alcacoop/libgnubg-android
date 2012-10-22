%{
/*
 * external_y.y -- command parser for external interface
 *
 * by Jørn Thyssen <jth@gnubg.org>, 2003.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 3 or later of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: external_y.y,v 1.18 2012/04/26 17:02:24 mdpetch Exp $
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "external.h"
#include "backgammon.h"

extern int extlex(void);
extern int exterror(const char *s);

extcmd ec; 

static void reset_command(void);

void ( *ExtErrorHandler )( const char *, const char *, const int ) = NULL;


%}

%name-prefix="ext"
%defines
%token-table

%union {
  int number;
  char *sval;
}

%token <sval> STRING
%token <number> NUMBER
%token EVALUATION PLIES CUBE CUBEFUL CUBELESS NOISE REDUCED PRUNE
%token CRAWFORDRULE JACOBYRULE
%token FIBSBOARD
%token <sval> AFIBSBOARD
%token ON
%token OFF

%%

the_command    : /* empty */  
               | reset_command command      
               ;

command        : evaluation
               | cmdfibsboard sessionrules
               ;

reset_command  : /* empty */ {
  reset_command();
}
;
 
cmdfibsboard   : AFIBSBOARD { 
  ec.ct = COMMAND_FIBSBOARD;
  free( ec.szFIBSBoard );
  ec.szFIBSBoard = $1;
}
               ;

fibsboard      : AFIBSBOARD { 
  free( ec.szFIBSBoard );
  ec.szFIBSBoard = $1;
}
               ;

optjacobyrule  : JACOBYRULE ON { ec.fJacobyRule = TRUE; }
               | JACOBYRULE OFF { ec.fJacobyRule = FALSE; }
               | /* empty */ { ec.fJacobyRule = fJacoby; }
               ;

optcrawfordrule : CRAWFORDRULE ON { ec.fCrawfordRule = TRUE; }
               |  CRAWFORDRULE OFF { ec.fCrawfordRule = FALSE; }
               |  /* empty */ { ec.fCrawfordRule = TRUE; }
               ;

optplies       : PLIES NUMBER { ec.nPlies = $2; }
               | /* empty */
               ;

optcube        : CUBE ON  { ec.fCubeful = TRUE; }
               | CUBE OFF { ec.fCubeful = FALSE; }
               | /* empty */
               ;

optcubeful     : CUBEFUL { ec.fCubeful = TRUE; }
               | /* empty */
               ;

optprune     : PRUNE { ec.fUsePrune = TRUE; }
               | /* empty */
               ;

optcubeless    : CUBELESS { ec.fCubeful = FALSE; }
               | /* empty */
               ;

optnoise       : NOISE NUMBER /* FIXME: FLOAT */ { ec.rNoise = $2; }
               | /* empty */
               ;

optreduced     : REDUCED NUMBER { ec.nReduced = $2; }
               | /* empty */
               ;

sessionrules   : optcrawfordrule optjacobyrule
               ;

evalcontext    : optplies optcube optcubeful optprune optcubeless optnoise optreduced
               ;

evaluation     : EVALUATION FIBSBOARD fibsboard evalcontext sessionrules {
  ec.ct = COMMAND_EVALUATION;
}
;

%%

/* lexer interface */

extern FILE *extin;
extern char *exttext;

extern int
exterror( const char *s ) {

  if ( ExtErrorHandler )
    ExtErrorHandler( s, exttext && exttext[ 0 ] ? exttext : "<EOT>", 1);
  else
    fprintf( stderr, "Error: %s at %s\n", 
             s, exttext && exttext[ 0 ] ? exttext : "<EOT>" );

  return 0;
}

static void
reset_command(void) {

  ec.ct = COMMAND_NONE;
  ec.nPlies = 0;
  ec.rNoise = 0;
  ec.fDeterministic = 1;
  ec.fCubeful = 0;
  ec.nReduced = 0;
  ec.fUsePrune = 0;
  ec.fCrawfordRule = 1;
  ec.fJacobyRule = fJacoby;
  free(ec.szFIBSBoard);
  ec.szFIBSBoard = NULL;

}


#ifdef EXTERNAL_TEST

extern int
main( int argc, char *argv[] ) {

  FILE *pf = NULL;

  if ( argc > 1 )
    if ( ! ( pf = fopen( argv[ 1 ], "r" ) ) ) {
      perror( argv[ 1 ] );
      return 1;
    }

  extin = pf ? pf : stdin;
  extparse();
  fclose( extin );

  printf( "command type %d\n"
          "plies %d\n"
          "noise %f\n"
          "deterministic %d\n"
          "cubeful %d\n"
          "reduced %d\n"
          "prune %d\n"
          "fibsboard %s\n",
          "crawfordrule %s\n",
          "jacobyrule %s\n",
          ec.ct, ec.nPlies, ec.rNoise, ec.fDeterministic, ec.fCubeful,
          ec.nReduced, ec.fUsePrune, ec.szFIBSBoard, ec.fCrawfordRule, 
          ec.fJacobyRule );

  return 0;

}

#endif
