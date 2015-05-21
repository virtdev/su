#include <HT.h>
#include <Pin.h>
#include <Dust.h>
#include <HCHO.h>
#include <Controller.h>

Controller ctrl = Controller();

HT ht0 = HT(PIN_HT0);
HCHO hcho0 = HCHO(PIN_HCHO0);
Dust dust0 = Dust(PIN_DUST0);

void create() {
  ctrl.add(&ht0);
  ctrl.add(&hcho0);
  ctrl.add(&dust0);;
}

void setup() {
  create();
  ctrl.setup();
}

void loop() {
  ctrl.proc();
}

