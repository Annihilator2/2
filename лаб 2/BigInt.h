#pragma once
#include "Array.h"

class BigInt {
public:
	BigInt();

	BigInt(long long value);

	BigInt(const BigInt& object);

	~BigInt();

	Array getBigInt();

	short getSign();

	char* cStr();

	short compareBigInt(BigInt& second);

	void equalizeRanks(BigInt& second);

	static void sumBigInt(BigInt& current, BigInt added);

	static void subBigInt(BigInt& current, BigInt subtracted);

	static void multiplyBigInt(BigInt& current,BigInt& factor);

	static void divBigInt(BigInt& current, BigInt& divr);

	void swapBigInts(BigInt& a);

	Array array;
	short sign;
	const static int BASE = 1000000000;

	int countDigits(long long value);
};
