/*
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
 * list.h
 *
 * by Gary Wong, 1996
 * $Id: list.h,v 1.7 2007/12/12 23:08:22 Superfly_Jon Exp $
 */

#ifndef _LIST_H_
#define _LIST_H_

typedef struct _list {
  struct _list* plPrev;
  struct _list* plNext;
  void* p;
} listOLD;
/* Renamed to listOLD - use GList instead (hopefullly replace existing usage eventually */

extern int ListCreate( listOLD *pl );
/* #define ListDestroy( pl ) ( assert( ListEmpty( pl ) ) ) */

#define ListEmpty( pl ) ( (pl)->plNext == (pl) )
extern listOLD* ListInsert( listOLD* pl, void* p );
extern void ListDelete( listOLD* pl );
extern void ListDeleteAll( const listOLD *pl );

#endif
