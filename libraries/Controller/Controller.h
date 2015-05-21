#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "Req.h"
#include "CRC.h"
#include "Comm.h"
#include "../Driver/Driver.h"

#define DEVICE_MAX 		4
#define BUF_SIZE 		350

class Controller
{
public:
	Controller();
	void proc();
	void setup();
	int add(Driver *device);

private:
	int receive();
	void checkReq();
	void checkEvent();
	void sendDeviceInfo();
	void mount(req_t *req);
	Driver *find(int index);
	int getRequest(req_t *req);
	char *getReplyBuf(Driver *device);
	void reply(Driver *device, size_t len);
	const size_t getReplyBufSize(Driver *device);

private:
	int m_len;
	int m_total;
	Comm m_comm;
	bool m_mount;
	char *m_reply;
	char m_buf[BUF_SIZE];
	Driver *m_devices[DEVICE_MAX];
};

#endif
