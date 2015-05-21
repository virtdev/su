/*      USD.cpp (Ultrasonic Detector)
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

#include "USD.h"

USD::USD(int pinTrig, int pinEcho):Driver(pinTrig)
{
	m_cnt = 0;
	m_hits = 0;
	m_start = 0;
	m_pinTrig = pinTrig;
	m_pinEcho = pinEcho;
}

void USD::setup()
{
	item_t range = itemNew("Enable", itemRange("False", "True"));
	
	set("USD", MODE_VISI | MODE_TRIG, range, 0);
	pinMode(m_pinTrig, OUTPUT);
	pinMode(m_pinEcho, INPUT);
}

bool USD::check()
{
	float distance;
	const int d0 = 30;
	const int d1 = 70;

	digitalWrite(m_pinTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(m_pinTrig, HIGH);
	delayMicroseconds(10);
	digitalWrite(m_pinTrig, LOW);
	distance = pulseIn(m_pinEcho, HIGH) / 58.82;
	if ((distance > d0) && (distance < d1))
		return true;
	else
		return false;
}

int USD::get(char *buf, size_t size)
{
	const int hits = 10;
	const int total = 25;
	const int interval = 20;
	unsigned long time = millis();

	if (time - m_start < interval)
		return false;

	m_start = time;
	if (m_cnt > 0)
		m_cnt++;

	if (check()) {
		if (0 == m_cnt) {
			m_cnt = 1;
			m_hits = 1;
			enableNotifier();
		} else
			m_hits++;
	} 
	else if (!m_cnt)
		disableNotifier();

	if (m_cnt == total) {
		m_cnt = 0;
		if (m_hits >= hits) {
			item_t res = itemNew("Enable", "True");
	
			return itemCopy(res, buf, size);
		}
	}
    return 0;
}
