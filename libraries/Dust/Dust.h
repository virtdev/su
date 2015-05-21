#ifndef _DUST_H
#define _DUST_H

#include "../Driver/Driver.h"

class Dust: public Driver
{
public:
	Dust(int pin):Driver(pin) {}
	
public:
	void setup();
	int get(char *buf, size_t size);

private:
	int m_result;
	unsigned long m_start;
	unsigned long m_occupancy;
};

#endif