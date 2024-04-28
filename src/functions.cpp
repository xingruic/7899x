#include "functions.h"
#include "drive.h"
#include "robotconfig.h"
#include "vex.h"
#include<iostream>

int lDriveModifier = 0, rDriveModifier = 0;
void setLRDriveModif(int x) {
    lDriveModifier = rDriveModifier = x;
}

void spinIntk(int speed) {
    MotorIntk.spin(vex::forward, speed, vex::percent);
}

void spinCat(int speed) {
    MotorCat.spin(vex::forward, speed, vex::percent);
    // if (speed != 0) setLRDriveModif(-5);
    // else setLRDriveModif(0);
}

void toggleController::bindButton(const vex::controller::button *button, void(*func)()) {
    if (isBinded(button)) {
        std::cerr << "button already binded. exiting bindButton()" << std::endl;
        return;
    }
    bindings[button] = func;
}

void toggleController::unbindButton(const vex::controller::button *button) {
    if (!isBinded(button)) {
        std::cerr << "button not binded. exiting unbindButton()" << std::endl;
        return;
    }
    bindings.erase(button);
}

void toggleController::updatePressing() {
    for (auto i = pressing.begin(); i != pressing.end(); i++) {
        i->second = i->first->pressing();
    }
}

bool toggleController::justPressed(const vex::controller::button *button) {
    return !pressing[button] && button->pressing();
}

void toggleController::runAllPressed() {
    for (auto i = bindings.begin();i != bindings.end();i++) {
        if (justPressed(i->first)) i->second();
    }
}

bool toggleController::isBinded(const vex::controller::button *button) {
    return bindings.count(button) > 0;
}

toggleController TCONTROLLER = toggleController();