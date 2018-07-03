//
// Created by 张圣辉 on 16/10/10.
//

#ifndef PROJECT_BIT_VECTOR_HH
#define PROJECT_BIT_VECTOR_HH

class BitVector {
public:
	BitVector(unsigned char *baseBytePtr,
			  unsigned baseBitOffset,
			  unsigned totNumBits);

	void setup(unsigned char *baseBytePtr,
			   unsigned baseBitOffset,
			   unsigned totNumBits);

	void putBits(unsigned from, unsigned numBits); // "numBits" <= 32
	void put1Bit(unsigned bit);

	unsigned getBits(unsigned numBits); // "numBits" <= 32
	unsigned get1Bit();

	bool get1BitBool() { return get1Bit() != 0; }

	void skipBits(unsigned numBits);

	unsigned curBitIndex() const { return _curBitIndex; }

	unsigned totNumBits() const { return _totNumBits; }

	unsigned numBitsRemaining() const { return _totNumBits - _curBitIndex; }

	unsigned get_expGolomb();
	// Returns the value of the next bits, assuming that they were encoded using an exponential-Golomb code of order 0

private:
	unsigned char *_baseBytePtr;
	unsigned _baseBitOffset;
	unsigned _totNumBits;
	unsigned _curBitIndex;
};

// A general bit copy operation:
void shiftBits(unsigned char *toBasePtr, unsigned toBitOffset,
			   unsigned char const *fromBasePtr, unsigned fromBitOffset,
			   unsigned numBits);

#endif
