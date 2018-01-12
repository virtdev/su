#ifndef _USD_H
#define _USD_H

#include "../Driver/Driver.h"

class USD: public Driver
{
public:
    USD(int pinTrig, int pinEcho);

public:
    void setup();
    void getSpec(String &spec);
    int get(char *buf, size_t size);

private:
    bool check();

private:
    int m_cnt;
    int m_hits;
    int m_pinTrig;
    int m_pinEcho;
    unsigned long m_start;
};

#endif
