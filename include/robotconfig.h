#pragma once
#include "vex.h"
#include <map>
#include <string>
extern vex::brain Brain;
extern vex::controller control;
extern vex::motor MotorLF;
extern vex::motor MotorLM;
extern vex::motor MotorLB;
extern vex::motor MotorRF;
extern vex::motor MotorRM;
extern vex::motor MotorRB;
extern vex::motor_group LeftDrive;
extern vex::motor_group RightDrive;
extern vex::motor MotorCat;
extern vex::motor MotorIntk;
extern vex::digital_out PneuLF;
extern vex::digital_out PneuRF;
extern vex::digital_out PneuLB;
extern vex::digital_out PneuRB;
extern vex::digital_out Pneu3;
extern vex::distance distances;
extern vex::inertial inertials;
extern vex::vision::signature TRIGREEN;
extern vex::vision Vision;
extern std::map<int, char *> motorName;