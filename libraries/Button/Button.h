#ifndef _BUTTON_H
#define _BUTTON_H

#include "../Driver/Driver.h"

class Button: public Driver
{
public:
	Button(int pin);

public:
	void setup();
	void getSpec(String &spec);
	int get(char *buf, size_t size);

private:
	bool m_down;
	unsigned long m_start;
};

#endif
