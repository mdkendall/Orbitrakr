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

#include "webui.h"

class RotatorAxis {
  public:
    RotatorAxis(AccelStepper::MotorInterfaceType motorInterfaceType, uint8_t motorPins[], uint8_t endstopPin);
    void doLoop(void);

    void setTarget(float pos);     // set target position in degrees
    float getPosition(void);       // get current position in degrees
    bool getEndstop(void);         // is the endstop sensor active?
    void home(void);               // move to endstop and set position
    void stop(void);               // immediately stop moving

    float stepsPerRev;      // steps per revolution
    float speedMax;         // max speed in steps per second
    float accelMax;         // acceleration in steps per second per second
    float posMin;           // limit of travel in CCW direction in degrees
    float posMax;           // limit of travel in CC direction in degrees

  private:
    void doHoming(void);
    enum HomingState { HS_INIT, HS_SEEK, HS_SEEKSTOP, HS_BACKOFF, HS_CREEP, HS_CREEPSTOP, HS_IDLE };
    AccelStepper stepper;
    uint8_t endstopPin;
    bool homed = false;
    HomingState homingState = HS_IDLE;
};

class Rotator {
  public:
    Rotator(WebUI &webUI);
    void reconfigure(void);

    WebUI &webUI;
    RotatorAxis *azAxis = nullptr;  // azimuth axis
    RotatorAxis *elAxis = nullptr;  // elevation axis

  private:
    static void task(void *param);

    TaskHandle_t taskHandle;
    WebUIItem *itemAz;
    WebUIItem *itemEl;
};

#endif
