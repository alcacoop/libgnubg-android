/*
 * external.c
 *
 * by Gary Wong <gtw@gnu.org>, 2001, 2002.
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
 * $Id: external.c,v 1.72 2012/05/18 09:18:04 gflohr Exp $
 */

#include "config.h"

#if HAVE_SOCKETS

#include <signal.h>
#include <glib.h>
#include <glib/gstdio.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* #if HAVE_UNISTD_H */

#ifndef WIN32

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_SYS_SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#endif /* #if HAVE_SYS_SOCKET_H */

#include <stdio.h>

#else /* #ifndef WIN32 */
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define EWOULDBLOCK             WSAEWOULDBLOCK
#define EINPROGRESS             WSAEINPROGRESS
#define EALREADY                WSAEALREADY
#define ENOTSOCK                WSAENOTSOCK
#define EDESTADDRREQ            WSAEDESTADDRREQ
#define EMSGSIZE                WSAEMSGSIZE
#define EPROTOTYPE              WSAEPROTOTYPE
#define ENOPROTOOPT             WSAENOPROTOOPT
#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#define EOPNOTSUPP              WSAEOPNOTSUPP
#define EPFNOSUPPORT            WSAEPFNOSUPPORT
#define EAFNOSUPPORT            WSAEAFNOSUPPORT
#define EADDRINUSE              WSAEADDRINUSE
#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
#define ENETDOWN                WSAENETDOWN
#define ENETUNREACH             WSAENETUNREACH
#define ENETRESET               WSAENETRESET
#define ECONNABORTED            WSAECONNABORTED
#define ECONNRESET              WSAECONNRESET
#define ENOBUFS                 WSAENOBUFS
#define EISCONN                 WSAEISCONN
#define ENOTCONN                WSAENOTCONN
#define ESHUTDOWN               WSAESHUTDOWN
#define ETOOMANYREFS            WSAETOOMANYREFS
#define ETIMEDOUT               WSAETIMEDOUT
#define ECONNREFUSED            WSAECONNREFUSED
#define ELOOP                   WSAELOOP
#define ENAMETOOLONG            WSAENAMETOOLONG
#define EHOSTDOWN               WSAEHOSTDOWN
#define EHOSTUNREACH            WSAEHOSTUNREACH
#define ENOTEMPTY               WSAENOTEMPTY
#define EPROCLIM                WSAEPROCLIM
#define EUSERS                  WSAEUSERS
#define EDQUOT                  WSAEDQUOT
#define ESTALE                  WSAESTALE
#define EREMOTE                 WSAEREMOTE
#define EINVAL                  WSAEINVAL
#define EINTR                   WSAEINTR

#define inet_aton(ip,addr)  (addr)->s_addr = inet_addr(ip), 1
#define inet_pton(fam,ip,addr) (addr)->s_addr = inet_addr(ip), 1

#endif /* #ifndef WIN32 */

#endif /* HAVE_SOCKETS */

#include "backgammon.h"
#include "drawboard.h"
#include "external.h"
#include "rollout.h"

#if HAVE_SOCKETS
/* Stuff for the yacc/lex parser */

extern extcmd ec;
static int fError = FALSE;
static char *szError = NULL;
extern void ExtStartParse( const char *s );


#if defined(AF_UNIX) && !defined(AF_LOCAL)
#define AF_LOCAL AF_UNIX
#define PF_LOCAL PF_UNIX
#endif

#ifdef WIN32
void OutputWin32SocketError(const char* action)
{
	LPVOID lpMsgBuf;
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL) != 0)
	{
		outputerrf("%s (%s):\n%s", _("Windows socket error"), action, (LPCTSTR)lpMsgBuf);
		if (LocalFree(lpMsgBuf) != NULL)
			g_print("LocalFree() failed\n");
	}
}
#endif

extern int ExternalSocket( struct sockaddr **ppsa, int *pcb, char *sz ) {

    int sock, f;
#ifndef WIN32
    struct sockaddr_un *psun;
#endif
    struct sockaddr_in *psin;
    struct hostent *phe;
    char *pch;
    
    if( ( pch = strchr( sz, ':' ) ) && !strchr( sz, '/' ) ) {
	/* Internet domain socket. */
	if( ( sock = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
	    return -1;

	f = TRUE;

#ifdef WIN32
	if( setsockopt( (SOCKET) sock, SOL_SOCKET, SO_REUSEADDR, (const char*) &f, sizeof f ) )
#else
	if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &f, sizeof f ) )
#endif /* WIN32 */
	    return -1;

	psin = malloc( *pcb = sizeof (struct sockaddr_in) );
	memset( psin, 0, sizeof (*psin));

	psin->sin_family = AF_INET;

	/* split hostname and port */
	*pch = 0;

	if( !*sz )
	    /* no host specified */
	    psin->sin_addr.s_addr = htonl( INADDR_ANY );
#ifdef WIN32
	else {
#else
	else if( !inet_aton( sz, &psin->sin_addr ) ) {
#endif /* WIN32 */
	    if( ( phe = gethostbyname( sz ) ) == 0 ) {
		*pch = ':';
		errno = EINVAL;
		free( psin );
		return -1;
	    }

#ifdef WIN32
	    memcpy( &(psin->sin_addr), (struct in_addr *) (phe->h_addr), phe->h_length);
#else
	    psin->sin_addr = *(struct in_addr *) phe->h_addr;
#endif /* WIN32 */

	}

	*pch++ = ':';
	
	psin->sin_port = htons( (u_short)atoi( pch ) );
	
	*ppsa = (struct sockaddr *) psin;
    } else {
#ifndef WIN32
	/* Local domain socket. */
	if( ( sock = socket( PF_LOCAL, SOCK_STREAM, 0 ) ) < 0 )
	    return -1;

	/* yuck... there's no portable way to obtain the necessary
	   sockaddr_un size, but this is a conservative estimate */
	psun = malloc( *pcb = 16 + strlen( sz ) );
	
	psun->sun_family = AF_LOCAL;
	strcpy( psun->sun_path, sz );
	*ppsa = (struct sockaddr *) psun;
#else /* #ifndef WIN32 */
	/* FIXME: what will we do on Windows? */
	return -1;
#endif /* #ifndef WIN32 */
    }

    return sock;
}
#endif /* HAVE_SOCKETS */

#if HAVE_SOCKETS
static void ExternalUnbind( char *sz ) {

    if( strchr( sz, ':' ) && !strchr( sz, '/' ) )
	/* it was a TCP socket; no cleanup necessary */
	return;

    g_unlink( sz );
}
#endif /* HAVE_SOCKETS */

#if HAVE_SOCKETS
extern int ExternalRead( int h, char *pch, size_t cch ) {

    char *p = pch, *pEnd;
    int n;
#ifndef WIN32
    psighandler sh;
#endif
    
    while( cch )
	{
		ProcessEvents();

	if( fInterrupt )
	    return -1;

#ifndef WIN32
	PortableSignal( SIGPIPE, SIG_IGN, &sh, FALSE );
#endif

#ifdef WIN32
	/* reading from sockets doesn't work on Windows
	   use recv instead */
	n = recv( (SOCKET) h, p, cch, 0);
#else
	n = read( h, p, cch );
#endif

#ifndef WIN32
	PortableSignalRestore( SIGPIPE, &sh );
#endif
	
	if( !n ) {
	    outputl( _("External connection closed.") );
	    return -1;
	} else if( n < 0 ) {
	    if( errno == EINTR )
		continue;

	    SockErr( _("reading from external connection") );
	    return -1;
	}
	
	if( ( pEnd = memchr( p, '\n', n ) ) ) {
	    *pEnd = 0;
	    return 0;
	}
	
	cch -= n;
	p += n;
	
    }

    p[ cch - 1 ] = 0;
    return 0;
}
#endif /* HAVE_SOCKETS */

#if HAVE_SOCKETS
extern int ExternalWrite( int h, char *pch, size_t cch ) {

    char *p = pch;
    int n;
#ifndef WIN32
    psighandler sh;
#endif

    while( cch )
	{
		ProcessEvents();

		if( fInterrupt )
			return -1;

#ifndef WIN32
	PortableSignal( SIGPIPE, SIG_IGN, &sh, FALSE );
#endif

#ifdef WIN32
	/* writing to sockets doesn't work on Windows
	   use send instead */
	n = send( (SOCKET) h, (const char *) p, cch, 0);
#else
	n = write( h, p, cch );
#endif

#ifndef WIN32
	PortableSignalRestore( SIGPIPE, &sh );
#endif
	
	if( !n )
	    return 0;
	else if( n < 0 ) {
	    if( errno == EINTR )
		continue;

	    SockErr( _("writing to external connection") );
	    return -1;
	}
	
	cch -= n;
	p += n;
    }

    return 0;
}
#endif /* HAVE_SOCKETS */


#if HAVE_SOCKETS
static void
ErrorHandler( const char *szMessage, const char *szNear, 
              const int fParseError ) {

  fError = TRUE;

  g_free( szError );
  szError = 
    g_strdup_printf( "Error (%s) near '%s'\n", szMessage, szNear );

}

static extcmd *
ExtParse( const char *szCommand ) {

  ExtErrorHandler = ErrorHandler;
  fError = FALSE;
  szError = NULL;
  ExtStartParse(szCommand);

  return fError ? NULL : &ec;

}

static char *
ExtEvaluation( extcmd *pec ) {

  char szName[ MAX_NAME_LEN ], szOpp[ MAX_NAME_LEN ];
  int nMatchTo, anScore[ 2 ],
    anDice[ 2 ], nCube, fCubeOwner, fDoubled, fCrawford, fJacoby;
  TanBoard anBoard;
  float arOutput[ NUM_ROLLOUT_OUTPUTS ];
  cubeinfo ci;
  int nScore, nScoreOpponent;
  char *szResponse;
  float r;
  evalcontext ec;

  if( ParseFIBSBoard( pec->szFIBSBoard, anBoard, szName, szOpp, &nMatchTo,
                      &nScore, &nScoreOpponent, anDice, &nCube,
                      &fCubeOwner, &fDoubled, &fCrawford ) ) {
    outputl( _("Warning: badly formed board from external controller.") );
    szResponse = 
      g_strdup_printf( "Error: badly formed board ('%s')\n", pec->szFIBSBoard );
  }
  else {
    
    anScore[ 0 ] = nScoreOpponent;
    anScore[ 1 ] = nScore;
	/* If the session isn't using Crawford rule, set crawford flag to false */
	fCrawford = pec->fCrawfordRule ? fCrawford : FALSE;
	/* Set the Jacoby flag appropriately from the external interface settings */
	fJacoby   = pec->fJacobyRule;

	/* printf ("Jacoby Setting: %d\n", fJacoby);*/
	/* printf ("Crawford Setting: %d\n", fCrawford); */
    
    SetCubeInfo ( &ci, nCube, fCubeOwner, 1, nMatchTo, anScore,
                  fCrawford, fJacoby, nBeavers, bgvDefault ); 

    ec.fCubeful = pec->fCubeful;
    ec.nPlies = pec->nPlies;
    ec.fUsePrune = pec->fUsePrune;
    ec.fDeterministic = pec->fDeterministic;
    ec.rNoise = pec->rNoise;

    if ( GeneralEvaluationE( arOutput, (ConstTanBoard)anBoard, &ci, &ec ) )
      return NULL;

    if ( nMatchTo ) {
      if ( ec.fCubeful )
        r = arOutput[ OUTPUT_CUBEFUL_EQUITY ];
      else
        r = eq2mwc( arOutput[ OUTPUT_EQUITY ], &ci );
    }
    else
      r = ec.fCubeful ? arOutput[ 6 ] : arOutput[ 5 ];
    
    szResponse = g_strdup_printf( "%f %f %f %f %f %f\n",
                                  arOutput[ 0 ],
                                  arOutput[ 1 ],
                                  arOutput[ 2 ],
                                  arOutput[ 3 ],
                                  arOutput[ 4 ],
                                  r );
  }

  return szResponse;

}

static char *
ExtFIBSBoard( extcmd *pec ) {

  char szName[ MAX_NAME_LEN ], szOpp[ MAX_NAME_LEN ];
  int nMatchTo, anScore[ 2 ],
    anDice[ 2 ], nCube, fCubeOwner, fDoubled, fCrawford, fJacoby,
    anMove[ 8 ], fTurn;
  TanBoard anBoard, anBoardOrig;
  float arDouble[ NUM_CUBEFUL_OUTPUTS ],
    aarOutput[ 2 ][ NUM_ROLLOUT_OUTPUTS ],
    aarStdDev[ 2 ][ NUM_ROLLOUT_OUTPUTS ];
  rolloutstat aarsStatistics[ 2 ][ 2 ];
  cubeinfo ci;
  int nScore, nScoreOpponent;
  char *szResponse;
 
  if( ParseFIBSBoard( pec->szFIBSBoard, anBoard, szName, szOpp, &nMatchTo,
                      &nScore, &nScoreOpponent, anDice, &nCube,
                      &fCubeOwner, &fDoubled, &fCrawford ) ) {
    outputl( _("Warning: badly formed board from external controller.") );
    szResponse = 
      g_strdup_printf( "Error: badly formed board ('%s')\n", pec->szFIBSBoard );
  }
  else {
    
    anScore[ 0 ] = nScoreOpponent;
    anScore[ 1 ] = nScore;

	/* If the session isn't using Crawford rule, set crawford flag to false */
	fCrawford = pec->fCrawfordRule ? fCrawford : FALSE;
	/* Set the Jacoby flag appropriately from the external interface settings */
	fJacoby   = pec->fJacobyRule;

	/* printf ("Crawford Setting: %d\n", fCrawford); */
	/* printf ("Jacoby Setting: %d\n", fJacoby); */
    
    fTurn = 1;
    SetCubeInfo ( &ci, nCube, fCubeOwner, fTurn, nMatchTo, anScore,
                  fCrawford, fJacoby, nBeavers, bgvDefault ); 
    
#if 0 
    {
      char *asz[ 7 ] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
      char szBoard[ 10000 ];
      outputl( DrawBoard( szBoard, anBoard, 1, asz,
                          "no matchid", 15 ) );

      printf( "score %d-%d to %d ", anScore[ 0 ], anScore[ 1 ],
              nMatchTo );
      printf( "dice %d %d ", anDice[ 0 ], anDice[ 1 ] );
      printf( "cubeowner %d cube %d turn %d crawford %d doubled %d\n",
              fCubeOwner, nCube, fTurn, fCrawford, fDoubled );
    }
#endif

    memcpy( anBoardOrig, anBoard, sizeof( anBoard ) );

    if ( fDoubled  ) {

      /* take decision */
      if( GeneralCubeDecision( aarOutput, aarStdDev,
                               aarsStatistics, (ConstTanBoard)anBoard, &ci,
                               GetEvalCube(), NULL, NULL ) < 0 )
        return NULL;
	  
      switch( FindCubeDecision( arDouble,  aarOutput, &ci )) {
      case DOUBLE_PASS:
      case TOOGOOD_PASS:
      case REDOUBLE_PASS:
      case TOOGOODRE_PASS:
        szResponse = g_strdup( "drop\n" );
        break;

      case NODOUBLE_BEAVER:
      case DOUBLE_BEAVER:
      case NO_REDOUBLE_BEAVER:
        szResponse = g_strdup( "beaver\n" );
        break;
		    
      default:
        szResponse = g_strdup( "take\n" );
      }

#if 0
      /* this code is broken as the sign of fDoubled 
         indicates who doubled */
    } else if ( fDoubled < 0 ) {
              
      /* if opp wants to resign (extension to FIBS board) */

      float arOutput[ NUM_ROLLOUT_OUTPUTS ];
      float rEqBefore, rEqAfter;
      const float epsilon = 1.0e-6;

      getResignation( arOutput, anBoard, &ci, &esEvalCube );

      getResignEquities ( arOutput, &ci, -fDoubled,
                          &rEqBefore, &rEqAfter );

      /* if opponent gives up equity by resigning */
      if( ( rEqAfter - epsilon ) < rEqBefore )
        szResponse = g_strdup( "accept\n" );
      else
        szResponse = g_strdup( "reject\n" );

#endif /* broken */
    } else if( anDice[ 0 ] ) {
      /* move */
      char szMove[ 64 ];
      if( FindBestMove( anMove, anDice[ 0 ], anDice[ 1 ],
                        anBoard, &ci, &GetEvalChequer()->ec,
                        *GetEvalMoveFilter() ) < 0 )
        return NULL;

      FormatMovePlain( szMove, anBoardOrig, anMove );
      szResponse = g_strconcat( szMove, "\n", NULL );
    } else {
      /* double decision */
      if( GeneralCubeDecision( aarOutput, aarStdDev,
                               aarsStatistics, (ConstTanBoard)anBoard, &ci,
                               GetEvalCube(), NULL, NULL ) < 0 )
        return NULL;
		
      switch( FindCubeDecision( arDouble,  aarOutput, &ci )) {
      case DOUBLE_TAKE:
      case DOUBLE_PASS:
      case DOUBLE_BEAVER:
      case REDOUBLE_TAKE:
      case REDOUBLE_PASS:
        szResponse = g_strdup( "double\n" );
        break;
		    
      default:
        szResponse = g_strdup( "roll\n" );
      }
    }
	    
  }

  return szResponse;

}
#endif

extern void CommandExternal( char *sz ) {

#if !HAVE_SOCKETS
    outputl( _("This installation of GNU Backgammon was compiled without\n"
	     "socket support, and does not implement external controllers.") );
#else
    int h, hPeer, cb;
    struct sockaddr *psa;
    char szCommand[ 256 ];
    char *szResponse = NULL;
    struct sockaddr_in saRemote;
    socklen_t saLen;
    extcmd *pec;
    
    sz = NextToken( &sz );
    
    if( !sz || !*sz ) {
	outputl( _("You must specify the name of the socket to the external controller.") );
	return;
    }

listenloop:
	{

      if( ( h = ExternalSocket( &psa, &cb, sz ) ) < 0 ) {
	SockErr( sz );
	return;
      }
      
      if( bind( h, psa, cb ) < 0 ) {
	SockErr( sz );
	closesocket( h );
	free( psa );
	return;
      }
      
      free( psa );
      
      if( listen( h, 1 ) < 0 ) {
	SockErr( "listen" );
	closesocket( h );
	ExternalUnbind( sz );
	return;
      }
      outputf( _("Waiting for a connection from %s...\n"), sz);
      outputx();
      ProcessEvents();

      /* Must set length when using windows */
      saLen = sizeof(struct sockaddr);
      while( ( hPeer = accept( h, (struct sockaddr*)&saRemote, &saLen ) ) < 0 )
	  {
		if( errno == EINTR )
		{
			ProcessEvents();

          if( fInterrupt ) {
            closesocket( h );
            ExternalUnbind( sz );
            return;
          }
	    
          continue;
	}
	
	SockErr( "accept" );
	closesocket( h );
	ExternalUnbind( sz );
	return;
      }

      closesocket( h );
      ExternalUnbind( sz );

      /* print info about remove client */

      outputf(_("Accepted connection from %s.\n"),
                 inet_ntoa( saRemote.sin_addr ) );
      outputx();
      ProcessEvents();

      while( !ExternalRead( hPeer, szCommand, sizeof( szCommand ) ) ) {

        if ( ( pec = ExtParse( szCommand ) ) == 0 ) {
          /* parse error */
          szResponse = szError;
        }
        else {

          switch ( pec->ct ) {
          case COMMAND_NONE:
            szResponse = g_strdup( "Error: no command given\n" );
            break;
          
          case COMMAND_FIBSBOARD:
          
            szResponse = ExtFIBSBoard( pec );
            break;
          
          case COMMAND_EVALUATION:
          
            szResponse = ExtEvaluation( pec );
            break;
          
          }

        }

        if ( szResponse ) {
          if ( ExternalWrite( hPeer, szResponse, strlen( szResponse ) ) )
            break;

          g_free( szResponse );
        }

      }
      closesocket( hPeer );
    }
	goto listenloop;
#endif
}
