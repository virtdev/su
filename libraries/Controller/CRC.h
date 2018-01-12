#ifndef _CRC_H
#define _CRC_H

#define CRC_SIZE 2

#define crc_update(crc, ch) ((crc >> 8) ^ crc_table[(crc ^ ch) & 0x00ff])
typedef unsigned short crc_t;

class CRC
{
public:
    crc_t encode(char *buf, int length);
};

#endif
