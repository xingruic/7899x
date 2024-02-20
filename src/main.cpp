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
  while (!Competition.isEnabled()) {
    wait(100, msec);
    Brain.Screen.printAt(20, 200, "Angle from program start: %.2f   ", inertials.rotation());
  }
}

void toggle(digital_out &dev) { dev.set(!dev.value()); }

void toggleLeft() { toggle(Pneu2); }

void toggleRight() { toggle(Pneu1); }

void toggleWings() {
  toggleLeft();
  toggleRight();
}

int intkSpeed = 0;
int cataModifier = 0;
bool cata = 0;

void incrCataModifier() {
  cataModifier += 10;
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
  inertials.resetRotation();
  control.Screen.clearScreen();
  control.Screen.setCursor(1, 1);
  control.Screen.print("press A to start auton");
  control.Screen.setCursor(2, 1);
  control.Screen.print("press B for driver");
  setStopping(coast);
  for (int i = 0; i < 500; i++) {
    wait(10, msec);
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

void usercontrol(void) {
  int left, right;
  thread dashboardThread(dashboardLoop);
  toggleController tc;
  tc.bindButton(&control.ButtonY, toggleCat);
  tc.bindButton(&control.ButtonLeft, incrCataModifier);
  tc.bindButton(&control.ButtonRight, decrCataModifier);
  tc.bindButton(&control.ButtonL1, toggleWings);
  tc.bindButton(&control.ButtonL2, []() {Pneu1.set(false); Pneu2.set(false);});
  tc.bindButton(&control.ButtonA, compSwitchKinda);
  tc.updatePressing();
  while (1) {
    left = control.Axis3.position(), right = left;
    left += control.Axis1.position();
    right -= control.Axis1.position();
    drivePct(left + lDriveModifier, right + rDriveModifier);

    if (cata || distances.isObjectDetected() && distances.objectDistance(mm) < 50) {
      spinCat(100 + cataModifier);
    } else {
      spinCat(0);
    }

    if (control.ButtonR1.pressing()) {
      spinIntk(100);
    } else if (control.ButtonR2.pressing()) {
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
