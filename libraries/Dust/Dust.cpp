/*      Dust.cpp
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

#include "Dust.h"

Dust::Dust(int pin):Driver(pin, "Dust", MODE_POLL | MODE_SYNC | MODE_VISI | MODE_OUT, 0.01)
{
	m_start = 0;
	m_result = 0;
	m_occupancy = 0;
}

void Dust::setup()
{
	pinMode(m_pin, INPUT);
}

void Dust::getSpec(String &spec)
{
	itemSpec(spec, "dust", "ug/m3", "int", "[0, 1000]");
}

int Dust::get(char *buf, size_t size)
{
	item_t res;
	unsigned long t;
	const int interval = 30000;
	
	if (0 == m_start)
		m_start = millis();
	
	t = millis();
	if (m_start > t)
		m_start = t;
	
	m_occupancy += pulseIn(m_pin, LOW);
	if(t - m_start >= interval) {
		float ratio = m_occupancy / (interval * 10.0);
		float concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;
		
		m_result = (int)(concentration * 0.0207916725464941);
		m_occupancy = 0;
		m_start = t;
	}
	res = itemNew("dust", String(m_result));
	return itemCopy(res, buf, size);
}
