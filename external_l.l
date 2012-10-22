%{

/*
 * external_l.l -- lexer for external interface
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
 * $Id: external_l.l,v 1.17 2012/05/18 20:15:00 mdpetch Exp $
 */

#include "config.h"
#include <string.h>

#include "external_y.h"

extern int extparse(void);

int prc_lineno = 1;
void escapes(const char *cp, char *tp);
%}

%option noyywrap
%option prefix="ext"
%option nounput
%option 7bit
%option case-insensitive
%option outfile="lex.yy.c"
%option noinput

NUMSTR -?[0-9]+

%%

(\"[^\"]*\")|(\'[^\']*\')	{
			char *buf = malloc( strlen( exttext ) + 1 );
			exttext[strlen(exttext)-1] = '\0';
			escapes(exttext+1, buf);
			extlval.sval = buf;
			BEGIN(INITIAL);
			return STRING;
		}


jacobyrule      { return JACOBYRULE; }
crawfordrule    { return CRAWFORDRULE; }
evaluation      { return EVALUATION; }
fibsboard       { return FIBSBOARD; }
cubeful         { return CUBEFUL; }
cubeless        { return CUBELESS; }
cube            { return CUBE; }
plies           { return PLIES; }
noise           { return NOISE; }
reduced         { return REDUCED; }
prune           { return PRUNE; }
yes             { return ON; }
on              { return ON; }
no              { return OFF; }
off             { return OFF; }

board:[[:alnum:]_]+:[[:alnum:]_]+(:{NUMSTR})+  { 
			char *buf = malloc( strlen( exttext ) + 1 );
			escapes(exttext, buf);
			extlval.sval = buf;
			return AFIBSBOARD; 
		}


(#.*)?\\?\n	{ prc_lineno++;	}   /* newline is ignored */

{NUMSTR}	{ extlval.number = atoi(exttext); return NUMBER; }

[^=;:, \t\r\n]+	{
			char *buf = malloc( strlen( exttext ) + 1 );
			escapes(exttext, buf);
			extlval.sval = buf;
			return STRING;
		}

[ \t\r]+	;	/* whitespace */

%%

void escapes(const char *cp, char *tp)
{
    while (*cp)
    {
	int	cval = 0;

	if (*cp == '\\' && strchr("0123456789xX", cp[1]))
	{
	    char *dp;
	    const char *hex = "00112233445566778899aAbBcCdDeEfF";
	    int dcount = 0;

	    if (*++cp == 'x' || *cp == 'X')
		for (++cp; (dp = strchr(hex, *cp)) && (dcount++ < 2); cp++)
		    cval = (cval * 16) + (dp - hex) / 2;
	    else if (*cp == '0')
		while (strchr("01234567",*cp) != (char*)NULL && (dcount++ < 3))
		    cval = (cval * 8) + (*cp++ - '0');
	    else
		while ((strchr("0123456789",*cp)!=(char*)NULL)&&(dcount++ < 3))
		    cval = (cval * 10) + (*cp++ - '0');
	}
	else if (*cp == '\\')		/* C-style character escapes */
	{
	    switch (*++cp)
	    {
	    case '\\': cval = '\\'; break;
	    case 'n': cval = '\n'; break;
	    case 't': cval = '\t'; break;
	    case 'b': cval = '\b'; break;
	    case 'r': cval = '\r'; break;
	    default: cval = *cp;
	    }
	    cp++;
	}
	else
	    cval = *cp++;
	*tp++ = cval;
    }
    *tp = '\0';
}

extern void ExtStartParse(const char* szCommand);
void ExtStartParse(const char* szCommand)
{
	YY_BUFFER_STATE buf_state;
	buf_state = ext_scan_string(szCommand);
	extparse();
	ext_delete_buffer(buf_state);
}
