#include <Arduino.h>

#include <TaskScheduler.h>

#include <SerialBus.h>
#include <SerialBusDefines.h>
#include <Slider.h>

void receive_data(command cmd);
void sendPosition();

command position;

SerialBus bus(Serial);
Slider slider;
Scheduler runner;
Task tSendPosition(10, -1, &sendPosition, &runner, true);

void setup() {
    position.type = command::FORC_POSITION;
    position.value = 0;

    bus.begin(115200);
    bus.set_receiver(receive_data);

    runner.startNow();
}

void receive_data(command packet) {
    switch (packet.type) {
    case command::FORS_POSITION:
        slider.setPosition(packet.value);
        break;
    case command::FORS_SPEED:
        slider.setSpeed(packet.value);
        break;
    case command::FORS_START_PID:
        slider.setMode(AUTOMATIC);
        break;
    case command::FORS_STOP_PID:
        slider.setMode(MANUAL);
        break;
    case command::FORS_RESIST_AT:
        slider.appendResist(packet.value);
        break;
    case command::FORS_RESIST_CLEAR:
        slider.clearResists();
        break;
    case command::FORS_PID_P:
        slider.setP(static_cast<double>(packet.value) / 327.68);
        break;
    case command::FORS_PID_I:
        slider.setI(static_cast<double>(packet.value) / 327.68);
        break;
    case command::FORS_PID_D:
        slider.setD(static_cast<double>(packet.value) / 327.68);
        break;
    default:
        break;
    }
}

void serialEvent() { bus.receivePacket(); }

void loop() {
    runner.execute();
    slider.update();
}

void sendPosition() {
    position.value = slider.getPosition();
    bus.sendPacket(position);

    command est_pos;
    command est_vel;

    est_pos.type = command::FORC_EST_POS;
    est_pos.value = static_cast<uint16_t>(slider.getEstPosition());
    est_vel.type = command::FORC_EST_VEL;
    est_vel.value = static_cast<uint16_t>(slider.getEstVelocity() + 500);

    bus.sendPacket(est_pos);
    bus.sendPacket(est_vel);
}
