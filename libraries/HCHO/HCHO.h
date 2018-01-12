#ifndef _HCHO_H
#define _HCHO_H

#include "../Driver/Driver.h"

class HCHO: public Driver
{
public:
    HCHO(int pin);

public:
    void setup();
    void getSpec(String &spec);
    int get(char *buf, size_t size);
};

#endif
