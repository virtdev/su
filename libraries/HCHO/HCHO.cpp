/* HCHO.cpp
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "HCHO.h"

HCHO::HCHO(int pin):Driver(pin, "HCHO", MODE_POLL | MODE_SYNC | MODE_VISI | MODE_OUT, 0.01)
{
}

void HCHO::setup()
{
    pinMode(m_pin, INPUT);
}

void HCHO::getSpec(String &spec)
{
    itemSpec(spec, "hcho", "PPM", "int", "[0, 4096]");
}

int HCHO::get(char *buf, size_t size)
{
    int val = analogRead(m_pin);
    item_t res = itemNew("hcho", String(val));

    return itemCopy(res, buf, size);
}
