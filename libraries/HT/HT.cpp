/* HT.cpp (Humidity and Temperature)
 *
 * Copyright (C) 2014 Yanpeng Li
 * Copyright (C) 2016 Yi-Wei Ci
 *
 * Distributed under the terms of the MIT license.
 */

#include "HT.h"

HT::HT(int pin):Driver(pin, "HT", MODE_POLL | MODE_SYNC | MODE_VISI | MODE_OUT, 0.01)
{
}

void HT::getSpec(String &spec)
{
	itemSpec(spec, "humidity", "%", "int", "[0, 100]");
	itemSpecNext(spec, "temperature", "Celsius", "int", "[-100, 100]");
}

int HT::get(char *buf, size_t size)
{
	item_t res;
	uint8_t cnt = 7;
	uint8_t idx = 0;
	uint8_t data[5];
	unsigned int loopCnt = 10000;

	for(int i = 0; i < 5; i++)
		data[i] = 0;

	pinMode(m_pin, OUTPUT);
	digitalWrite(m_pin, LOW);
	delay(18);
	digitalWrite(m_pin, HIGH);
	delayMicroseconds(40);
	pinMode(m_pin, INPUT);

	loopCnt = 10000;
	while(digitalRead(m_pin) == LOW)
		if (loopCnt-- == 0)
			return 0;

	loopCnt = 10000;
	while(digitalRead(m_pin) == HIGH)
		if (loopCnt-- == 0)
			return 0;

	for(int bits = 0; bits < 40; bits++) {
		loopCnt = 10000;
		while(digitalRead(m_pin) == LOW)
			if (loopCnt-- == 0)
				return 0;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(m_pin) == HIGH)
			if (loopCnt-- == 0)
				return 0;

		if ((micros() - t) > 40)
			data[idx] |= (1 << cnt);

		if (cnt == 0) {
			cnt = 7;
			idx++;
		} else
			cnt--;
	}

	delayMicroseconds(100);
	pinMode(m_pin, OUTPUT);
	digitalWrite(m_pin, HIGH);

	if(data[4] != data[0] + data[2])
		return 0;

	res = itemNew("humidity", String(data[0]));
	res += itemNext("temperature", String(data[2]));
	return itemCopy(res, buf, size);
}
