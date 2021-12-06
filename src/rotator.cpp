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
        stepper.enableOutputs();
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

Rotator::Rotator(WebUI &webUI) :
    webUI(webUI),
    azAxis(AccelStepper::HALF4WIRE, 13, 14, 12, 27),
    elAxis(AccelStepper::HALF4WIRE, 32, 25, 26, 33) {

    azAxis.stepsPerRev = webUI.getAzStepsPerRev(); elAxis.stepsPerRev = webUI.getElStepsPerRev();
    azAxis.speedMax = webUI.getAzSpeedMax(); elAxis.speedMax = webUI.getElSpeedMax();
    azAxis.accelMax = webUI.getAzAccelMax(); elAxis.accelMax = webUI.getElAccelMax();
    azAxis.posMin = -180.0; azAxis.posMax = 180.0;
    elAxis.posMin = 0.0; elAxis.posMax = 90.0;

    WebUIItemGroup &itemGroup = webUI.addItemGroup("rotator", "Rotator");
    itemAz = &itemGroup.addItem("az", "Azimuth", "&deg;");
    itemEl = &itemGroup.addItem("el", "Elevation", "&deg;");

    xTaskCreatePinnedToCore(task, "Rotator", 2048, this, 3, &taskHandle, 1);
}

void Rotator::task(void *param) {
    Rotator *rotator = (Rotator*)param;
    while (true) {
        rotator->azAxis.doLoop();
        rotator->elAxis.doLoop();
        rotator->itemAz->setValue(rotator->azAxis.getPosition());
        rotator->itemEl->setValue(rotator->elAxis.getPosition());
        vTaskDelay(1);
    }
}
