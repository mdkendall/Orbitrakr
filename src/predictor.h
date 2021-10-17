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

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>
#include <Arduino.h>

#include "SGP4.h"

class Predictor {
  public:
    Predictor(uint32_t catalogNumber);
    void init(void);
    void propagate(time_t t);
    void position(double& latgc, double& latgd, double& lon, double& hellp);
    void look(double slatgd, double slon, double& rho, double& az, double& el);

  private:
    // Satelite
    uint32_t catalogNumber;
    String satName;
    String tle[2];      // NORAD two-line element set
    elsetrec satrec;    // SGP4 satellite record
    time_t epoch;       // TLE epoch as UNIX timestamp
    // Prediction
    double tsince;      // time since epoch in minutes
    double recef[3];    // satelite position in ECEF reference frame
    double vecef[3];    // satelite velocity in ECEF reference frame
    double aecef[3];    // satelite acceleration in ECEF reference frame
};

#endif
