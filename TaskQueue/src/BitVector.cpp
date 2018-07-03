
//

#include "BitVector.h"

BitVector::BitVector(unsigned char *baseBytePtr,
					 unsigned baseBitOffset,
					 unsigned totNumBits) {
	setup(baseBytePtr, baseBitOffset, totNumBits);
}

void BitVector::setup(unsigned char *baseBytePtr,
					  unsigned baseBitOffset,
					  unsigned totNumBits) {
	_baseBytePtr = baseBytePtr;
	_baseBitOffset = baseBitOffset;
	_totNumBits = totNumBits;
	_curBitIndex = 0;
}

static unsigned char const singleBitMask[8]
		= {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

#define MAX_LENGTH 32

void BitVector::putBits(unsigned from, unsigned numBits) {
	if (numBits == 0) return;

	unsigned char tmpBuf[4];
	unsigned overflowingBits = 0;

	if (numBits > MAX_LENGTH) {
		numBits = MAX_LENGTH;
	}

	if (numBits > _totNumBits - _curBitIndex) {
		overflowingBits = numBits - (_totNumBits - _curBitIndex);
	}

	tmpBuf[0] = (unsigned char) (from >> 24);
	tmpBuf[1] = (unsigned char) (from >> 16);
	tmpBuf[2] = (unsigned char) (from >> 8);
	tmpBuf[3] = (unsigned char) from;

	shiftBits(_baseBytePtr, _baseBitOffset + _curBitIndex, /* to */
			  tmpBuf, MAX_LENGTH - numBits, /* from */
			  numBits - overflowingBits /* num bits */);
	_curBitIndex += numBits - overflowingBits;
}

void BitVector::put1Bit(unsigned bit) {
	// The following is equivalent to "putBits(..., 1)", except faster:
	if (_curBitIndex >= _totNumBits) { /* overflow */
		return;
	} else {
		unsigned totBitOffset = _baseBitOffset + _curBitIndex++;
		unsigned char mask = singleBitMask[totBitOffset % 8];
		if (bit) {
			_baseBytePtr[totBitOffset / 8] |= mask;
		} else {
			_baseBytePtr[totBitOffset / 8] &= ~mask;
		}
	}
}

unsigned BitVector::getBits(unsigned numBits) {
	if (numBits == 0) return 0;

	unsigned char tmpBuf[4];
	unsigned overflowingBits = 0;

	if (numBits > MAX_LENGTH) {
		numBits = MAX_LENGTH;
	}

	if (numBits > _totNumBits - _curBitIndex) {
		overflowingBits = numBits - (_totNumBits - _curBitIndex);
	}

	shiftBits(tmpBuf, 0, /* to */
			  _baseBytePtr, _baseBitOffset + _curBitIndex, /* from */
			  numBits - overflowingBits /* num bits */);
	_curBitIndex += numBits - overflowingBits;

	unsigned result
			= (tmpBuf[0] << 24) | (tmpBuf[1] << 16) | (tmpBuf[2] << 8) | tmpBuf[3];
	result >>= (MAX_LENGTH - numBits); // move into low-order part of word
	result &= (0xFFFFFFFF << overflowingBits); // so any overflow bits are 0
	return result;
}

unsigned BitVector::get1Bit() {
	// The following is equivalent to "getBits(1)", except faster:

	if (_curBitIndex >= _totNumBits) { /* overflow */
		return 0;
	} else {
		unsigned totBitOffset = _baseBitOffset + _curBitIndex++;
		unsigned char curFromByte = _baseBytePtr[totBitOffset / 8];
		unsigned result = static_cast<unsigned>((curFromByte >> (7 - (totBitOffset % 8))) & 0x01);
		return result;
	}
}

void BitVector::skipBits(unsigned numBits) {
	if (numBits > _totNumBits - _curBitIndex) { /* overflow */
		_curBitIndex = _totNumBits;
	} else {
		_curBitIndex += numBits;
	}
}

unsigned BitVector::get_expGolomb() {
	unsigned numLeadingZeroBits = 0;
	unsigned codeStart = 1;

	while (get1Bit() == 0 && _curBitIndex < _totNumBits) {
		++numLeadingZeroBits;
		codeStart *= 2;
	}

	return codeStart - 1 + getBits(numLeadingZeroBits);
}


void shiftBits(unsigned char *toBasePtr, unsigned toBitOffset,
			   unsigned char const *fromBasePtr, unsigned fromBitOffset,
			   unsigned numBits) {
	if (numBits == 0) return;

	/* Note that from and to may overlap, if from>to */
	unsigned char const *fromBytePtr = fromBasePtr + fromBitOffset / 8;
	unsigned fromBitRem = fromBitOffset % 8;
	unsigned char *toBytePtr = toBasePtr + toBitOffset / 8;
	unsigned toBitRem = toBitOffset % 8;

	while (numBits-- > 0) {
		unsigned char fromBitMask = singleBitMask[fromBitRem];
		unsigned char fromBit = (*fromBytePtr) & fromBitMask;
		unsigned char toBitMask = singleBitMask[toBitRem];

		if (fromBit != 0) {
			*toBytePtr |= toBitMask;
		} else {
			*toBytePtr &= ~toBitMask;
		}

		if (++fromBitRem == 8) {
			++fromBytePtr;
			fromBitRem = 0;
		}
		if (++toBitRem == 8) {
			++toBytePtr;
			toBitRem = 0;
		}
	}
}
