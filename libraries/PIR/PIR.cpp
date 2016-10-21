/* PIR.cpp
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "PIR.h"

PIR::PIR(int pin):Driver(pin, "PIR", MODE_VISI | MODE_TRIG, 0)
{
	m_start = 0;
	m_detect = 0;
}

void PIR::setup()
{
	pinMode(m_pin, INPUT);
}

void PIR::getSpec(String &spec)
{
	itemSpec(spec, "enable", NULL, NULL, NULL);
}

int PIR::get(char *buf, size_t size)
{
	unsigned long t;
	const int interval = 20;

	if (0 == m_start)
		m_start = millis();

	t = millis();
	if (m_start > t)
		m_start = t;

	if (t - m_start >= interval) {
		m_start = t;
		if(HIGH == digitalRead(m_pin)) {
			if (!m_detect) {
				item_t res = itemNew("enable", "true");

				m_detect = true;
				return itemCopy(res, buf, size);
			}
		} else {
			if (m_detect) {
				item_t res = itemNew("enable", "false");

				m_detect =false;
				return itemCopy(res, buf, size);
			}
		}
	}
	return 0;
}
