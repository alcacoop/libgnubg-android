/*
 * gtkrelational.h
 *
 * by Christian Anthon <anthon@kiku.dk>, 2006.
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
 * $Id: gtkrelational.h,v 1.10 2008/02/17 12:26:24 c_anthon Exp $
 */

#include "relational.h"

#include <gtk/gtk.h>

#ifndef _GTKRELATIONAL_H_
#define _GTKRELATIONAL_H_
extern void GtkRelationalAddMatch(gpointer p, guint n, GtkWidget * pw);
extern void GtkShowRelational(gpointer p, guint n, GtkWidget * pw);
extern GtkWidget *RelationalOptions(void);
extern void RelationalOptionsShown(void);
extern void RelationalSaveOptions(void);
extern void GtkShowQuery(RowSet* pRow);
#endif
