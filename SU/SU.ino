#include <HT.h>
#include <Pin.h>
#include <LED.h>
#include <Controller.h>

Controller ctrl = Controller();

HT ht = HT(PIN_HT);
LED led = LED(PIN_LED);

void create() {
  ctrl.add(&ht);
  ctrl.add(&led);
}

void setup() {
  create();
  ctrl.setup();
}

void loop() {
  ctrl.proc();
}
