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

void toggleBackWings() {
  bool k = PneuLB.value();
  PneuLB.set(!k);
  PneuRB.set(!k);
}

int intkSpeed = 0;
int cataModifier = 0;
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
  drivePct(0, 0);
  spinCat(0);
  spinIntk(0);
  setStopping(coast);
  inertials.resetRotation();
  control.Screen.clearScreen();
  control.Screen.setCursor(1, 1);
  control.Screen.print("hold A to start auton");
  for (int cnt = 0;cnt < 50;) {
    if (control.ButtonB.pressing()) {
      control.Screen.clearScreen();
      setStopping(brake);
      driverTime.clear();
      return;
    }
    if (control.ButtonA.pressing()) {
      cnt++;
    } else cnt = 0;
    Brain.Screen.printAt(20, 200, "Angle from reset: %.2f   ", inertials.rotation());
    wait(20, msec);
  }
  setStopping(brake);
  auton();
  control.Screen.clearScreen();
  driverTime.clear();
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
    if (!SKILLS_MACRO_RUN) {
      control.Screen.clearScreen();
      return;
    }
    if (control.ButtonB.pressing()) {
      thr.interrupt();
      control.Screen.clearScreen();
      setStopping(brake);
      return;
    }
    wait(50, msec);
  }
}

void usercontrol(void) {
  int left, right;
  thread dashboardThread(dashboardLoop);
  autonmode::updateControlScreen();
  TCONTROLLER.bindButton(&control.ButtonLeft, autonmode::decrMode);
  TCONTROLLER.bindButton(&control.ButtonY, toggleCat);
  TCONTROLLER.bindButton(&control.ButtonUp, incrCataModifier);
  TCONTROLLER.bindButton(&control.ButtonDown, decrCataModifier);
  TCONTROLLER.bindButton(&control.ButtonL1, []() {openFrontWings = !openFrontWings;});
  TCONTROLLER.bindButton(&control.ButtonB, toggleBackWings);
  TCONTROLLER.bindButton(&control.ButtonA, compSwitchKinda);
  TCONTROLLER.updatePressing();

  int seeCata = 0;

  // double sens = 0.7; // sensitivity from 0.0 to 1.0
  // double controlLR;

  driverTime.clear(); // damn

  while (1) {
    if (driverTime.time(msec) < 500 && control.ButtonX.pressing()) {
      skillsMacro();
    }

    if (autonmode::mode == autonmode::SKILLS) {
      if (driverTime.time(sec) > 55) {
        PneuHang.set(true);
      }
    } else {
      if (driverTime.time(sec) > 95 && control.ButtonL2.pressing()) {
        PneuHang.set(true);
      }
    }
    if (2 <= (0 + control.ButtonUp.pressing() + control.ButtonDown.pressing() + control.ButtonLeft.pressing() + control.ButtonRight.pressing())) {
      PneuHang.set(true);
    }

    left = control.Axis3.position(), right = left;
    left += control.Axis1.position();
    right -= control.Axis1.position();
    drivePct(left + lDriveModifier, right + rDriveModifier);

    if (distances.objectDistance(mm) > 23) {
      seeCata = 0;
    } else if (seeCata < 5) {
      seeCata++;
    }

    if (cata || seeCata < 1) {
      if (MotorCat.velocity(vex::percent) < 10) {
        spinCat(cataModifier + 100);
        spinCat(cataModifier + 99);
      }
    } else {
      spinCat(0);
    }

    if (control.ButtonR1.pressing()) {
      spinIntk(99);
      spinIntk(100);
    } else if (control.ButtonR2.pressing()/* || openFrontWings || control.ButtonL2.pressing()*/) {
      spinIntk(-99);
      spinIntk(-100);
    } else {
      spinIntk(0);
    }

    if (control.ButtonL2.pressing() || openFrontWings) {
      PneuLF.set(true);
      PneuRF.set(true);
    } else {
      PneuLF.set(false);
      PneuRF.set(false);
    }

    TCONTROLLER.runAllPressed();
    TCONTROLLER.updatePressing();

    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  driverTime = timer();
  // Run the pre-autonomous function.
  pre_auton();

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    if (!Competition.isEnabled()) {
      PneuLF.set(false);
      PneuRF.set(false);
      PneuHang.set(false);
    }
    // PneuHang.set(false);
    wait(100, msec);
  }
}
