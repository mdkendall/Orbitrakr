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

#include "rotator.h"

/* --- Rotator Axis --- */

RotatorAxis::RotatorAxis(AccelStepper::MotorInterfaceType motorInterfaceType,
    uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) :
    stepper(motorInterfaceType, pin1, pin2, pin3, pin4) {
}

void RotatorAxis::doLoop(void) {
    stepper.run();
}

void RotatorAxis::setTarget(float pos) {
    if (homed) {
        pos = max(min(pos, posMax), posMin);
        stepper.setMaxSpeed(speedMax);
        stepper.setAcceleration(accelMax);
        stepper.moveTo(stepsPerRev * pos / 360.);
    }
};

float RotatorAxis::getPosition(void) {
    return (stepper.currentPosition() / stepsPerRev) * 360.;
}

void RotatorAxis::home(void) {
    homed = true;                 // FIXME
};

void RotatorAxis::stop(void) {
    stepper.stop();
}

/* --- Rotator --- */

Rotator::Rotator(void) :
    azAxis(AccelStepper::FULL4WIRE, 13, 12, 14, 27),
    elAxis(AccelStepper::FULL4WIRE, 26, 25, 33, 32) {

    azAxis.stepsPerRev = 2037.8864; elAxis.stepsPerRev = 2037.8864;
    azAxis.speedMax = 200; elAxis.speedMax = 200;
    azAxis.accelMax = 50; elAxis.accelMax = 50;
    azAxis.posMin = -180.0; azAxis.posMax = 180.0;
    elAxis.posMin = 0.0; elAxis.posMax = 90.0;
}

void Rotator::doLoop(void) {
    azAxis.doLoop();
    elAxis.doLoop();
}
