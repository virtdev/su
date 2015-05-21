#ifndef _IRD_H
#define _IRD_H

#include "../Driver/Driver.h"

class IRD: public Driver
{
public:
	IRD(int pin):Driver(pin) {}

public:
	void setup();
	int get(char *buf, size_t size);

private:
	int m_cnt;
	unsigned long m_start;
};

#endif
