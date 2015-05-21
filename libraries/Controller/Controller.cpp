/*      Controller.cpp
 *
 *      Copyright (C) 2014  Yi-Wei Ci <ciyiwei@hotmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include "Controller.h"

Driver *_notifier = NULL;

Controller::Controller()
{
	m_len = 0;
	m_total = 0;
	m_reply = NULL;
	m_mount = false;
}

void Controller::setup()
{
	m_comm.setup(m_buf, BUF_SIZE);
	for (int i = 0; i < m_total; i++)
		m_devices[i]->setup();
}


int Controller::add(Driver *device)
{
	if (m_total == DEVICE_MAX)
		return -1;
	
	m_devices[m_total] = device;
	m_total++;
	return 0;
}

void Controller::reply(Driver *device, size_t len)
{
	if (len > 0) {
		int size = BUF_SIZE - m_len;
		item_t key = itemKey(device->getIndex());
		
		if (key.length() + len + 4 > size)
			return;
		
		m_reply[key.length() + len + 2] = '}';
		m_reply[key.length() + len + 3] = '}';
		m_comm.put(m_reply, key.length() + len + 4);
	} else
		m_comm.put(m_reply, 0);
}

char *Controller::getReplyBuf(Driver *device)
{
	int len;
	int size = BUF_SIZE - m_len;
	int index = device->getIndex();
	item_t key = itemKey(index);
	
	if (key.length() + 4 >= size)
		return NULL;
	
	m_reply[0] = '{';
	key.toCharArray(&m_reply[1], size - 2);
	len = 1 + key.length();
	m_reply[len] = '{';
	return &m_reply[len + 1];
}

const size_t Controller::getReplyBufSize(Driver *device)
{
	item_t key = itemKey(device->getIndex());
	
	return BUF_SIZE - 4 - key.length();
}

int Controller::getRequest(req_t *req)
{
	if (m_len < CRC_SIZE + REQ_HEAD_SIZE)
		return -1;

	memcpy(&req->index, &m_buf[CRC_SIZE], 4);
	memcpy(&req->flags, &m_buf[CRC_SIZE + 4], 4);
	req->buf = &m_buf[CRC_SIZE + REQ_HEAD_SIZE];
	req->len = m_len - REQ_HEAD_SIZE - CRC_SIZE;
	return 0;
}

void Controller::sendDeviceInfo()
{
	int index;
	item_t key;
	int len = 1;
	int sz_info;
	int size = BUF_SIZE - m_len;
	
	m_reply[0] = '{';
	if (m_total > 0) {
		index = m_devices[0]->getIndex();
		key = itemKey(index);
		if (key.length() + len >= size)
			return;
		key.toCharArray(&m_reply[len], size - len);
		len += key.length();
		sz_info = m_devices[0]->getInfo(&m_reply[len], size - len);
		if (0 == sz_info)
			return;
		len += sz_info;
		for (int i = 1; i < m_total; i++) {
			index = m_devices[i]->getIndex();
			key = itemKeyNext(index);
			if (key.length() + len >= size)
				return;
			key.toCharArray(&m_reply[len], size - len);
			len += key.length();
			sz_info = m_devices[i]->getInfo(&m_reply[len], size - len);
			if (0 == sz_info)
				return;
			len += sz_info;
		}
	}
	m_reply[len] = '}';
	m_comm.put(m_reply, len + 1);
}

void Controller::mount(req_t *req)
{
	int len;

	if (strncmp(req->buf, REQ_SECRET, strlen(REQ_SECRET)))
		return;
	
	sendDeviceInfo();
	m_mount = true;
}

int Controller::receive()
{
	int ret = m_comm.get();

	if (ret > 0) {
		m_len = ret;
		m_reply = &m_buf[m_len];
		return 0;
	} else
		return -1;
}

void Controller::checkEvent()
{
	if(!m_mount)
		return;
	
	for (int i = 0; i < m_total; i++) {
		Driver *device = m_devices[i];
		
		if (device->getMode() & MODE_TRIG) {
			int len = device->get(getReplyBuf(device), getReplyBufSize(device));
		
			if (len > 0)
				reply(device, len);
		}
	}
}

Driver *Controller::find(int index)
{
	for (int i = 0; i < m_total; i++)
		if (m_devices[i]->getIndex() == index)
			return m_devices[i];
	return NULL;
}

void Controller::checkReq()
{
	int len;
	req_t req;
	Driver *device;

	if (receive() < 0)
		return;
	
	if (getRequest(&req) < 0)
		return;
	
	if (req.flags & REQ_MOUNT) {
		mount(&req);
		return;
	}
	
	if(!m_mount)
		return;

	device = find(req.index);
	if (!device)
		return;

	if (req.flags & REQ_OPEN) {
    	device->open();
	} else if (req.flags & REQ_GET) {
		len = device->get(getReplyBuf(device), getReplyBufSize(device));
		reply(device, len);
	} else if (req.flags & REQ_PUT) {
		len = device->put(req.buf, req.len, getReplyBuf(device), getReplyBufSize(device));
		reply(device, len);
	} else if (req.flags & REQ_CLOSE) {
		device->close();
	}
}

void Controller::proc()
{
	checkReq();
	checkEvent();
}
