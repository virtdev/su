#ifndef _REQ_H
#define _REQ_H

#define REQ_GET					0x0001
#define REQ_PUT					0x0002
#define REQ_OPEN				0x0004
#define REQ_CLOSE				0x0008
#define REQ_MOUNT				0x0010

#define REQ_HEAD_SIZE			8
#define REQ_SECRET				"VIRTDEVLOGIN"

typedef struct request {
	int len;
	int index;
	int cmd;
	char *buf;
} req_t;

#endif
