#ifndef _LED_H
#define _LED_H

#include "../Driver/Driver.h"

class LED: public Driver
{
public:
    LED(int pin);

public:
    void open();
    void close();
    void setup();
};

#endif
