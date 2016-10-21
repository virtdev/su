/*      PIR.cpp
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
