/*
 * FPGAMgrMsg.cpp
 *
 *  Created on: Nov 20, 2017
 *      Author: ballance
 */

#include "FPGAMgrMsg.h"
#include <string.h>
#include <stdio.h>

FPGAMgrMsg::FPGAMgrMsg() {
	m_get_idx = 0;
	m_put_idx = 0;
	m_max = 1024;
	m_data = new uint8_t[1024];
}

FPGAMgrMsg::FPGAMgrMsg(const FPGAMgrMsg &other) {
	m_get_idx = other.m_get_idx;
	m_put_idx = other.m_put_idx;
	m_max = other.m_max;
	m_data = new uint8_t[m_max];
	memcpy(m_data, other.m_data, m_max);
}

FPGAMgrMsg::~FPGAMgrMsg() {
	if (m_data) {
		delete [] m_data;
	}
}

void FPGAMgrMsg::operator = (const FPGAMgrMsg &rhs) {
	if (m_data) {
		delete [] m_data;
	}

	m_data = new uint8_t[rhs.m_max];
	m_max = rhs.m_max;
	memcpy(m_data, rhs.m_data, rhs.m_max);
	m_get_idx = rhs.m_get_idx;
	m_put_idx = rhs.m_put_idx;
}

void FPGAMgrMsg::reset_get() {
	m_get_idx = 0;
}

void FPGAMgrMsg::reset_put() {
	m_put_idx = 0;
}

uint8_t FPGAMgrMsg::get8() {
	if (m_get_idx < m_put_idx) {
		return get8(m_get_idx++);
	} else {
		return 0;
	}
}

uint8_t FPGAMgrMsg::get8(uint32_t idx) {
	return m_data[idx];
}

uint32_t FPGAMgrMsg::get32() {
	if (m_get_idx+4 <= m_put_idx) {
		uint32_t v = get32(m_get_idx);
		m_get_idx += 4;
		return v;
	} else {
		return 0;
	}
}

uint32_t FPGAMgrMsg::get32(uint32_t idx) {
	return (
			m_data[idx] 		|
			m_data[idx+1] << 8 	|
			m_data[idx+2] << 16 |
			m_data[idx+3] << 24
			);
}

void FPGAMgrMsg::put8(uint8_t v) {
	if (m_put_idx >= m_max) {
		uint8_t *t = m_data;
		m_max += 1024;
		m_data = new uint8_t[m_max];
		memcpy(m_data, t, m_put_idx);
		delete [] t;
	}
	put8(v, m_put_idx++);
}

void FPGAMgrMsg::put8(uint8_t v, uint32_t idx) {
	m_data[idx] = v;
}

void FPGAMgrMsg::put32(uint32_t v) {
	if (m_put_idx+4 > m_max) {
		uint8_t *t = m_data;
		m_max += 1024;
		m_data = new uint8_t[m_max];
		memcpy(m_data, t, m_put_idx);
		delete [] t;
	}
	put32(v, m_put_idx);
	m_put_idx += 4;
}

void FPGAMgrMsg::put32(uint32_t v, uint32_t idx) {
	m_data[idx]   = ((v >> 0) & 0xFF);
	m_data[idx+1] = ((v >> 8) & 0xFF);
	m_data[idx+2] = ((v >> 16) & 0xFF);
	m_data[idx+3] = ((v >> 24) & 0xFF);
}

void FPGAMgrMsg::write(void *p, uint32_t sz) {
	if (m_put_idx+sz > m_max) {
		uint8_t *t = m_data;
		m_max = ((((m_put_idx+sz-1)/1024)+1) * 1024);
		m_data = new uint8_t[m_max];
		memcpy(m_data, t, m_put_idx);
		delete [] t;
	}
	memcpy(&m_data[m_put_idx], (uint8_t *)p, sz);
	m_put_idx += sz;
}

void FPGAMgrMsg::read(void *p, uint32_t sz) {
	if (m_get_idx+sz <= m_put_idx) {
		memcpy(p, &m_data[m_get_idx], sz);
		m_get_idx += sz;
	}
}

void FPGAMgrMsg::dump() {
	const int rows = 16;
	for (int i=0; i<(m_put_idx-m_get_idx); i++) {
		if (i && !(i%rows)) {
			fprintf(stdout, "\n");
		}
		fprintf(stdout, "%02x ", m_data[m_get_idx+i]);
	}
	fprintf(stdout, "\n");
}
