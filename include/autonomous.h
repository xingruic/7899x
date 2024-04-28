#pragma once
#include "vex.h"

namespace autonmode {
    enum auton_mode {
        FAR_SIDE,
        CLOSE_SIDE,
        SKILLS
    };
    extern char *mode_strings[3];
    extern auton_mode mode;
    extern int AWP; // true for WP, otherwise Elims
    void updateControlScreen();
    void decrMode();
    void incrMode();
    void toggleAWP();
}

extern vex::timer driverTime;

void skillsBeginning();

void auton();