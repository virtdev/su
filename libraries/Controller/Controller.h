#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "Req.h"
#include "CRC.h"
#include "Comm.h"
#include "../Driver/Driver.h"

#define BUF_SIZE         320
#define DEVICE_MAX         2

class Controller
{
public:
    Controller();
    void proc();
    void setup();
    int add(Driver *device);

private:
    int receive();
    void procReq();
    void procEvent();
    void sendProfile();
    void mount(req_t *req);
    Driver *find(int index);
    int getRequest(req_t *req);
    char *getReply(Driver *device);
    void reply(Driver *device, size_t len);
    const size_t getReplySize(Driver *device);
    int getProfile(Driver *device, char *buf, int size);

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
