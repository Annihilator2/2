#pragma once

class Array {
public:
	Array();

	Array(int N);

	Array(const Array& object);

	~Array();

	static Array empty();

	int getSize();

	int getElmNum();
	
	long long* getArr();

	short isEmpty();

	void expand();

	void redArrCap(int amount);

	void makeEqualSize(int size1, int size2);

	char* cStr();

	void addToEnd(long long value);
	
	void insertByIndex(long long value, int index);
	
	void deleteByIndex(int index);

	long long getByIndex(int index);

	long long* getSlice(int from, int to);

	void shiftSliceR(int index);

	void shiftSliceL(int index);

	short searchForElm(long long value);

	void swapArray(Array& second);

	int size;
	int elemNum;
	long long* arr;
	static const int expCoef = 2;

	void setArr(int from);

	int countDigits(long long value);

};
