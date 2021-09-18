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

#ifndef ROTATOR_H
#define ROTATOR_H

#include <AccelStepper.h>

class RotatorAxis {
   public:
    RotatorAxis(AccelStepper::MotorInterfaceType motorInterfaceType,
    uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    void doLoop(void);

    void setTarget(float pos);     // set target position in degrees
    float getPosition(void);       // get current position in degrees
    void home(void);               // move to endstop and set position
    void stop(void);               // immediately stop moving

    float stepsPerRev;      // steps per revolution
    float speedMax;         // max speed in steps per second
    float accelMax;         // acceleration in steps per second per second
    float posMin;           // limit of travel in CCW direction in degrees
    float posMax;           // limit of travel in CC direction in degrees

   private:
    AccelStepper stepper;
    bool homing = false;
    bool homed = false;
};

class Rotator {
   public:
    Rotator();
    void doLoop(void);

    RotatorAxis azAxis;     // azimuth axis
    RotatorAxis elAxis;     // elevation axis
};

#endif
