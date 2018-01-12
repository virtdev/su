/* Buzzer.cpp
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "Buzzer.h"

Buzzer::Buzzer(int pin):Driver(pin, "Buzzer", MODE_VISI | MODE_SWITCH, 0)
{
}

void Buzzer::setup()
{
    pinMode(m_pin, OUTPUT);
}

void Buzzer::open()
{
    tone(m_pin, 5000);
}

void Buzzer::close()
{
    noTone(m_pin);
}
