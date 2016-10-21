/*      HCHO.cpp
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
