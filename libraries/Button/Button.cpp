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

void Button::setup()
{
	item_t range = itemNew("Enable", itemRange("False", "True"));
	
	set("Button", MODE_VISI | MODE_TRIG, range, 0);
	pinMode(getPin(), INPUT);
	m_start = 0;
	m_down = 0;
}

int Button::get(char *buf, size_t size)
{
	const int interval = 10;
	unsigned long time = millis();

	if (time - m_start < interval)
		return 0;

	m_start = time;
	if (!digitalRead(getPin())) {
		m_down = true;
	} else {
		if (m_down) {
			item_t res = itemNew("Enable", "True");

			m_down = false;
			return itemCopy(res, buf, size);
		}
	}
	return 0;
}
