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
		item_t key = itemKey(device->m_index);

		if (key.length() + len + 4 > size)
			return;

		m_reply[key.length() + len + 2] = '}';
		m_reply[key.length() + len + 3] = '}';
		m_comm.put(m_reply, key.length() + len + 4);
	} else
		m_comm.put(m_reply, 0);
}

char *Controller::getReply(Driver *device)
{
	int len;
	int size = BUF_SIZE - m_len;
	int index = device->m_index;
	item_t key = itemKey(index);

	if (key.length() + 4 >= size)
		return NULL;

	m_reply[0] = '{';
	key.toCharArray(&m_reply[1], size - 2);
	len = 1 + key.length();
	m_reply[len] = '{';
	return &m_reply[len + 1];
}

const size_t Controller::getReplySize(Driver *device)
{
	item_t key = itemKey(device->m_index);

	return BUF_SIZE - 4 - key.length();
}

int Controller::getRequest(req_t *req)
{
	if (m_len < CRC_SIZE + REQ_HEAD_SIZE)
		return -1;

	memcpy(&req->index, &m_buf[CRC_SIZE], 4);
	memcpy(&req->cmd, &m_buf[CRC_SIZE + 4], 4);
	req->buf = &m_buf[CRC_SIZE + REQ_HEAD_SIZE];
	req->len = m_len - REQ_HEAD_SIZE - CRC_SIZE;
	return 0;
}

int Controller::getProfile(Driver *device, char *buf, int size)
{
	int len;
	String s;
	String spec;

	s = String("{'type':'") + device->m_name + "', 'mode':" + String(device->m_mode);
	if (s.length() >= size)
		return 0;
	s.toCharArray(buf, size);
	len = s.length();
	device->getSpec(spec);
	if (spec.length() > 0) {
		s = String(", 'spec':{") + spec + "}";
		if (s.length() + len >= size)
			return 0;
		s.toCharArray(&buf[len], size - len);
		len += s.length();
	}
	if (device->m_freq > 0) {
		s = String(", 'freq':") + String(device->m_freq);
		if (s.length() + len >= size)
			return 0;
		s.toCharArray(&buf[len], size - len);
		len += s.length();
	}
	buf[len] = '}';
	return len + 1;
}

void Controller::sendProfile()
{
	int len;
	int index;
	item_t key;
	int cnt = 1;
	int size = BUF_SIZE - m_len;

	m_reply[0] = '{';
	if (m_total > 0) {
		index = m_devices[0]->m_index;
		key = itemKey(index);
		if (key.length() + cnt >= size)
			return;
		key.toCharArray(&m_reply[cnt], size - cnt);
		cnt += key.length();
		len = getProfile(m_devices[0], &m_reply[cnt], size - cnt);
		if (0 == len)
			return;
		cnt += len;
		for (int i = 1; i < m_total; i++) {
			index = m_devices[i]->m_index;
			key = itemKeyNext(index);
			if (key.length() + cnt >= size)
				return;
			key.toCharArray(&m_reply[cnt], size - cnt);
			cnt += key.length();
			len = getProfile(m_devices[i], &m_reply[cnt], size - cnt);
			if (0 == len)
				return;
			cnt += len;
		}
	}
	m_reply[cnt] = '}';
	m_comm.put(m_reply, cnt + 1);
}

void Controller::mount(req_t *req)
{
	int len;

	if (strncmp(req->buf, REQ_SECRET, strlen(REQ_SECRET)))
		return;

	sendProfile();
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

void Controller::procEvent()
{
	if(!m_mount)
		return;

	for (int i = 0; i < m_total; i++) {
		Driver *device = m_devices[i];

		if (device->m_mode & MODE_TRIG) {
			int len = device->get(getReply(device), getReplySize(device));

			if (len > 0)
				reply(device, len);
		}
	}
}

Driver *Controller::find(int index)
{
	for (int i = 0; i < m_total; i++)
		if (m_devices[i]->m_index == index)
			return m_devices[i];
	return NULL;
}

void Controller::procReq()
{
	int len;
	req_t req;
	Driver *device;

	if (receive() < 0)
		return;

	if (getRequest(&req) < 0)
		return;

	if (req.cmd & REQ_MOUNT) {
		mount(&req);
		return;
	}

	if(!m_mount)
		return;

	device = find(req.index);
	if (!device)
		return;

	if (req.cmd & REQ_OPEN) {
    	device->open();
	} else if (req.cmd & REQ_GET) {
		len = device->get(getReply(device), getReplySize(device));
		reply(device, len);
	} else if (req.cmd & REQ_PUT) {
		len = device->put(req.buf, req.len, getReply(device), getReplySize(device));
		reply(device, len);
	} else if (req.cmd & REQ_CLOSE) {
		device->close();
	}
}

void Controller::proc()
{
	procReq();
	procEvent();
}
