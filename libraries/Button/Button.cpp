/* Button.cpp
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "Button.h"

Button::Button(int pin):Driver(pin, "Button", MODE_VISI | MODE_TRIG, 0)
{
	m_down = 0;
	m_start = 0;
}

void Button::setup()
{
	pinMode(m_pin, INPUT);
}

void Button::getSpec(String &spec)
{
	itemSpec(spec, "enable", NULL, NULL, NULL);
}

int Button::get(char *buf, size_t size)
{
	unsigned long t;
	const int interval = 10;

	if (0 == m_start)
		m_start = millis();

	t = millis();
	if (m_start > t)
		m_start = t;

	if (t - m_start >= interval) {
		m_start = t;
		if (!digitalRead(m_pin)) {
			m_down = true;
		} else {
			if (m_down) {
				item_t res = itemNew("enable", "true");

				m_down = false;
				return itemCopy(res, buf, size);
			}
		}
	}
	return 0;
}
