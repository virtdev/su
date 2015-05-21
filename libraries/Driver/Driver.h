#ifndef _DRIVER_H
#define _DRIVER_H

#include "Item.h"
#include "Mode.h"
#include "Notifier.h"

class Driver
{
public:
	Driver(int pin) 
	{
		m_freq = 0;
		m_mode = 0;
		m_pin = pin;
		m_index = pin;
	}
	
	void set(String type, int mode, String range, float freq)
	{
		m_type = type;
		m_mode = mode;
		m_range = range;
		m_freq = freq;
	}

public:
	virtual void open() {}
	virtual void close() {}
	virtual void setup() {}
	virtual int get(char *buf, size_t size) {return 0;}
	virtual int put(char *in, size_t inlen, char *out, size_t outlen) {return 0;}

public:
	int getPin() {return m_pin;}
	int getMode() {return m_mode;}
	int getIndex() {return m_index;}
	int getInfo(char *buf, int size) 
	{ 
		int len;
		String s;
		
		s = String("{'type':'") + m_type + "', 'mode':" + String(m_mode);
		if (s.length() >= size)
			return 0;
		s.toCharArray(buf, size);
		len = s.length();
		if (m_range.length() > 0) {
			s = String(", 'range':{") + m_range + "}";
			if (s.length() + len >= size)
				return 0;
			s.toCharArray(&buf[len], size - len);
			len += s.length();
		}
		if (m_freq > 0) {
			s = String(", 'freq':") + String(m_freq);
			if (s.length() + len >= size)
				return 0;
			s.toCharArray(&buf[len], size - len);
			len += s.length();
		}
		buf[len] = '}';
		return len + 1;
	}

public:
	int m_pin;
	int m_mode;
	int m_index;
	float m_freq;
	String m_type;
	String m_range;
};

#endif
