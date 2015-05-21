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

void PIR::setup()
{
	item_t range = itemNew("Enable", itemRange("False", "True"));
	
	set("PIR", MODE_VISI | MODE_TRIG, range, 0);
	pinMode(getIndex(), INPUT);
	m_detect = 0;
	m_start = 0;
}

int PIR::get(char *buf, size_t size)
{
	const int interval = 20;
	unsigned long time = millis();

	if (time - m_start < interval)
		return 0;

	m_start = time;
	if(HIGH == digitalRead(getPin())) {
		if (!m_detect) {
			item_t res = itemNew("Enable", "True");
			
			m_detect = true;
			return itemCopy(res, buf, size);
		}
	} else {
		if (m_detect) {
			item_t res = itemNew("Enable", "False");
			
			m_detect =false;
			return itemCopy(res, buf, size);
		}
	}
	return 0;
}
