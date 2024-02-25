#include "drive.h"
#include "robotconfig.h"
#include <iostream>

/**
 * @returns the rotation of the `inertials` sensor, adjusted using `actual360`.
*/
double getRotation() {
    return inertials.rotation() / pid::actual360 * 360;
}

/**
 * sets the stopping mode of the drivetrain to the given amount
 * @param bt the stopping mode
*/
void setStopping(const vex::brakeType &bt) {
    LeftDrive.setStopping(bt);
    RightDrive.setStopping(bt);
}

/**
 * drives with the specified left and right speed in volts
 * @param left the left speed
 * @param right the right speed
*/
void driveVolts(const double &left, const double &right) {
    LeftDrive.spin(vex::directionType::fwd, left, vex::voltageUnits::volt);
    RightDrive.spin(vex::directionType::fwd, right, vex::voltageUnits::volt);
}

/**
 * drives with the specified left and right speed in percent
 * @param left the left speed
 * @param right the right speed
*/
void drivePct(const int &left, const int &right) {
    driveVolts(left * 0.12, right * 0.12);
}

/**
 * drives at specified speed and waits until the drive has driven a certain distance (measured in degrees)
 * @param dist the distance (degrees) to wait for
 * @param lmult to multiply for the left side
 * @param rmult to multiply for the right side
*/
void driveWait(const double &dist, const double &lmult, const double &rmult) {
    LeftDrive.resetPosition();
    RightDrive.resetPosition();
    double curr;
    double error, prev_error;
    error = prev_error = dist;
    while (1) {
        vex::wait(10, vex::msec);
        if (lmult > rmult) curr = LeftDrive.position(vex::deg);
        else curr = RightDrive.position(vex::deg);
        error = dist - curr;
        if (error == 0 || error / prev_error <= 0) break;
        prev_error = error;
        if (error > 0) drivePct(100 * lmult, 100 * rmult);
        else drivePct(-100 * lmult, -100 * rmult);
    }
}

/**
 * drives at specified speed and waits until the drive has driven a certain distance (measured in degrees)
 * @param dist the distance (degrees) to wait for
*/
void driveWait(const double &dist) { driveWait(dist, 1, 1); }

/**
 * driveWait but you specify separate speeds for left and right and instead of waiting until a distance, it waits until the angle is reached
 * @param angle the angle (degrees) to wait for
 * @param lspct the Left Speed in PerCenT
 * @param rspct the Right Speed in PerCenT
 * @param reset whether or not to reset the gyro rotation before arcWait happens
*/
void arcWait(const double &angle, const int &lspct, const int &rspct, bool reset) {
    if (reset) inertials.resetRotation();
    double curr;
    double error, prev_error;
    error = prev_error = angle - getRotation();

    if (error > 0) {
        if (lspct < rspct) {
            std::cout << "bruh wtf\n";
            return;
        }
    } else if (lspct > rspct) {
        std::cout << "bruh wtf\n";
        return;
    }

    while (1) {
        vex::wait(10, vex::msec);
        curr = getRotation();
        error = angle - curr;
        std::cout << "arcwait: " << error << "\n\n";
        if (error == 0 || error / prev_error <= 0) break;
        prev_error = error;
        drivePct(lspct, rspct);
    }
}

/**
 * driveWait but you specify separate speeds for left and right and instead of waiting until a distance, it waits until the angle is reached
 * @param angle the angle (degrees) to wait for
 * @param lspct the Left Speed in PerCenT
 * @param rspct the Right Speed in PerCenT
*/
void arcWait(const double &angle, const int &lspct, const int &rspct) {
    arcWait(angle, lspct, rspct, true);
}


const pid::PIDconsts pid::PID45(1, 0., 1, 1);
const pid::PIDconsts pid::PID90(0.74, 0.2, 0.55, 4);
const pid::PIDconsts pid::PID135(0.65, 0.5, 0.5, 2.5);
const pid::PIDconsts pid::PID180(0.65, 0.2, 1.5, 3);
const pid::PIDconsts pid::PID_DRIVE(0.36, 0.3, 0.5, 5);
const pid::PIDconsts pid::PID_ARC(2.7, 0.5, 1.5, 3);
const pid::PIDconsts pid::PID_SLOWARC(6, 0.5, 1, 3, 60);
const pid::PIDconsts pid::PID_DRIVEANGLE(1, 0, 0, 0);
const pid::PIDconsts pid::PID_FLY(1, 0, 0, 0);

/**
 * drives for a certain distance (measured in degrees)
 * @param dist the distance (degrees) to run the drivetrain to
 * @param timeout the time in ms after which the function will exit
 * @param pc PID constants
*/
void pid::drive(double dist, int timeout, const PIDconsts &pc) {
    LeftDrive.resetPosition();
    RightDrive.resetPosition();
    double li, ri;
    double ld, rd;
    li = ri = ld = rd = 0;
    double lperr, lerr, rperr, rerr; // left prev_error, left error, etc.
    lperr = lerr = rperr = rerr = dist;
    double lcurr, rcurr; lcurr = rcurr = 0;
    double lvelo, rvelo;
    vex::timer t1;
    while (t1.time(vex::msec) < timeout) {
        vex::wait(10, vex::msec);
        lcurr = LeftDrive.position(vex::deg);
        rcurr = RightDrive.position(vex::deg);
        lerr = dist - lcurr;
        rerr = dist - rcurr;
        std::cout << "PID Drive error = (" << lcurr << ", " << rcurr << ")\n\n";
        ld = lerr - lperr;
        rd = rerr - rperr;
        if (lerr < 0 != lperr < 0) {
            li = 0;
        }
        if (rerr < 0 != rperr < 0) {
            ri = 0;
        }
        lperr = lerr;
        rperr = rerr;
        if (fabs(lerr) < pc.threshold) li += lerr;
        if (fabs(rerr) < pc.threshold) ri += rerr;
        lvelo = lerr * pc.kp + li * pc.ki + ld * pc.kd;
        rvelo = rerr * pc.kp + ri * pc.ki + rd * pc.kd;
        if (lvelo > pc.speedPct) lvelo = pc.speedPct;
        if (rvelo > pc.speedPct) rvelo = pc.speedPct;
        if (lvelo < -pc.speedPct) lvelo = -pc.speedPct;
        if (rvelo < -pc.speedPct) rvelo = -pc.speedPct;
        drivePct(lvelo, rvelo);
    }
    drivePct(0, 0);
}

/**
 * drives for a certain distance (measured in degrees)
 * @param dist the distance (degrees) to run the drivetrain to
 * @param timeout the time in ms after which the function will exit
*/
void pid::drive(double dist, int timeout) {
    drive(dist, timeout, PID_DRIVE);
}

/**
 * like pid::drive but with different speeds on left and right.
 * @param lmult to multiply the calculated velocity for the left side
 * @param rmult to multiply the calculated velocity for the right side
 * @param dist the distance to drive
 * @param timeout the time limit for the function
 * @param pc PID constants
*/
void pid::arcDist(double lmult, double rmult, double dist, int timeout, const PIDconsts &pc) {
    LeftDrive.resetPosition();
    RightDrive.resetPosition();
    double pi, pd;
    pi = pd = 0;
    double perr, err;
    perr = err = dist;
    double curr = 0;
    double velo;
    vex::timer t1;
    while (t1.time(vex::msec) < timeout) {
        vex::wait(10, vex::msec);
        if (lmult > rmult) curr = LeftDrive.position(vex::deg);
        else curr = RightDrive.position(vex::deg);
        err = dist - curr;
        std::cout << "PID arcDist error = " << curr << ")\n\n";
        pd = err - perr;
        if (err < 0 != perr < 0) {
            pi = 0;
        }
        perr = err;
        if (fabs(err) < pc.threshold) pi += err;
        velo = err * pc.kp + pi * pc.ki + pd * pc.kd;
        if (velo > pc.speedPct) velo = pc.speedPct;
        if (velo < -pc.speedPct) velo = -pc.speedPct;
        drivePct(velo * lmult, velo * rmult);
    }
    drivePct(0, 0);
}

/**
 * like pid::drive but with different speeds on left and right.
 * @param lmult to multiply the calculated velocity for the left side
 * @param rmult to multiply the calculated velocity for the right side
 * @param dist the distance to drive
 * @param timeout the time limit for the function
*/
void pid::arcDist(double lmult, double rmult, double dist, int timeout) {
    arcDist(lmult, rmult, dist, timeout, PID_DRIVE);
}

/**
 * drives on an arc of radius and angle to the right/left side of the bot.
 * @param right if true, the bot will turn right when going in the positive direction
 * @param radius the radius (degrees) of the circle to turn on
 * @param angle the target angle (positive = forward)
 * @param timeout the time in ms after which the function will exit
 * @param reset whether or not to reset the inertial sensor's rotation before executing the arc
 * @param pc PID constants
 * @todo tune the PID_ARC variable
*/
void pid::arcTurn(bool right, int radius, double angle, int timeout, bool reset, const PIDconsts &pc) {
    // calculate the lSpeed:rSpeed ratio using the radius
    const double PI = 3.14159;
    double lArc, rArc;
    lArc = (radius - WHEEL_TO_CENTER) * (angle / 180 * PI);
    rArc = (radius + WHEEL_TO_CENTER) * (angle / 180 * PI);
    if (right) {
        double t = lArc;
        lArc = rArc;
        rArc = t;
    }
    double lSpeed, rSpeed;
    if (right) {
        lSpeed = 100;
        rSpeed = rArc / lArc * 100;
    } else {
        rSpeed = 100;
        lSpeed = lArc / rArc * 100;
    }
    if (reset)
        inertials.resetRotation();
    double integral = 0;
    double derivative = 0;
    double prev_error, error;
    prev_error = error = angle;
    double curr = 0;
    double velo;
    vex::timer t1;
    while (t1.time(vex::msec) < timeout) {
        vex::wait(10, vex::msec);
        curr = getRotation();
        if (!right)curr *= -1;
        error = angle - curr;
        std::cout << error << "       \n";
        derivative = error - prev_error;
        if (error < 0 != prev_error < 0) {
            integral = 0;
        }
        prev_error = error;
        if (fabs(error) < pc.threshold) integral += error;
        velo = error * pc.kp + integral * pc.ki + derivative * pc.kd;
        if (velo > pc.speedPct) velo = pc.speedPct;
        if (velo < -pc.speedPct) velo = -pc.speedPct;
        velo /= 100;
        drivePct(lSpeed * velo, rSpeed * velo);
    }
    drivePct(0, 0);
}
void pid::arcTurn(bool right, int radius, double angle, int timeout, bool reset) {
    arcTurn(right, radius, angle, timeout, reset, pid::PID_ARC);
}
void pid::arcTurn(bool right, int radius, double angle, int timeout) {
    arcTurn(right, radius, angle, timeout, true);
}

/**
 * turns a specified amount of degrees
 * @param angle the angle (degrees) to turn
 * @param timeout the time in ms after which the function will exit
 * @param reset whether or not to reset the inertial sensor's rotation before executing the turn
 * @param pc PID constants
*/
void pid::turn(double angle, int timeout, bool reset, const PIDconsts &pc) {
    if (reset) inertials.resetRotation();
    double integral = 0;
    double derivative = 0;
    double prev_error, error;
    prev_error = error = angle;
    double curr = 0;
    double velo;
    vex::timer t1;
    while (t1.time(vex::msec) < timeout) {
        vex::wait(10, vex::msec);
        curr = getRotation();
        error = angle - curr;
        std::cout << "PID Turn error = " << error << "\n\n";
        derivative = error - prev_error;
        if (error < 0 != prev_error < 0) {
            integral = 0;
        }
        prev_error = error;
        if (fabs(error) < pc.threshold) integral += error;
        velo = error * pc.kp + integral * pc.ki + derivative * pc.kd;
        if (velo > pc.speedPct) velo = pc.speedPct;
        if (velo < -pc.speedPct) velo = -pc.speedPct;
        drivePct(velo, -velo);
    }
    drivePct(0, 0);
}

/**
 * turns a specified amount of degrees
 * @param angle the angle (degrees) to turn
 * @param timeout the time in ms after which the function will exit
 * @param reset whether or not to reset the inertial sensor's rotation before executing the turn
*/
void pid::turn(double angle, int timeout, bool reset) {
    double error;
    if (reset) error = angle;
    else error = angle - getRotation();
    if (fabs(error) <= 70) turn(angle, timeout, reset, PID45);
    else if (fabs(error) <= 120) turn(angle, timeout, reset, PID90);
    else if (fabs(error) <= 160) turn(angle, timeout, reset, PID135);
    else turn(error, timeout, reset, PID180);
};

/**
 * turns a specified amount of degrees
 * @param angle the angle (degrees) to turn
 * @param timeout the time in ms after which the function will exit
*/
void pid::turn(double angle, int timeout) {
    turn(angle, timeout, true);
}

/**
 * my attempt at driving facing at an angle
 * @pre
*/
void pid::driveAtAngle(int dist, double angle, int timeout, bool reset) {
    if (reset) inertials.resetRotation();

}

void pid::driveAtAngle(int dist, double angle, int timeout) {
    driveAtAngle(dist, angle, timeout, true);
}

/**
 * @param pc the PIDconsts instance to copy the other values from
 * @param speedPct the new speedPct
 * @returns a new PIDconsts instance that is a copy of pc but with speedPct replaced
*/
pid::PIDconsts pid::with_speed(const PIDconsts &pc, int speedPct) {
    return PIDconsts(pc.kp, pc.ki, pc.kd, pc.threshold, speedPct);
}