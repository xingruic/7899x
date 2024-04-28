#pragma once
#include <map>
#include "vex.h"

extern int lDriveModifier;
extern int rDriveModifier;

void setLRDriveModif(int x);

void spinIntk(int speed);

void spinCat(int speed);

class toggleController {
public:
    void bindButton(const vex::controller::button *button, void(*func)());
    void unbindButton(const vex::controller::button *button);
    void updatePressing();
    bool justPressed(const vex::controller::button *button);
    void runAllPressed();
    bool isBinded(const vex::controller::button *button);
private:
    std::map<const vex::controller::button *, void(*)()> bindings;
    std::map<const vex::controller::button *, bool> pressing;
}extern TCONTROLLER;