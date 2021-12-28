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

RotatorAxis::RotatorAxis(AccelStepper::MotorInterfaceType motorInterfaceType, uint8_t pins[]) :
    stepper(motorInterfaceType, pins[0], pins[1], pins[2], pins[3]) {
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
    webUI(webUI) {

    reconfigure();
    WebUIItemGroup &itemGroup = webUI.addItemGroup("rotator", "Rotator");
    itemAz = &itemGroup.addItem("az", "Azimuth", "&deg;");
    itemEl = &itemGroup.addItem("el", "Elevation", "&deg;");

    xTaskCreatePinnedToCore(task, "Rotator", 2048, this, 3, &taskHandle, 1);
}

void Rotator::reconfigure(void) {

    if (azAxis) delete(azAxis);
    if (elAxis) delete(elAxis);

    uint8_t pins[4] = {0};
    azAxis = new RotatorAxis((AccelStepper::MotorInterfaceType)webUI.getAzMotorType(), webUI.getAzMotorPins(pins));
    Serial.printf("AzAxis: %d [%d %d %d %d]\n", webUI.getAzMotorType(), pins[0], pins[1], pins[2], pins[3]);
    elAxis = new RotatorAxis((AccelStepper::MotorInterfaceType)webUI.getAzMotorType(), webUI.getElMotorPins(pins));
    Serial.printf("ElAxis: %d [%d %d %d %d]\n", webUI.getElMotorType(), pins[0], pins[1], pins[2], pins[3]);

    azAxis->stepsPerRev = webUI.getAzStepsPerRev(); elAxis->stepsPerRev = webUI.getElStepsPerRev();
    azAxis->speedMax = webUI.getAzSpeedMax(); elAxis->speedMax = webUI.getElSpeedMax();
    azAxis->accelMax = webUI.getAzAccelMax(); elAxis->accelMax = webUI.getElAccelMax();
    azAxis->posMin = -180.0; azAxis->posMax = 180.0;
    elAxis->posMin = 0.0; elAxis->posMax = 90.0;
}

void Rotator::task(void *param) {
    Rotator *rotator = (Rotator*)param;
    while (true) {
        rotator->azAxis->doLoop();
        rotator->elAxis->doLoop();
        rotator->itemAz->setValue(rotator->azAxis->getPosition());
        rotator->itemEl->setValue(rotator->elAxis->getPosition());
        vTaskDelay(1);
    }
}
