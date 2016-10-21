#ifndef _BUZZER_H
#define _BUZZER_H

#include "../Driver/Driver.h"

class Buzzer: public Driver
{
public:
	Buzzer(int pin);

public:
	void open();
	void close();
	void setup();
};

#endif
