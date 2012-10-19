/*
 * external.h
 *
 * by Gary Wong <gtw@gnu.org>, 2001.
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
 * $Id: external.h,v 1.14 2012/04/26 01:15:20 mdpetch Exp $
 */

#ifndef _EXTERNAL_H_
#define _EXTERNAL_H_

#if HAVE_SOCKETS

#ifndef WIN32

#define closesocket close

#if HAVE_SYS_SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#endif /* #if HAVE_SYS_SOCKET_H */

#else /* #ifndef WIN32 */
#include <winsock2.h>
#endif /* #ifndef WIN32 */

typedef enum _cmdtype  {
  COMMAND_NONE = 0,
  COMMAND_FIBSBOARD = 1,
  COMMAND_EVALUATION = 2
} cmdtype;


typedef struct _extcmd {
  cmdtype ct;
  
  /* evalcontext */
  int nPlies;
  float rNoise;
  int fDeterministic;
  int fCubeful;
  int nReduced;
  int fUsePrune;

  /* session rules */
  int fCrawfordRule;
  int fJacobyRule;

  /* fibs board */
  char *szFIBSBoard;
} extcmd;

extern int ExternalSocket( struct sockaddr **ppsa, int *pcb, char *sz );
extern int ExternalRead( int h, char *pch, size_t cch );
extern int ExternalWrite( int h, char *pch, size_t cch );
#ifdef WIN32
extern void OutputWin32SocketError(const char* action);
#define SockErr OutputWin32SocketError
#else
#define SockErr outputerr
#endif

/* Parser functions */

extern void ( *ExtErrorHandler )( const char *szMessage, const char *szNear, 
                                  const int fParseError );

#endif /* #if HAVE_SOCKETS */

#endif /* #ifndef _EXTERNAL_H_ */
