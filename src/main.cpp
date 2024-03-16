#include "dashboard.h"
#include "robotconfig.h"
#include "drive.h"
#include "autonomous.h"
#include "functions.h"
#include "vex.h"

#include<iostream>

using namespace vex;

using std::cout, std::endl;

competition Competition;

void pre_auton(void) {
  thread autonSelectThread(autonSelect);
  setStopping(brake);
  MotorCat.setStopping(brake);
  MotorIntk.setStopping(brake);
  inertials.calibrate();
  while (inertials.isCalibrating()) wait(100, msec);
  inertials.resetRotation();
  Brain.Screen.drawRectangle(450, 10, 20, 20, green);
  thread printAngle([]() {
    while (true) {
      wait(200, msec);
      if (!Competition.isEnabled())
        Brain.Screen.printAt(20, 200, "Angle from program start: %.2f   ", inertials.rotation());
    }
    });
}

void toggle(digital_out &dev) { dev.set(!dev.value()); }

void toggleLeft() { toggle(Pneu2); }

void toggleRight() { toggle(Pneu1); }

void toggleWings() {
  toggleLeft();
  toggleRight();
}

int intkSpeed = 0;
int cataModifier = -10;
bool cata = 0;

void incrCataModifier() {
  cataModifier += 5;
  cataModifier = fmin(cataModifier, 0);
}

void decrCataModifier() {
  cataModifier -= 10;
  cataModifier = fmax(cataModifier, -60);
}

void toggleCat() {
  cata = !cata;
}

void usercontrol(void);

void compSwitchKinda(void) {
  // inertials.setRotation(-180 / 360.0 * pid::actual360, deg);
  inertials.resetRotation();
  control.Screen.clearScreen();
  control.Screen.setCursor(1, 1);
  control.Screen.print("press A to start auton");
  control.Screen.setCursor(2, 1);
  control.Screen.print("press B for driver");
  setStopping(coast);
  for (int i = 0; i < 500; i++) {
    wait(10, msec);
    Brain.Screen.printAt(20, 200, "Angle from program start: %.2f   ", inertials.rotation());
    if (control.ButtonB.pressing()) {
      control.Screen.clearScreen();
      setStopping(brake);
      return;
    }
  }
  while (!control.ButtonA.pressing()) {
    if (control.ButtonB.pressing()) {
      control.Screen.clearScreen();
      setStopping(brake);
      return;
    }
    Brain.Screen.printAt(20, 200, "Angle from program start: %.2f   ", inertials.rotation());
    wait(20, msec);
  }
  setStopping(brake);
  auton();
}

bool SKILLS_MACRO_RUN;
void skillsMacro() {
  control.Screen.clearScreen();
  control.Screen.setCursor(2, 1);
  control.Screen.print("press B for driver");
  spinCat(0);
  SKILLS_MACRO_RUN = true;
  thread thr([]() {
    skillsBeginning();
    SKILLS_MACRO_RUN = false;
    });
  while (1) {
    if (!SKILLS_MACRO_RUN) return;
    if (control.ButtonB.pressing()) {
      thr.interrupt();
      return;
    }
    wait(50, msec);
  }
}

void usercontrol(void) {
  int left, right;
  thread dashboardThread(dashboardLoop);
  toggleController tc;
  tc.bindButton(&control.ButtonY, toggleCat);
  tc.bindButton(&control.ButtonLeft, incrCataModifier);
  tc.bindButton(&control.ButtonRight, decrCataModifier);
  tc.bindButton(&control.ButtonL1, toggleRight);
  tc.bindButton(&control.ButtonL2, toggleLeft);
  tc.bindButton(&control.ButtonA, compSwitchKinda);
  tc.bindButton(&control.ButtonDown, skillsMacro);
  tc.updatePressing();

  int seeCata = 0;

  double sens = 0.7; // sensitivity from 0.0 to 1.0
  double controlLR;
  while (1) {
    left = control.Axis3.position(), right = left;
    controlLR = control.Axis1.position();
    if (controlLR<90 || controlLR>-90) controlLR *= sens;

    left += control.Axis1.position();
    right -= control.Axis1.position();
    drivePct(left + lDriveModifier, right + rDriveModifier);

    if (!distances.isObjectDetected() || distances.objectDistance(mm) > 50) {
      seeCata = 0;
    } else if (seeCata < 10) {
      seeCata++;
    }

    if (cata || seeCata < 1) {
      spinCat(99 + cataModifier);
      spinCat(100 + cataModifier);
    } else {
      spinCat(0);
    }

    if (control.ButtonR1.pressing()) {
      spinIntk(99);
      spinIntk(100);
    } else if (control.ButtonR2.pressing()) {
      spinIntk(-99);
      spinIntk(-100);
    } else {
      spinIntk(0);
    }

    tc.runAllPressed();
    tc.updatePressing();

    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Run the pre-autonomous function.
  pre_auton();

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
