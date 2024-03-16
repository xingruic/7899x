#pragma once
#include "vex.h"

double getRotation();

void setStopping(const vex::brakeType &bt);

void driveVolts(const double &left, const double &right);

void drivePct(const int &left, const int &right);

void driveWait(const double &dist, const double &lmult, const double &rmult);
void driveWait(const double &dist);

void arcWait(const double &angle, const int &lspct, const int &rspct, bool reset, int maxtime);
void arcWait(const double &angle, const int &lspct, const int &rspct, bool reset);
void arcWait(const double &angle, const int &lspct, const int &rspct);

namespace pid {
    // actual360 is how many degrees the gyro sensor thinks it turned when it is turned 360 degrees clockwise.
    const double actual360 = 358.2;

    struct PIDconsts {
        const double kp, ki, kd, threshold;
        const int speedPct;
        PIDconsts(double kp = 0, double ki = 0, double kd = 0, double threshold = 10, int speedPct = 100) :kp(kp), ki(ki), kd(kd), threshold(threshold), speedPct(speedPct) {}
    };
    extern const PIDconsts PID45, PID90, PID135, PID180, PID_DRIVE, PID_ARC, PID_SLOWARC, PID_DRIVEANGLE, PID_FLY;
    const double WHEEL_TO_CENTER = NULL; // the distance (degrees) from a wheel to the center
    void drive(double dist, int timeout, const struct PIDconsts &pc);
    void drive(double dist, int timeout);
    void arcDist(double lmult, double rmult, double dist, int timeout, const PIDconsts &pc);
    void arcDist(double lmult, double rmult, double dist, int timeout);
    void arcTurn(bool right, int radius, double angle, int timeout, bool reset, const struct PIDconsts &pc);
    void arcTurn(bool right, int radius, double angle, int timeout, bool reset);
    void arcTurn(bool right, int radius, double angle, int timeout);
    void turn(double angle, int timeout, bool reset, const struct PIDconsts &pc);
    void turn(double angle, int timeout, bool reset);
    void turn(double angle, int timeout);
    void driveAtAngle(int dist, double angle, int timeout, bool reset);
    void driveAtAngle(int dist, double angle, int timeout);
    PIDconsts with_speed(const PIDconsts &pc, int speedPct);
}