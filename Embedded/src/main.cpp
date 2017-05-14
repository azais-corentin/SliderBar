#include <Arduino.h>

#include <SerialBus.h>
#include <Slider.h>

#include <math.h>

SerialBus bus(Serial);
Slider slider;

void receive_data(command cmd);

void setup() {
    bus.begin(115200);
    bus.set_receiver(receive_data);
}

void receive_data(command cmd) {
    command feedback;
    feedback.type = static_cast<command::command_type>(cmd.type + 0x7F);
    feedback.value = cmd.value;
    bus.sendPacket(feedback);

    switch (cmd.type) {
    case command::FORS_POSITION:
        // slider.setPosition(cmd.value / 655.36)
        break;
    default:
        Serial.println("UNDEFINED");
        break;
    }
}

void loop() {
    bus.receivePacket();
    slider.update();
    // bus.receive(1000);
    // slider.update()

    /*command cmd;
    cmd.type = command::FORC_POSITION;
    cmd.value = (uint16_t)(fmod(ilol / 10., 100) * 655.36 + 1);
    bus.sendPacket(cmd);
    delay(50);
    ilol++;
    */
}
