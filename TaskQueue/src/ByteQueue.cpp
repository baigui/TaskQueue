//
// Created by 张圣辉 on 16/8/26.
//

#include "ByteQueue.h"
#include <string.h>
#include <stdlib.h>

#if defined(WIN32) || defined(WIN64) || defined(_CONSOLE)
#include <winsock2.h>
#include <stdint.h>
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#elif defined(__ANDROID__)
#include <netinet/in.h>
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif

ByteQueue::ByteQueue(long capacity) {
	_capacity = capacity + 1;
	_bytes = new unsigned char[_capacity];
	_head = 0;
	_tail = 0;
}

ByteQueue::~ByteQueue() {
	delete[] _bytes;
}


long ByteQueue::getHead(long index) {
	long head = _head + index;
	if (_head > _tail) {
		if (head >= _capacity) {
			head -= _capacity;
			if (head >= _tail) {
				return -1;
			}
		}
	} else {
		if (head >= _tail) {
			return -1;
		}
	}
	return head;
}

long ByteQueue::increaseHead(long length) {
	long head = _head + length;
	if (_head > _tail) {
		if (head >= _capacity) {
			head -= _capacity;
			if (head > _tail) {
				return -1;
			}
		}
	} else {
		if (head > _tail) {
			return -1;
		}
	}
	return head;
}

long ByteQueue::increaseTail(long length) {
	long tail = _tail + length;
	if (_tail >= _head) {
		if (tail >= _capacity) {
			tail -= _capacity;
			if (tail >= _head) {
				return -1;
			}
		}
	} else {
		if (tail >= _head) {
			return -1;
		}
	}
	return tail;
}

ByteQueue *ByteQueue::clone(const void *bytes, long length) {
	ByteQueue *byteQueue = new ByteQueue(length + 1);
	byteQueue->putBytes(bytes, length);
	return byteQueue;
}

long ByteQueue::available() {
	return _capacity - length() - 1;
}

long ByteQueue::length() {
	if (_tail >= _head) {
		return _tail - _head;
	} else {
		return _capacity + _tail - _head;
	}
}

void ByteQueue::clear() {
	_head = 0;
	_tail = 0;
}

bool ByteQueue::putByte(unsigned char aByte) {
	long tail = increaseTail(1);
	if (tail != -1) {
		_bytes[_tail] = aByte;
		_tail = tail;
		return true;
	} else {
		return false;
	}
}

bool ByteQueue::putBytes(const void *bytes, long length) {
	if (length == 0) return true;
	long tail = increaseTail(length);
	if (tail != -1) {
		if (tail > _tail || tail == 0) {
			memcpy(_bytes + _tail, bytes, static_cast<size_t>(length));
		} else {
			long tailLen = _capacity - _tail;
			memcpy(_bytes + _tail, bytes, static_cast<size_t>(tailLen));
			memcpy(_bytes, reinterpret_cast<const unsigned char *>(bytes) + tailLen, static_cast<size_t>(length - tailLen));
		}
		_tail = tail;
		return true;
	} else {
		return false;
	}
}

int ByteQueue::getByte() {
	int aByte = peekByte();
	if (aByte != -1) {
		_head = increaseHead(1);
	}
	return aByte;
}

bool ByteQueue::getBytes(void *dst, long length) {
	if (peekBytes(dst, length)) {
		_head = increaseHead(length);
		return true;
	} else {
		return false;
	}
}

void ByteQueue::getAll(void *dst) {
	peekAll(dst);
	_head = _tail;
}

int ByteQueue::peekAt(long offset) {
	long head = getHead(offset);
	if (head != -1) {
		return static_cast<unsigned char>(_bytes[head]);
	} else {
		return -1;
	}
}

int ByteQueue::peekByte() {
	return peekAt(0);
}

bool ByteQueue::peekBytes(void *dst, long length) {
	if (length == 0) return true;
	long head = increaseHead(length);
	if (head != -1) {
		if (head > _head || head == 0) {
			memcpy(dst, _bytes + _head, static_cast<size_t>(length));
		} else {
			long tailLen = _capacity - _head;
			memcpy(dst, _bytes + _head, static_cast<size_t>(tailLen));
			memcpy(reinterpret_cast<unsigned char *>(dst) + tailLen, _bytes, static_cast<size_t>(length - tailLen));
		}
		return true;
	} else {
		return false;
	}
}

void ByteQueue::peekAll(void *dst) {
	peekBytes(dst, length());
}

void ByteQueue::skip(long length) {
	long head = increaseHead(length);
	if (head != -1) {
		_head = head;
	} else {
		_head = _tail;
	}
}

const unsigned char *ByteQueue::dataBuffer() {
	return _bytes + _head;
}

long ByteQueue::dataBufferLength() {
	if (_tail >= _head) {
		return _tail - _head;
	} else {
		return _capacity - _head;
	}
}

char ByteQueue::readChar() {
	return static_cast<char>(getByte());
}

unsigned char ByteQueue::readUChar() {
	return static_cast<unsigned char>(getByte());
}

short ByteQueue::readShort() {
    short aShort;
    getBytes(&aShort, 2);
    return ntohs(aShort);
}

unsigned short ByteQueue::readUShort() {
    unsigned short aUShort;
    getBytes(&aUShort, 2);
    return ntohs(aUShort);
}

int ByteQueue::readInt() {
    int aInt;
    getBytes(&aInt, 4);
    return ntohl(aInt);
}

unsigned int ByteQueue::readUInt() {
    unsigned int aUInt;
    getBytes(&aUInt, 4);
    return ntohl(aUInt);
}

long long ByteQueue::readLong() {
    long long aLong;
    getBytes(&aLong, 8);
    return ntohll(aLong);
}

unsigned long long ByteQueue::readULong() {
    unsigned long long aULong;
    getBytes(&aULong, 8);
    return ntohll(aULong);
}

bool ByteQueue::writeChar(char aChar) {
	return putByte(static_cast<unsigned char>(aChar));
}

bool ByteQueue::writeUChar(unsigned char aUChar) {
	return putByte(aUChar);
}

bool ByteQueue::writeShort(short aShort) {
	aShort = htons(aShort);
	return putBytes(&aShort, 2);
}

bool ByteQueue::writeUShort(unsigned short aUShort) {
	aUShort = htons(aUShort);
	return putBytes(&aUShort, 2);
}

bool ByteQueue::writeInt(int aInt) {
	aInt = htonl(aInt);
    return putBytes(&aInt, 4);
}

bool ByteQueue::writeUInt(unsigned int aUInt) {
	aUInt = htonl(aUInt);
    return putBytes(&aUInt, 4);
}

bool ByteQueue::writeLong(long long aLong) {
	aLong = htonll(aLong);
    return putBytes(&aLong, 8);
}

bool ByteQueue::writeULong(unsigned long long aULong) {
	aULong = htonll(aULong);
    return putBytes(&aULong, 8);
}

bool ByteQueue::writeString(const char *str) {
	return putBytes(str, strlen(str) + 1);
}
