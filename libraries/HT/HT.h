#ifndef _HT_H
#define _HT_H

#include "../Driver/Driver.h"

class HT: public Driver
{
public:
	HT(int pin):Driver(pin) {}

public:
	void setup();
	int get(char *buf, size_t size);
};

#endif
