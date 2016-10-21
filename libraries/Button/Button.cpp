/*      Button.cpp
 *
 *      Copyright (C) 2014  Yanpeng Li <lyp40293@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
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
