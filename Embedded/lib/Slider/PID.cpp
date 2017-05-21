#include "Arduino.h"

#include "PID.h"

PID::PID(double *Input, double *Output, double *Setpoint, double Kp, double Ki,
         double Kd, int ControllerDirection) {

    myOutput = Output;
    myInput = Input;
    mySetpoint = Setpoint;
    inAuto = false;

    PID::SetOutputLimits(0, 255);

    SampleTime = 100;

    PID::SetControllerDirection(ControllerDirection);
    PID::SetTunings(Kp, Ki, Kd);

    lastTime = millis() - SampleTime;
}

void PID::setIenabled(bool enabled) { iEnabled = enabled; }

bool PID::Compute() {
    if (!inAuto)
        return false;
    unsigned long now = millis();
    unsigned long timeChange = (now - lastTime);
    if (timeChange >= SampleTime) {
        double input = *myInput;
        double error = *mySetpoint - input;
        if (iEnabled) {
            ITerm += (ki * error);
            if (ITerm > outMax)
                ITerm = outMax;
            else if (ITerm < outMin)
                ITerm = outMin;
        }
        double dInput = (input - lastInput);

        double output;
        if (iEnabled)
            output = kp * error + ITerm - kd * dInput;
        else
            output = kp * error - kd * dInput;

        if (output > outMax)
            output = outMax;
        else if (output < outMin)
            output = outMin;
        *myOutput = output;

        lastInput = input;
        lastTime = now;
        return true;
    } else
        return false;
}

void PID::SetTunings(double Kp, double Ki, double Kd) {
    if (Kp < 0 || Ki < 0 || Kd < 0)
        return;

    dispKp = Kp;
    dispKi = Ki;
    dispKd = Kd;

    double SampleTimeInSec = ((double)SampleTime) / 1000;
    kp = Kp;
    ki = Ki * SampleTimeInSec;
    kd = Kd / SampleTimeInSec;

    if (controllerDirection == REVERSE) {
        kp = (0 - kp);
        ki = (0 - ki);
        kd = (0 - kd);
    }
}

void PID::SetSampleTime(int NewSampleTime) {
    if (NewSampleTime > 0) {
        double ratio = (double)NewSampleTime / (double)SampleTime;
        ki *= ratio;
        kd /= ratio;
        SampleTime = (unsigned long)NewSampleTime;
    }
}

void PID::SetOutputLimits(double Min, double Max) {
    if (Min >= Max)
        return;
    outMin = Min;
    outMax = Max;

    if (inAuto) {
        if (*myOutput > outMax)
            *myOutput = outMax;
        else if (*myOutput < outMin)
            *myOutput = outMin;

        if (ITerm > outMax)
            ITerm = outMax;
        else if (ITerm < outMin)
            ITerm = outMin;
    }
}

void PID::SetMode(int Mode) {
    bool newAuto = (Mode == AUTOMATIC);
    if (newAuto && !inAuto) { /*we just went from manual to auto*/
        PID::Initialize();
    }
    inAuto = newAuto;
}

void PID::Initialize() {
    ITerm = *myOutput;
    lastInput = *myInput;
    if (ITerm > outMax)
        ITerm = outMax;
    else if (ITerm < outMin)
        ITerm = outMin;
}

void PID::SetControllerDirection(int Direction) {
    if (inAuto && Direction != controllerDirection) {
        kp = (0 - kp);
        ki = (0 - ki);
        kd = (0 - kd);
    }
    controllerDirection = Direction;
}

double PID::GetKp() { return dispKp; }
double PID::GetKi() { return dispKi; }
double PID::GetKd() { return dispKd; }
int PID::GetMode() { return inAuto ? AUTOMATIC : MANUAL; }
int PID::GetDirection() { return controllerDirection; }
