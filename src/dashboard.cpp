#include "dashboard.h"
#include "robotconfig.h"
#include "vex.h"
#include <string>
using namespace vex;
void drawControls(int x, int y, int horizontal, int vertical) {
  Brain.Screen.setFillColor(black);
  Brain.Screen.drawRectangle(x, y, 50, 100);
  Brain.Screen.drawRectangle(x + 70, y + 25, 100, 50);
  Brain.Screen.setFillColor(transparent);
  Brain.Screen.drawRectangle(x, y, 50, 100);
  Brain.Screen.drawRectangle(x + 70, y + 25, 100, 50);
  Brain.Screen.setFillColor(white);
  if (vertical > 0)
    Brain.Screen.drawRectangle(x, y + 50 - vertical / 2, 50, vertical / 2);
  else
    Brain.Screen.drawRectangle(x, y + 50, 50, -vertical / 2);
  if (horizontal > 0)
    Brain.Screen.drawRectangle(x + 120, y + 25, horizontal / 2, 50);
  else
    Brain.Screen.drawRectangle(x + 120 + horizontal / 2, y + 25,
      -horizontal / 2, 50);
}
void drawVisionCoords(int x, int y, vex::vision::signature &sig) {
  Vision.takeSnapshot(sig);
  Brain.Screen.setFillColor(transparent);
  Brain.Screen.printAt(x, y, "Vision");
  Brain.Screen.printAt(x, y + 20, "\tx: %d", Vision.largestObject.centerX);
  Brain.Screen.printAt(x, y + 40, "\ty: %d", Vision.largestObject.centerY);
}
void drawMotorHeader(int x, int y) {
  Brain.Screen.setFillColor(transparent);
  Brain.Screen.printAt(x, y, "Motor");
  Brain.Screen.printAt(x + 100, y, "Current");
  Brain.Screen.printAt(x + 200, y, "Temp");
}
void drawMotorDisplay(int x, int y, motor *m) {
  Brain.Screen.setFillColor(transparent);
  Brain.Screen.printAt(x, y, true, "%s", motorName[m->index() + 1]);
  if (!m->installed()) {
    Brain.Screen.printAt(x + 100, y, "Motor not installed");
    return;
  }
  float curr = m->current();
  float temp = m->temperature();
  Brain.Screen.printAt(x + 110, y, "%.1fA     ", curr);
  if (curr < 1)
    Brain.Screen.setFillColor(green);
  else if (curr >= 1 && curr <= 2)
    Brain.Screen.setFillColor(yellow);
  else
    Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(x + 140, y - 15, 15, 15);

  Brain.Screen.setFillColor(transparent);
  Brain.Screen.printAt(x + 180, y, "%.1fC     ", temp);
  if (temp < 45)
    Brain.Screen.setFillColor(green);
  else if (temp <= 50 && temp >= 45)
    // TRUE and TRUE --> True
    // TRUE and FALSE --> False
    // FALSE and FALSE --> False
    Brain.Screen.setFillColor(yellow);
  else
    Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(x + 230, y - 15, 15, 15);
  Brain.Screen.setFillColor(transparent);
}
// void drawDriveMotorsDisplays(int x, int y)
// {
//     for (int i = 0, j = 0; i < driveMotors.size(); i++, j += 100)
//     {
//         drawMotorDisplay(20, j, driveMotors[i]);
//     }
// }
void dashboardLoop(void) {
  Brain.Screen.clearScreen();
  while (1) {
    wait(200, msec);
    drawMotorHeader(1, 17);
    drawMotorDisplay(1, 34, &MotorLF);
    drawMotorDisplay(1, 51, &MotorLM);
    drawMotorDisplay(1, 68, &MotorLB);
    drawMotorDisplay(1, 85, &MotorRF);
    drawMotorDisplay(1, 102, &MotorRM);
    drawMotorDisplay(1, 119, &MotorRB);
    drawMotorDisplay(1, 136, &MotorCat);
    drawMotorDisplay(1, 153, &MotorIntk);
    // drawVisionCoords(200, 100, TRIGREEN);
    Brain.Screen.printAt(1, 224, "distances %d %.2f", distances.isObjectDetected(), distances.objectDistance(mm));
    // Brain.Screen.printAt(1, 224, "inertials.heading()  %.2f",
    //   inertials.heading());
    drawControls(300, 30, control.Axis1.position(), control.Axis3.position());

  }
}
