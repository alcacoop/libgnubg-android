/*
 * gtksplash.h
 *
 * by Joern Thyssen <jth@gnubg.org>, 2002
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
 * $Id: gtksplash.h,v 1.5 2009/03/02 23:01:30 Superfly_Jon Exp $
 */

#ifndef _GTKSPLASH_H_
#define _GTKSPLASH_H_

extern GtkWidget *
CreateSplash ( void );

extern void
DestroySplash ( GtkWidget *pwSplash );

extern void
PushSplash ( GtkWidget *pwSplash, const gchar *szText0, const gchar *szText1 );

#endif /* _GTKSPLASH_H_ */
