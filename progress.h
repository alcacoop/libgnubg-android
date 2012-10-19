/*
 * progress.h
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
 * $Id: progress.h,v 1.9 2009/09/29 22:09:05 c_anthon Exp $
 */

#ifndef _PROGRESS_H_
#define _PROGRESS_H_

#include "eval.h"
#include "rollout.h"

extern void
RolloutProgressStart( const cubeinfo *pci, const int n,
                      rolloutstat aars[][ 2 ],
                      rolloutcontext *pes, char asz[][ 40 ], gboolean multiple, void **pp );

extern void
RolloutProgress( float aarOutput[][ NUM_ROLLOUT_OUTPUTS ],
                 float aarStdDev[][ NUM_ROLLOUT_OUTPUTS ],
                 const rolloutcontext *prc,
                 const cubeinfo aci[],
                 unsigned int initial_game_count,
                 const int iGame,
                 const int iAlternative,
				 const int nRank,
				 const float rJsd,
				 const int fStopped,
				 const int fShowRanks,
				 int fCubeRollout,
                 void *pUserData );

extern int
RolloutProgressEnd( void **pp, gboolean destroy );

#endif /* _PROGRESS_H_ */

