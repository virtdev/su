#ifndef _DRIVER_H
#define _DRIVER_H

#include "Item.h"
#include "Mode.h"
#include "Notifier.h"

class Driver
{
public:
	Driver(int pin, String name, int mode, float freq) 
	{
		m_pin = pin;
		m_index = pin;
		m_name = name;
		m_mode = mode;
		m_freq = freq;
	}

public:
	virtual void open() {}
	virtual void close() {}
	virtual void setup() {}
	virtual void getSpec(String &spec) {spec = "";}
	virtual int get(char *buf, size_t size) {return 0;}
	virtual int put(char *in, size_t inlen, char *out, size_t outlen) {return 0;}

public:
	int m_pin;
	int m_mode;
	int m_index;
	float m_freq;
	String m_name;
};

#endif
