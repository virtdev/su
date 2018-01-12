/* IRD.cpp (Infrared Detector)
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "IRD.h"

IRD::IRD(int pin):Driver(pin, "IRD", MODE_VISI | MODE_TRIG, 0)
{
    m_cnt = 0;
    m_start = 0;
}

void IRD::setup()
{
    pinMode(m_pin, INPUT);
}

void IRD::getSpec(String &spec)
{
    itemSpec(spec, "enable", NULL, NULL, NULL);
}

int IRD::get(char *buf, size_t size)
{
    unsigned long t;
    const int total = 30;
    const int interval = 20;

    if (0 == m_start)
        m_start = millis();

    t = millis();
    if (m_start > t)
        m_start = t;

    if (t - m_start >= interval) {
        m_start = t;
        if (!digitalRead(m_pin))
            m_cnt++;
        else
            m_cnt = 0;

        if (m_cnt == total) {
            m_cnt = 0;
            item_t res = itemNew("enable", "true");

            return itemCopy(res, buf, size);
        }
    }
    return 0;
}
