/*      Comm.cpp
 *
 *      Copyright (C) 2014 Yi-Wei Ci <ciyiwei@hotmail.Comm>
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

#include "Comm.h"
#include "CRC.h"
#include "Arduino.h"

Comm::Comm()
{
	m_ch = 0;
	m_count = 0;
	m_start = 0;
	m_buf = NULL;
}

void Comm::setup(char *buf, int size)
{
	m_buf = buf;
	m_size = size;
	Serial.begin(COMM_RATE);
	Serial.setTimeout(COMM_TIMEOUT);
}

int Comm::recv()
{
	char ch;
	int ret = -1;

	if (!Serial.available())
		return ret;
	
	ch = Serial.read();
	if (COMM_DLE == m_ch) {
		if (COMM_STX == ch) {
			m_count = 0;
			m_start = 1;
		} else if (COMM_ETX == ch) {
			if (m_start) {
				m_start = 0;
				if (m_count >= CRC_SIZE)
					ret = m_count;
			}
		} else if (COMM_DLE == ch) {
			if (m_start) {
				if (m_size == m_count) {
					m_start = 0;
				} else {
					m_buf[m_count] = ch;
					m_count++;
				}
			}
		}
	} else if (m_start && (ch != COMM_DLE)) {
		if (m_size == m_count) {
			m_start = 0;
		} else {
			m_buf[m_count] = ch;
			m_count++;
		}
	}
	m_ch = ch;
	return ret;
}

int Comm::get()
{
	int size = recv();

	if (size < CRC_SIZE)
		return -1;
	else {
		crc_t code;
		CRC crc = CRC();
		
		code = crc.encode(&m_buf[CRC_SIZE], size - CRC_SIZE);
		if (code == *((crc_t *)m_buf))
			return size;
		else
			return -1;
	}
}

void Comm::send(char *buf, int length)
{
	for (int i = 0; i < length; i++) {
		Serial.write(buf[i]);
		if (COMM_DLE == buf[i])
			Serial.write(COMM_DLE);
	}
}

void Comm::sendBytes(char *buf, int length)
{
	for (int i = 0; i < length; i++)
		Serial.write(buf[i]);
}

void Comm::put(char *buf, int length)
{
	CRC crc = CRC();
	crc_t code = crc.encode(buf, length);

	sendBytes(COMM_HEAD, COMM_HEAD_LEN);
	send((char *)&code, sizeof(crc_t));
	send(buf, length);
	sendBytes(COMM_TAIL, COMM_TAIL_LEN);
}
