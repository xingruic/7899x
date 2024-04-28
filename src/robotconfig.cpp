#include "robotconfig.h"
#include "vex.h"
vex::brain Brain;
vex::controller control(vex::primary);
// U = up, F = front, B = back
vex::motor MotorRF(vex::PORT20, vex::gearSetting::ratio18_1, false);
vex::motor MotorRM(vex::PORT12, vex::gearSetting::ratio18_1, false);
vex::motor MotorRB(vex::PORT3, vex::gearSetting::ratio18_1, false);
vex::motor MotorLF(vex::PORT10, vex::gearSetting::ratio18_1, true);
vex::motor MotorLM(vex::PORT19, vex::gearSetting::ratio18_1, true);
vex::motor MotorLB(vex::PORT18, vex::gearSetting::ratio18_1, true);
vex::motor_group LeftDrive(MotorLM, MotorLF, MotorLB);
vex::motor_group RightDrive(MotorRM, MotorRF, MotorRB);
vex::motor MotorCat(vex::PORT4, vex::gearSetting::ratio18_1, false);
vex::motor MotorIntk(vex::PORT11, vex::gearSetting::ratio18_1, false);
vex::digital_out PneuLF(Brain.ThreeWirePort.A);
vex::digital_out PneuRF(Brain.ThreeWirePort.B);
vex::digital_out PneuLB(Brain.ThreeWirePort.H);
vex::digital_out PneuRB(Brain.ThreeWirePort.G);
vex::digital_out PneuHang(Brain.ThreeWirePort.D);
vex::distance distances(vex::PORT6);
vex::inertial inertials(vex::PORT9);
std::map<int, char *> motorName = {
    {20, "MotorRF"}, {12, "MotorRM"}, {3, "MotorRB"},
    {10, "MotorLF"}, {19, "MotorLM"}, {18, "MotorLB"},
    {4, "MotorCat"}, {11, "MotorIntk"} };
