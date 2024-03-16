#pragma once

enum auton_mode {
    FAR_SIDE,
    CLOSE_SIDE,
    SKILLS,
    NONE,
};

extern auton_mode autonMode;

void skillsBeginning();

void auton();

void autonSelect();