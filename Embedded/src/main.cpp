#include <Arduino.h>

#include <TaskScheduler.h>

#include <SerialBus.h>
#include <SerialBusDefines.h>
#include <Slider.h>

void receive_data(command cmd);
void sendPosition();

command position = {command::FORC_POSITION, 0};

SerialBus bus(Serial);
Slider slider;
Scheduler runner;
Task tSendPosition(10, -1, &sendPosition, &runner, true);
// Task tComputePID(10, -1, &slider.update &runner, true);

void setup() {
    bus.begin(115200);
    bus.set_receiver(receive_data);

    runner.startNow();
}

void receive_data(command cmd) {
    int speed;
    switch (cmd.type) {
    case command::FORS_POSITION:
        slider.setPosition(cmd.value);
        break;
    case command::FORS_SPEED:
        slider.setSpeed(0);
        break;
    default:
        Serial.println("UNDEFINED");
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
}
