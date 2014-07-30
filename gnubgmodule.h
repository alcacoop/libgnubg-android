/*
 * gnubgmodule.h
 *
 * by Joern Thyssen <jth@gnubg.org>, 2003
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
 * $Id: gnubgmodule.h,v 1.27 2013/07/18 02:01:24 mdpetch Exp $
 */

#ifndef GNUBGMODULE_H
#define GNUBGMODULE_H

#if USE_PYTHON

#ifdef WIN32
/* needed for mingw inclusion of Python.h */
#include <stdint.h>
#endif
#include <Python.h>
extern PyObject *PythonGnubgModule(void);

#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#endif                          /* PY_VERSION_CHK */
extern PyObject *PythonGnubgModule();
#endif                          /* USE_PYTHON */

#include "lib/simd.h"

#include <glib.h>

extern void PythonInitialise(char *argv0);
extern void PythonShutdown(void);
extern void PythonRun(const char *sz);
extern int LoadPythonFile(const char *sz, int fQuiet);
extern gint python_run_file(gpointer file);

#endif                          /* GNUBGMODULE_H */
