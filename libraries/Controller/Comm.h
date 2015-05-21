#ifndef _COMM_H
#define _COMM_H

#include <string.h>

#define COMM_DLE 	'@'
#define COMM_STX 	'0'
#define COMM_ETX 	'1'
#define COMM_CHR 	"@@"
#define COMM_HEAD 	"@0"
#define COMM_TAIL 	"@1"

#define COMM_TIMEOUT	100
#define COMM_RATE		9600

const int COMM_HEAD_LEN = strlen(COMM_HEAD);
const int COMM_TAIL_LEN = strlen(COMM_TAIL);

class Comm
{
public:
	Comm();
	int get();
	void setup(char *buf, int size);
	void put(char *buf, int length);

private:
	int recv();
	void send(char *buf, int length);
	void sendBytes(char *buf, int length);

private:
	char m_ch;
	int m_size;
	int m_count;
	int m_start;
	char *m_buf;
};

#endif
