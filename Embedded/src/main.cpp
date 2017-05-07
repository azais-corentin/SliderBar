#include <Arduino.h>

#include <SerialBus.h>
#include <Slider.h>

SerialBus bus(Serial, 115200);

void receive_data(command cmd);

void setup() {
  // bus.begin();
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

void loop() {
  // bus.receive();
  // bus.receive(1000);
  // slider.update()

  command cmd;
  cmd.type = command::FORS_POSITION;
  cmd.value = (uint16_t)50. / 655.36;
  bus.send_command(cmd);
}
