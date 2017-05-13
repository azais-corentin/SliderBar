#include <Arduino.h>

#include <SerialBus.h>
#include <Slider.h>

#include <math.h>

SerialBus bus(Serial);

void receive_data(command cmd);

void setup() {
    bus.begin(115200);
    bus.set_receiver(receive_data);
}

void receive_data(command cmd) {
    switch (cmd.type) {
    case command::FORS_POSITION:
        Serial.println("FORS_POSITON");
        // slider.setPosition(payload[1])
        break;
    default:
        Serial.println("UNDEFINED");
        break;
    }
}

uint64_t ilol = 0;

void loop() {
    // bus.receive();
    // bus.receive(1000);
    // slider.update()

    command cmd;
    cmd.type = command::FORC_POSITION;
    cmd.value = (uint16_t)(fmod(ilol / 10., 100) * 655.36 + 1);
    bus.send_command(cmd);
    delay(50);
    ilol++;
}
