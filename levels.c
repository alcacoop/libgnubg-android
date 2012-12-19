 /*
 ##################################################################
 #                     GNU BACKGAMMON MOBILE                      #
 ##################################################################
 #                                                                #
 #  Authors: Domenico Martella - Davide Saurino                   #
 #  E-mail: info@alcacoop.it                                      #
 #  Date:   19/12/2012                                            #
 #                                                                #
 ##################################################################
 #                                                                #
 #  Copyright (C) 2012   Alca Societa' Cooperativa                #
 #                                                                #
 #  This file is part of GNU BACKGAMMON MOBILE.                   #
 #  GNU BACKGAMMON MOBILE is free software: you can redistribute  # 
 #  it and/or modify it under the terms of the GNU General        #
 #  Public License as published by the Free Software Foundation,  #
 #  either version 3 of the License, or (at your option)          #
 #  any later version.                                            #
 #                                                                #
 #  GNU BACKGAMMON MOBILE is distributed in the hope that it      #
 #  will be useful, but WITHOUT ANY WARRANTY; without even the    #
 #  implied warranty of MERCHANTABILITY or FITNESS FOR A          #
 #  PARTICULAR PURPOSE.  See the GNU General Public License       #
 #  for more details.                                             #
 #                                                                #
 #  You should have received a copy of the GNU General            #
 #  Public License v3 along with this program.                    #
 #  If not, see <http://http://www.gnu.org/licenses/>             #
 #                                                                #
 ##################################################################
*/


#include "levels.h"
/*
 * LEVELS: 
 *   BEGINNER, CASUAL, INTERMEDIATE, ADVANCED, EXPERT, WORLDCLASS, SUPREMO, GRANDMASTER
 */


/*
MOVEFILTERS:

Superficiale:
  {{{0, 5, 0.08f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   {{0, 5, 0.08f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
   {{0, 5, 0.08f }, {-1, 0, 0}, {0, 2, 0.02f }, {0, 0, 0}},
   {{0, 5, 0.08f }, {-1, 0, 0}, {0, 2, 0.02f }, {-1, 0, 0}}}

Poco Approfondito:
  {{{0, 8, 0.12f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   {{0, 8, 0.12f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
   {{0, 8, 0.12f }, {-1, 0, 0}, {0, 2, 0.03f }, {0, 0, 0}},
   {{0, 8, 0.12f }, {-1, 0, 0}, {0, 2, 0.03f }, {-1, 0, 0}}}

Normale:
  {{{0, 8, 0.16f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   {{0, 8, 0.16f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
   {{0, 8, 0.16f }, {-1, 0, 0}, {0, 2, 0.04f }, {0, 0, 0}},
   {{0, 8, 0.16f }, {-1, 0, 0}, {0, 2, 0.04f }, {-1, 0, 0}}}

Largo:
  {{{0, 16, 0.32f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   {{0, 16, 0.32f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
   {{0, 16, 0.32f }, {-1, 0, 0}, {0, 4, 0.08f }, {0, 0, 0}},
   {{0, 16, 0.32f }, {-1, 0, 0}, {0, 4, 0.08f }, {-1, 0, 0}}}

Approfondito:
  {{{0, 20, 0.44f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   {{0, 20, 0.44f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
   {{0, 20, 0.44f }, {-1, 0, 0}, {0, 6, 0.11f }, {0, 0, 0}},
   {{0, 20, 0.44f }, {-1, 0, 0}, {0, 6, 0.11f }, {-1, 0, 0}}}
*/

level levels[] = {
  //BEGINNER
  {
    //{1, 0, 0, 1, 0.060000}, NULL
    {1, 0, 0, 0, 0.080000}, NULL
  },

  //CASUAL
  {
    //{1, 0, 0, 1, 0.050000}, NULL
    {1, 0, 0, 0, 0.060000}, NULL
  },

  //INTERMEDIATE
  {
    //{1, 0, 0, 1, 0.040000}, NULL
    {1, 0, 0, 0, 0.050000}, NULL
  },

  //ADVANCED
  {
    {1, 0, 0, 1, 0.015000}, NULL
  },

  //EXPERT
  {
    {1, 0, 0, 1, 0.000000}, NULL
  },

  //WORLDCLASS
  {
    {1, 2, 1, 1, 0.000000},
    {{{0, 2, 0.04f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
     {{0, 2, 0.04f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
     {{-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}, {0, 0, 0}},
     {{-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}, {0, 0, 0}}},
    /*{{{0, 5, 0.08f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
     {{0, 5, 0.08f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
     {{0, 5, 0.08f }, {-1, 0, 0}, {0, 2, 0.02f }, {0, 0, 0}},
     {{0, 5, 0.08f }, {-1, 0, 0}, {0, 2, 0.02f }, {-1, 0, 0}}}*/
  },

  //SUPREMO
  {
    {1,2,1,1,0.000000},
    {{{0, 6, 0.10f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
     {{0, 6, 0.10f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
     {{0, 6, 0.10f }, {-1, 0, 0}, {0, 2, 0.02f }, {0, 0, 0}},
     {{0, 6, 0.10f }, {-1, 0, 0}, {0, 2, 0.02f }, {-1, 0, 0}}}
  },

  //GRANDMASTER
  {
    {1,3,1,1,0.000000},
    {{{0, 3, 0.04f }, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
     {{0, 3, 0.04f }, {-1, 0, 0}, {0, 0, 0 }, {0, 0, 0}},
     {{0, 3, 0.04f }, {-1, 0, 0}, {0, 2, 0.01f }, {0, 0, 0}},
     {{0, 3, 0.04f }, {-1, 0, 0}, {0, 2, 0.01f }, {-1, 0, 0}}}
  }
};
