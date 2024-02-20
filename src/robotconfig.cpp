#include "robotconfig.h"
#include "vex.h"
vex::brain Brain;
vex::controller control(vex::primary);
// U = up, F = front, B = back
vex::motor MotorRF(vex::PORT17, vex::gearSetting::ratio18_1, false);
vex::motor MotorRM(vex::PORT1, vex::gearSetting::ratio18_1, false);
vex::motor MotorRB(vex::PORT3, vex::gearSetting::ratio18_1, false);
vex::motor MotorLF(vex::PORT20, vex::gearSetting::ratio18_1, true);
vex::motor MotorLM(vex::PORT2, vex::gearSetting::ratio18_1, true);
vex::motor MotorLB(vex::PORT12, vex::gearSetting::ratio18_1, true);
vex::motor_group LeftDrive(MotorLM, MotorLF, MotorLB);
vex::motor_group RightDrive(MotorRM, MotorRF, MotorRB);
vex::motor MotorCat(vex::PORT10, vex::gearSetting::ratio18_1, false);
vex::motor MotorIntk(vex::PORT13, vex::gearSetting::ratio18_1, false);
vex::digital_out Pneu1(Brain.ThreeWirePort.C);
vex::digital_out Pneu2(Brain.ThreeWirePort.D);
vex::digital_out Pneu3(Brain.ThreeWirePort.F);
vex::distance distances(vex::PORT18);
vex::inertial inertials(vex::PORT21);
std::map<int, char *> motorName = {
    {17, "MotorRF"}, {1, "MotorRM"}, {3, "MotorRB"},
    {20, "MotorLF"}, {2, "MotorLM"}, {12, "MotorLB"},
    {10, "MotorCat"}, {13, "MotorIntk"} };
