#ifndef _HT_H
#define _HT_H

#include "../Driver/Driver.h"

class HT: public Driver
{
public:
    HT(int pin);

public:
    void getSpec(String &spec);
    int get(char *buf, size_t size);
};

#endif
