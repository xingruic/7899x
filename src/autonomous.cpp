#include "autonomous.h"
#include "vex.h"
#include "robotconfig.h"
#include "drive.h"
#include "functions.h"
#include "dashboard.h"
#include<iostream>

using vex::wait, vex::sec, vex::msec, vex::deg;
using pid::PID20, pid::PID60, pid::PID90, pid::PID135, pid::PID180, pid::PID_DRIVE, pid::PID_ARC, pid::PID_SLOWARC,
/****/pid::turn, pid::drive, pid::arcTurn, pid::arcDist;
using namespace autonmode;

void farSide100();
void farSide50();
void farSide0();

void closeSide100();
void closeSide50();
void closeSide0();

void progSkills();

auton_mode autonmode::mode = FAR_SIDE;
int autonmode::AWP = 0;
char *autonmode::mode_strings[3] = { "Far Side","Close Side","Skills" };

vex::timer driverTime = vex::timer();

void autonmode::updateControlScreen() {
    control.Screen.clearScreen();
    control.Screen.setCursor(1, 1);
    control.Screen.print("%s", mode_strings[mode]);
    if (mode < 2) {
        control.Screen.setCursor(3, 1);
        control.Screen.print("AWP: %d%s", AWP, "%");
    }
}

void autonmode::decrMode() {
    if (mode != SKILLS && AWP > 0) AWP -= 50;
    else {
        mode = (auton_mode)((mode - 1 + 3) % 3);
        if (mode != SKILLS) AWP = 100;
    }
    driverTime.clear();
    updateControlScreen();
}

void autonmode::incrMode() {
    if (mode != SKILLS && AWP < 100) AWP += 50;
    else {
        mode = (auton_mode)((mode + 1) % 3);
        if (mode != SKILLS) AWP = 0;
    }
    driverTime.clear();
    updateControlScreen();
}

void autonmode::toggleAWP() {
    AWP = !AWP;
    updateControlScreen();
}

void auton() {
    switch (mode) {
    case FAR_SIDE:
        if (AWP == 100) farSide100();
        if (AWP == 50) farSide50();
        if (AWP == 0) farSide0();
        break;
    case CLOSE_SIDE:
        if (AWP == 100) closeSide100();
        if (AWP == 50) closeSide50();
        if (AWP == 0) closeSide0();
        break;
    case SKILLS:
        progSkills();
        break;
    default:
        break;
    }
}

void closeSide100() {
    spinIntk(100);
    PneuLB.set(true);
    PneuRB.set(true);
    wait(250, msec);
    turn(-180, 750);
    PneuRB.set(false);
    turn(-340, 1000);
    PneuLB.set(false);
    spinIntk(100);
    drive(500, 900);
    turn(-360, 500);
    spinIntk(-100);
    drive(300, 500);
}

void closeSide50() {
    spinIntk(100);
    drive(900, 1200);
    driveWait(-10, 0.7, 0.7);
    arcWait(-40, 100, -100);
    turn(0, 800);
    PneuLF.set(true);
    spinIntk(-100);
    drive(450, 1000);
    PneuLF.set(false);
    arcWait(-70, -70, 0);
    driveWait(-300);
    arcWait(-140, -100, 40);
    turn(-185, 700);
    driveWait(700);
    arcWait(-250, 70, -20);
    turn(-290, 1000);
    driveWait(200, 0.5, 0.7);
    PneuRB.set(true);
    driveWait(200, 0.7, 0.8);
    arcWait(-380, -50, 100);
    turn(-340, 500);
    PneuRB.set(false);
    spinIntk(-100);
    arcDist(0.8, 1.0, 400, 800);
    turn(-360, 500);
    drive(250, 1000);
}

void closeSide0() {
    spinIntk(100);
    drive(900, 1200);
    driveWait(-10, 0.7, 0.7);
    turn(0, 800);
    PneuLF.set(true);
    spinIntk(-100);
    drive(450, 1000);
    PneuLF.set(false);
    arcWait(-70, -70, 0);
    driveWait(-270);
    arcWait(-140, -100, 20);
    turn(-185, 700);
    driveWait(700);
    arcWait(-250, 70, -20);
    turn(-290, 1000);
    driveWait(200, 0.5, 0.7);
    PneuRB.set(true);
    driveWait(200, 0.7, 0.8);
    arcWait(-380, -50, 100);
    turn(-340, 500);
    PneuRB.set(false);
    spinIntk(-100);
    arcDist(0.8, 1.0, 400, 800);
    turn(-360, 500);
    driveWait(250);
    drive(-500, 1000);
}

void farSide100() { // set up such that the intake will take the triball under the pole
    spinIntk(100);
    wait(800, msec);
    driveWait(-300);
    driveWait(-200, 1.0, 0.7);
    PneuLB.set(true);
    driveWait(-200, 1.0, 0.5);
    driveWait(-100);
    arcWait(-80, -80, 40);
    PneuLB.set(false);
    turn(-220, 650);
    PneuLF.set(true);
    driveWait(250, 0.5, 1.0);
    arcDist(0.8, 1.0, 1000, 700);
    spinIntk(-100);
    driveWait(-200, 0.9, 1.0);
    turn(-250, 500);
    arcDist(0.8, 1.0, 1000, 700);
    PneuLF.set(false);
    driveWait(-200, 0.8, 1.0);
    turn(-345, 800);
    spinIntk(100);
    drive(900, 1200);
    turn(-290, 500);
    driveWait(320, 0.5, 0.5);
    arcWait(-185, 50, -10);
    spinIntk(-100);
    PneuLF.set(true);
    PneuRF.set(true);
    arcDist(0.6, 0.6, 1000, 1000);
    driveWait(-200);
    drive(1000, 800);
    PneuLF.set(false);
    PneuRF.set(false);
    driveWait(-20);
    turn(-90, 700);
    drive(100, 500);
    turn(-40, 500);
    drivePct(100, 100);
    wait(1.5, sec);
}

void farSide50() { // set up such that the intake will take the triball under the pole
    spinIntk(100);
    wait(800, msec);
    driveWait(-300);
    driveWait(-200, 1.0, 0.7);
    PneuLB.set(true);
    driveWait(-200, 1.0, 0.5);
    driveWait(-100);
    arcWait(-80, -80, 40);
    PneuLB.set(false);
    turn(-215, 650);
    PneuLF.set(true);
    driveWait(250, 0.5, 1.0);
    arcDist(0.8, 1.0, 1000, 700);
    spinIntk(-100);
    driveWait(-200, 0.9, 1.0);
    turn(-250, 500);
    arcDist(0.8, 1.0, 1000, 700);
    PneuLF.set(false);
    driveWait(-200, 0.8, 1.0);
    turn(-350, 800);
    spinIntk(100);
    drive(900, 1200);
    turn(-290, 500);
    driveWait(320, 0.5, 0.5);
    arcWait(-185, 50, -10);
    spinIntk(-100);
    PneuLF.set(true);
    PneuRF.set(true);
    arcDist(0.6, 0.6, 1000, 1000);
    driveWait(-200);
    drive(1000, 800);
    PneuLF.set(false);
    PneuRF.set(false);
    drive(-200, 1000);
}

void farSide0() { // right front wheel on cross, with intake facing the center barrier triball (+55 degrees from reset)
    spinIntk(100);
    drive(1200, 1400);
    arcWait(130, 100, -100);
    turn(180, 200);
    PneuLF.set(true);
    PneuRF.set(true);
    spinIntk(-100);
    drive(1000, 800);
    PneuLF.set(false);
    PneuRF.set(false);
    driveWait(-20);
    arcWait(270, 100, -100);
    turn(327, 500);
    spinIntk(100);
    drive(525, 750);
    turn(215, 700);
    spinIntk(-100);
    driveWait(550);
    arcWait(190, -100, 100);
    turn(330, 1000);
    driveWait(100);
    arcWait(350, 80, 40);
    spinIntk(100);
    driveWait(190);
    turn(360, 500);
    driveWait(-300);
    driveWait(-200, 1.0, 0.7);
    PneuLB.set(true);
    driveWait(-200, 1.0, 0.5);
    driveWait(-50);
    arcWait(280, -80, 40);
    PneuLB.set(false);
    turn(145, 650);
    PneuLF.set(true);
    driveWait(250, 0.5, 1.0);
    arcDist(0.8, 1.0, 1000, 700);
    spinIntk(-100);
    driveWait(-200, 0.9, 1.0);
    turn(110, 500);
    arcDist(0.8, 1.0, 1000, 700);
    PneuLF.set(false);
    driveWait(-200, 0.8, 1.0);
    turn(10, 900);
}

void skillsBeginning() {
    spinIntk(100);
    drive(-300, 600);
    spinIntk(-100);
    const int align = -24;
    driveWait(20);
    arcWait(0, -50, 50, false);
    driveWait(-20);
    turn(align, 700, false);
    // launch triballs --
    PneuRB.set(true);
    MotorIntk.setStopping(vex::coast);
    setStopping(vex::hold);
    drivePct(0, 0);
    MotorCat.setStopping(vex::coast);
    const int delay = 50;
    int spd = 80; // spd in pct
    MotorCat.resetPosition();
    double prevPos = 0;
    for (int i = 0; i < 22; i++) {
        for (int ms = 0; ms < 1000; ms += delay) {
            wait(delay, msec);
            spinCat(spd - 1);
            spinCat(spd);
        }
    }
    PneuRB.set(false);
    // launch triballs ^^
    MotorCat.setStopping(vex::brake);
    MotorIntk.setStopping(vex::brake);
    setStopping(vex::brake);
    spinCat(100);
    wait(200, msec);
    while (MotorCat.current(vex::amp) > 1) wait(10, msec);
    spinCat(0);
    // inertials.calibrate();
    // while (inertials.isCalibrating()) wait(100, msec);
    turn(60, 600, false);
    arcDist(0.5, 1.0, -1000, 700);
}

void progSkills() {
    // skillsBeginning();
    driveWait(100, 0.3, 1.0);
    turn(-20, 750);
    spinIntk(100);
    driveWait(500);
    PneuLF.set(true);
    PneuRF.set(true);
    arcWait(-40, 0, 70);
    arcWait(-60, 60, 70);
    driveWait(1000);
    arcWait(-130, -50, 100);
    turn(-170, 500);
    PneuLF.set(false);
    driveWait(300);
    arcWait(-40, 100, 20);
    PneuRF.set(false);
    spinIntk(-100);
    arcWait(-20, 100, 90);
    driveWait(800);
    driveWait(400, 1.0, 0.9);
    PneuRF.set(true);
    arcWait(30, 100, 20);
    driveWait(200);
    arcWait(60, 100, 50);
    driveWait(200, 1.0, 0.8);
    arcDist(1.0, 0.9, 1000, 500);
    driveWait(-200, 1.0, 0.7);
    turn(70, 500);
    arcDist(1.0, 0.7, 1000, 1000);
    PneuRF.set(false);
    driveWait(-300, 1.0, 0.3);
    turn(160, 800);
    spinIntk(100);
    driveWait(700);
    arcWait(90, -50, 100);
    PneuLF.set(true);
    PneuRF.set(true);
    inertials.setRotation(inertials.rotation(deg) - 20, deg);
    arcWait(10, 0, 60);
    arcDist(0.8, 0.8, 1000, 1000);
    PneuRF.set(false);
    driveWait(-30);
    turn(0, 500);
    driveWait(-250, 0.7, 1.0);
    arcWait(40, 70, -70);
    driveWait(100, 0.6, 0.6);
    arcWait(10, 0, 60);
    PneuRF.set(true);
    arcDist(0.8, 0.8, 1000, 1000);
    PneuRF.set(false);
    driveWait(-30);
    turn(0, 500);
    driveWait(-250, 0.7, 1.0);
    arcWait(40, 70, -70);
    driveWait(250);
    arcWait(-5, 0, 60);
    PneuRF.set(true);
    arcDist(0.9, 0.8, 1000, 1200);
    PneuRF.set(false);
    driveWait(-30);
    turn(0, 500);
    driveWait(-100, 0.7, 1.0);
    arcWait(40, 70, -70);
    turn(90, 500);
}