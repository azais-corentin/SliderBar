#ifndef PID_H
#define PID_H

#define LIBRARY_VERSION 1.1.1

#define AUTOMATIC 1
#define MANUAL 0
#define DIRECT 0
#define REVERSE 1

class PID {
  public:
    PID(double *Input, double *Output, double *Setpoint, double Kp, double Ki,
        double Kd, int ControllerDirection);

    void setIenabled(bool enabled);

    void SetMode(int Mode);
    bool Compute();
    void SetOutputLimits(double, double);

    void SetTunings(double, double, double);
    void SetControllerDirection(int);
    void SetSampleTime(int);
    double GetKp();
    double GetKi();
    double GetKd();
    int GetMode();
    int GetDirection();

  private:
    void Initialize();

    double dispKp;
    double dispKi;
    double dispKd;

    double kp;
    double ki;
    double kd;

    int controllerDirection;

    double *myInput;
    double *myOutput;
    double *mySetpoint;

    unsigned long lastTime;
    double ITerm, lastInput;

    unsigned long SampleTime;
    double outMin, outMax;
    bool inAuto;
    bool iEnabled = true;
};
#endif // PID_H
