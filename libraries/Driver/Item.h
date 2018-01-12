#ifndef _ITEM_H
#define _ITEM_H

#include "Arduino.h"

#define itemEnd() String("}")
#define itemStart() String("{")
#define itemEmpty() String("{}")
#define itemNew(key, value) (itemKey(key) + String(value))
#define itemNext(key, value) (itemKeyNext(key) + String(value))
#define itemKey(key) (String("'") + String(key) + String("':"))
#define itemString(val) (String("'") + String(val) + String("'"))
#define itemFirst(key, value) (String("{") + itemNew(key, value))
#define itemLast(key, value) (itemNext(key, value) + String("}"))
#define itemKeyNext(key) (String(", '") + String(key) + String("':"))
#define itemOne(key, value) (String("{") + itemKey(key) + String(value) + String("}"))

#define itemSpec(spec, name, unit, type, range) do {\
    spec = itemKey(name); \
    spec += itemStart(); \
    if (unit) { \
        spec += itemNew("unit", itemString(unit)); \
        if (type) \
            spec += itemNext("type", itemString(type));\
        if (range) \
            spec += itemNext("range", itemString(range)); \
    } else if (type) { \
        spec += itemNew("type", itemString(type)); \
        if (range) \
            spec += itemNext("range", itemString(range)); \
    } else if (range) { \
        spec += itemNew("range", itemString(range)); \
    } \
    spec += itemEnd(); \
} while(0)

#define itemSpecNext(spec, name, unit, type, range) do { \
    spec += itemKeyNext(name); \
    spec += itemStart(); \
    if (unit) { \
        spec += itemNew("unit", itemString(unit)); \
        if (type) \
            spec += itemNext("type", itemString(type)); \
        if (range) \
            spec += itemNext("range", itemString(range)); \
    } else if (type) { \
        spec += itemNew("type", itemString(type)); \
        if (range) \
            spec += itemNext("range", itemString(range)); \
    } else if (range) { \
        spec += itemNew("range", itemString(range)); \
    } \
    spec += itemEnd(); \
} while (0)

typedef String item_t;

static inline int itemCopy(item_t &item, char *buf, size_t size)
{
    item.toCharArray(buf, size);
    return item.length();
}

#endif
