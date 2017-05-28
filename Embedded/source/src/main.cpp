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
    int speed;
    switch (packet.type) {
    case command::FORS_POSITION:
        slider.setPosition(packet.value);
        break;
    case command::FORS_SPEED:
        slider.setSpeed(0);
        break;
    case command::FORS_START_PID:
        slider.setMode(AUTOMATIC);
        break;
    case command::FORS_STOP_PID:
        slider.setMode(MANUAL);
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
