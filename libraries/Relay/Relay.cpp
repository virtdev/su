/* Relay.cpp
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "Relay.h"

Relay::Relay(int pin):Driver(pin, "Relay", MODE_VISI | MODE_SWITCH, 0)
{
}

void Relay::setup()
{
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, HIGH);
}

void Relay::open()
{
    digitalWrite(m_pin, LOW);
}

void Relay::close()
{
    digitalWrite(m_pin, HIGH);
}
