#ifndef _PIR_H
#define _PIR_H

#include "../Driver/Driver.h"

class PIR: public Driver
{
public:
	PIR(int pin);

public:
	void setup();
	void getSpec(String &spec);
	int get(char *buf, size_t size);

private:
	bool m_detect;
	unsigned long m_start;
};

#endif
