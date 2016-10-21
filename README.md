# SU
A framework of hard driver

# Quick Start
### Configure
The configuration file is as follows:
* SU/SU.ino

Example:
Add two devices HT(DHT11) and LED.
```c
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
```

The definition of pin is as follows:
* libraries/Pin/Pin.h
```c
#define PIN_HT			12
#define PIN_LED		9
```

# License
SU is released under the terms of the MIT License.
