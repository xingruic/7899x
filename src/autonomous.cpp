#include "autonomous.h"
#include "vex.h"
#include "robotconfig.h"
#include "drive.h"
#include "functions.h"
#include<iostream>

using vex::wait, vex::sec, vex::msec, vex::deg;
using pid::PID45, pid::PID90, pid::PID135, pid::PID180, pid::PID_DRIVE, pid::PID_ARC, pid::PID_SLOWARC,
/****/pid::turn, pid::drive, pid::arcTurn, pid::arcDist;

auton_mode autonMode = SKILLS;

void farSide3BallStart();
void farSideWP();
void farSideElims();

void closeSideWP();
void closeSideElims();

void progSkills();

void auton() {
    Brain.Screen.clearScreen();
    switch (autonMode) {
    case FAR_SIDE:
        // farSideWP();
        farSideElims();
        break;
    case CLOSE_SIDE:
        closeSideWP();
        // closeSideElims();
        break;
    case SKILLS:
        progSkills();
        break;
    default:
        break;
    }
}

void handleBrainAutonSelect() {
    // x^2 + y^2 <= r^2
    int r = 60;
    r *= r;
    int x = Brain.Screen.xPosition(), y = Brain.Screen.yPosition();
    // Brain.Screen.printAt(30, 30, "%d     , %d     ", x, y);
    int X = 80, Y = 90;
    if (pow(x - X, 2) + pow(y - Y, 2) <= r) {
        autonMode = FAR_SIDE;
        Brain.Screen.printAt(30, 30, "Far    ");
        return;
    }
    X = 220;
    if (pow(x - X, 2) + pow(y - Y, 2) <= r) {
        autonMode = CLOSE_SIDE;
        Brain.Screen.printAt(30, 30, "Close    ");
        return;
    }
    X = 360;
    if (pow(x - X, 2) + pow(y - Y, 2) <= r) {
        autonMode = SKILLS;
        Brain.Screen.printAt(30, 30, "Skills    ");
        return;
    }
    autonMode = NONE;
    Brain.Screen.printAt(30, 30, "None    ");
}

void autonSelect() {
    // autonMode = NONE;
    Brain.Screen.printAt(30, 30, "None");
    Brain.Screen.drawCircle(80, 90, 50);
    Brain.Screen.printAt(50, 90, "Far");
    Brain.Screen.drawCircle(220, 90, 50);
    Brain.Screen.printAt(190, 90, "Close");
    Brain.Screen.drawCircle(360, 90, 50);
    Brain.Screen.printAt(330, 90, "Skills");
    Brain.Screen.pressed(handleBrainAutonSelect);
}

/*old closeSideWP
    spinIntk(100);
    driveWait(-200, 0.8, 1.0);
    spinIntk(-100);
    arcWait(70, -20, -70, false);
    turn(90, 500, false);
    drive(100, 500);
    driveWait(-130);
    drive(100, 500);
    arcWait(60, 20, 80, false);
    Pneu1.set(true);
    arcWait(40, 90, 100, false);
    driveWait(200, 0.5, 1.0);
    driveWait(200, 0, 1.0);
    Pneu1.set(false);
    turn(20, 500, false);
    drive(530, 1000);
*/

void closeSideWP() {
    spinIntk(100);
    drive(1120, 1350);
    driveWait(-50, 0.5, 0.5);
    turn(90, 500, false);
    drive(-50, 400);
    spinIntk(-100);
    wait(1000, msec);
    arcWait(60, -70, 0, false);
    spinIntk(100);
    turn(0, 350, false);
    drive(150, 500);
    driveWait(-150, 0.7, 0.7);
    turn(45, 450, false);
    drive(-700, 900);
    driveWait(-10);
    turn(-30, 900, false);
    Pneu2.set(true);
    Pneu1.set(true);
    wait(150, msec);
    driveWait(-50, 0.6, 0.6);
    turn(-90, 750, false);
    Pneu2.set(false);
    turn(-30, 500, false);
    driveWait(-200, 1.0, 0.3);
    turn(-80, 300, false);
    drive(-500, 850);
    Pneu1.set(false);
    turn(90, 1000, false);
    spinIntk(-100);
    drive(100, 2000);
}

void closeSideElims() {
    spinIntk(100);
    drive(-50, 750);
    driveWait(-550);
    Pneu1.set(true);
    arcWait(250, 70, -70, false);
    Pneu1.set(false);
    turn(180, 1000, false);
    spinIntk(-100);
    drive(200, 500);
    arcWait(250, 0, -100, false);
    turn(300, 500, false);
    driveWait(-500);
    arcDist(0.3, 0.7, -300, 1000);
}

void farSide3BallStart() {
    spinIntk(100);
    wait(1000, msec);
    drive(-570, 800);
    Pneu2.set(true);
    arcWait(-30, -70, -5);
    arcWait(-60, -70, -40, false);
    arcWait(-100, -100, 0, false);
    Pneu2.set(false);
    turn(-80, 500, false);
    drivePct(-100, -80);
    wait(300, msec);
    drive(180, 500);
    drivePct(-100, -100);
    wait(800, msec);
}

void farSideWP() {
    Pneu2.set(true);
    spinIntk(100);
    wait(150, msec);
    driveWait(-100, 0.7, 0.7);
    arcWait(-60, -80, 0, false);
    arcWait(-90, -80, 80, false);
    Pneu2.set(false);
    turn(-70, 650, false);
    arcDist(1.0, 0.7, -1000, 750);
    arcWait(-40, 70, 0, false);
    driveWait(250, 0.7, 0.7);
    arcWait(60, 70, 0, false);
    turn(80, 300, false);
    drive(400, 650);
    turn(43, 450, false);
    drive(400, 650);
    arcWait(140, 40, -70, false);
    turn(180, 250, false);
    spinIntk(-100);
    drive(2000, 800);
    driveWait(-100);
    turn(335, 900, false);
    spinIntk(100);
    drive(400, 900);
    arcWait(220, -70, 40, false);
    turn(180, 250, false);
    spinIntk(-100);
    drive(2000, 800);
    driveWait(-100);
    turn(330, 900, false);
    spinIntk(100);
    driveWait(150);
    arcWait(310, 50, 100, false);
    drive(300, 1000);
}

void farSideElims() {
    spinIntk(100);
    Pneu1.set(true);
    wait(200, msec);
    Pneu1.set(false);
    drive(900, 1000);
    turn(90, 500, false);
    spinIntk(-100);
    wait(450, msec);
    turn(-70, 700, false);
    spinIntk(100);
    drive(300, 700);
    driveWait(-50, 0.5, 1.0);
    turn(90, 700, false);
    spinIntk(-100);
    wait(450, msec);
    turn(230, 750, false);
    spinIntk(100);
    drive(300, 700);
    driveWait(-75);
    Pneu1.set(true);
    turn(270, 500, false);
    drive(-2000, 700);
    driveWait(20);
    Pneu1.set(false);
    turn(180, 700, false);
    driveWait(800);
    arcWait(100, -70, 70, false);
    spinIntk(-100);
    turn(80, 500, false);
    turn(-90, 1000, false);
}

void skillsBeginning() {
    spinIntk(100);
    drive(-330, 700);
    spinIntk(-100);
    turn(-23, 800, false);
    drive(-150, 400);
    // launch triballs --
    Pneu1.set(true);
    MotorCat.setBrake(vex::coast);
    MotorIntk.setBrake(vex::coast);
    const int catModif = -8;
    for (int i = 0; i < 24; i++) {
        Brain.Screen.printAt(50, 50, "i: %d   ", i);
        spinCat(100 + catModif);
        spinIntk(-100);
        wait(500, msec);
        spinCat(99 + catModif);
        spinIntk(-99);
        wait(500, msec);
    }
    Pneu1.set(false);
    // launch triballs ^^
    MotorCat.setBrake(vex::brake);
    MotorIntk.setBrake(vex::brake);
    for (int i = 0; i < 50 && (distances.isObjectDetected() && distances.objectDistance(vex::mm) > 50); i++) {
        spinCat(100);
        wait(10, msec);
        spinCat(99);
        wait(10, msec);
    }spinCat(0);
    // inertials.calibrate();
    // while (inertials.isCalibrating()) wait(100, msec);
    turn(60, 600, false);
    arcDist(0.5, 1.0, -1000, 700);
}

void progSkills() {
    // beginning macro
    skillsBeginning();

    arcWait(65, -20, 80, false);
    turn(50, 350, false);
    spinIntk(-100); // just to make sure it spins
    spinCat(0); // just to make sure it stops lol
    driveWait(200);
    arcWait(35, 70, 100, false);
    arcWait(15, 50, 100, false);
    driveWait(150, 1.0, 0.9);
    driveWait(175);
    spinCat(0); // just to make sure it stops lol
    arcDist(0.8, 1.0, 1000, 800);
    turn(-220, 1000, false);

    Pneu1.set(true);
    driveWait(-100);
    driveWait(-180, 1.0, 0.5);
    arcDist(0.7, 0.5, -2000, 1000);
    driveWait(50);
    turn(-250, 250, false);
    arcDist(1.0, 0.7, 200, 650);
    arcDist(1.0, 0.7, -2000, 1000);

    control.Screen.clearScreen();
    control.Screen.setCursor(1, 1);
    control.Screen.print("Angle: %.2f    ", getRotation());

    // inertials.setRotation(, deg);
    spinIntk(0);
    driveWait(50);
    arcWait(-230, 100, -80, false);
    Pneu1.set(false);
    turn(-175, 400, false);

    driveWait(200);
    driveWait(500, 0.8, 1.0);
    arcWait(-190, 0, 100, false);
    turn(-220, 500, false);
    Pneu1.set(true);
    Pneu2.set(true);
    driveWait(-200, 0.5, 0.6);
    spinIntk(-30);
    arcWait(-210, -40, -60, false);
    Pneu1.set(false);
    arcWait(-200, -40, -60, false);
    turn(-180, 300, false);
    drive(200, 500);
    arcDist(0.7, 0.7, -2000, 700);
    drive(100, 250);
    driveWait(30);
    arcWait(-185, 20, -20, false, 500);
    arcWait(-175, -20, 20, false, 500);

    spinIntk(0);
    driveWait(50);
    Pneu2.set(false);
    driveWait(150);
    arcWait(-115, 100, 10, false);
    driveWait(150);
    arcWait(-140, -70, 70, false);
    turn(-160, 600, false);
    Pneu2.set(true);
    wait(100, msec);
    driveWait(-300, 0.6, 0.5);
    driveWait(100, 0.7, 0.7);
    spinIntk(-30);
    arcDist(0.7, 0.7, -2000, 1000);
    driveWait(30);
    arcWait(-180, 20, -20, false, 500);
    arcWait(-170, -20, 20, false, 500);
    driveWait(100);
    Pneu2.set(false);

    spinIntk(0);
    driveWait(100);
    arcWait(-105, 100, 10, false);
    driveWait(150);
    arcWait(-115, -70, 70, false);
    turn(-135, 700, false);
    Pneu1.set(true);
    Pneu2.set(true);
    driveWait(-200, 0.6, 0.5);
    spinIntk(-30);
    arcWait(-160, -60, -40, false);
    turn(-170, 300, false);
    Pneu1.set(false);
    drive(200, 500);
    arcDist(0.7, 0.7, -2000, 900); //
    driveWait(30);
    arcWait(-185, 20, -20, false, 500);
    arcWait(-175, -20, 20, false, 500);

    spinIntk(100);
    driveWait(100);
    Pneu1.set(false);
    Pneu2.set(false);
    driveWait(100);
    arcWait(-115, 100, 10, false);
    driveWait(300);
    turn(0, 800, false);

    spinIntk(-100);
    drive(400, 700);
    turn(-90, 700, false);
    spinIntk(100);
    drive(600, 800);
    spinIntk(-100);
    turn(0, 700, false);
    arcWait(60, 20, -100, false);
    spinIntk(100);
    turn(180, 600, false);
    Pneu2.set(true);
    driveWait(-150, 0.6, 0.7);
    arcWait(200, -20, -60, false);
    driveWait(-200, 0.5, 0.5);
    turn(225, 300, false);
    driveWait(-180, 0.5, 1.0);
    arcDist(0.5, 0.7, -2000, 1000);
    driveWait(50);
    turn(250, 250, false);
    arcDist(0.7, 1.0, 200, 650);
    arcDist(0.7, 1.0, -2000, 1000);
    drive(200, 500);
}