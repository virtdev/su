#ifndef _RELAY_H
#define _RELAY_H

#include "../Driver/Driver.h"

class Relay: public Driver
{
public:
	Relay(int pin):Driver(pin) {}

public:
	void open();
	void close();
	void setup();
};

#endif
