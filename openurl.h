/*
 * openurl.h
 *
 * by Jørn Thyssen <jth@gnubg.org>, 2002
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
 * $Id: openurl.h,v 1.4 2007/07/02 12:43:39 ace Exp $
 */

#ifndef _OPENURL_H_
#define _OPENURL_H_


extern void OpenURL( const char *szURL );
extern char *set_web_browser (const char *sz);
extern const gchar *get_web_browser (void);

#endif /* _OPENURL_H_ */
