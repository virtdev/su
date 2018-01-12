/* LED.cpp
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "LED.h"

LED::LED(int pin):Driver(pin, "LED", MODE_VISI | MODE_SWITCH, 0)
{
}

void LED::setup()
{
    pinMode(m_pin, OUTPUT);
}

void LED::open()
{
    digitalWrite(m_pin, HIGH);
}

void LED::close()
{
    digitalWrite(m_pin, LOW);
}
