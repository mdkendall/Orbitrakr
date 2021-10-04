/***********************************************************************

  Copyright (C) 2021 Matthew Kendall

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  ***********************************************************************/

#include "predictor.h"
#include <string.h>

#include <Arduino.h>

Predictor::Predictor(uint32_t catalogNumber) {

    // Initial testing FIXME
    strncpy(tle[0], "1 46494U 20068J   21264.59740918 -.00000154  00000-0 -66228-5 0  9992", 70);
    strncpy(tle[1], "2 46494  97.6971 201.6260 0019667 111.1537 249.1792 15.03720340 53785", 70);

    // convert the string format TLE to SGP4 elements and initialise satellite record
    SGP4Funcs::twoline2rv(tle[0], tle[1], 'a', wgs72, satrec);
}
