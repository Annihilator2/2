#include "BigInt.h"
#include <cstdlib>
#include <iostream>

	//����������� �� ���������
	BigInt::BigInt() {
		sign = 1;
	}
	/*
	* �����������, ��������� BigInt �� ����� value:
	* 1)�������������� ���-�� �������� ����� value;
	* 2)�������������� ����������� ���-�� ����� ��� ������ � ������ Array,
	    ��� ������ ������ �������� � ���� 9 ����
	  3)����� ������������ � ������ ��������
	*/
	BigInt::BigInt(long long value) {
		sign = 1;
		if (value < 0) {
			sign = -1;
			value *= sign;
		}
		if (value == 0) {
			array.addToEnd(0);
		}
		else {
			long long digits = countDigits(value);
			long long cells = (digits % 9) == 0 ? digits / 9 : digits / 9 + 1;
			array.makeEqualSize(array.getSize(), cells);
			for (int i = cells - 1; i >= 0; i--) {
				array.insertByIndex(value % BASE, i);
				value /= BASE;
			}
		}
	}

	//����������� �����������
	BigInt::BigInt(const BigInt& object) : array(object.array) {
		sign = object.sign;
	}
	//����������
	BigInt::~BigInt() {
	}

	//��������� �����, ����������������� �������� Array
	Array BigInt::getBigInt() {
		return array;
	}
	//��������� ����� �����
	short BigInt::getSign() {
		return sign;
	}

	//�������� ���������� ������������� ������ BigInt � ���� �������� �����
	char* BigInt::cStr() {
		int size=0;
		char* cStr = array.cStr();
		for (int i = 0; cStr[i] != '\0'; i++)
			size++;
		if (sign == 1) memmove(cStr, cStr + 1, size);
		else cStr[0] = '-';
		for (int i = 0; i < size; i++) {
			if (cStr[i] == ' ' || cStr[i] == ',' || cStr[i] == ']') {
				memmove(cStr + i, cStr + i + 1, size - i);
				size--;
			}
		}
		return cStr;
	}

	//����� ��������� ���� ����� ���� BigInt:
	//���������� 2, ���� ������ > �������
	//���������� 1, ���� ���-�� ����� ������� > ���-�� ����� �������
	//���������� 0, ���� �����
	//���������� -1, ���� ���-�� ����� ������� < ���-�� ����� �������
	//���������� -2, ���� ������ < �������
	short BigInt::compareBigInt(BigInt& second) {
		short state;
		if (array.getElmNum() > second.array.getElmNum()) 
			state = 1;
		else if (array.getElmNum() == second.array.getElmNum()) {
			state = 0;
			int size1 = array.getSize();
			int size2 = second.array.getSize();
			for (int i = 0; i < size1 && (state != 2 && state != -2); i++) {
				long long curElem = array.getByIndex(i);
				long long secondElem = i < size2 ? second.array.getByIndex(i) : 0;
				if (curElem > secondElem) 
					state = 2;
				else if (curElem < secondElem) 
					state = -2;
			}
		}
		else state = -1;
		return state;
	}

	//����� ��������� �������� ���� BigInt �����
	void BigInt::equalizeRanks(BigInt& second) {
		short state = compareBigInt(second);
		if (state == 1) {
			while (second.array.getElmNum() < array.getElmNum()) {
				second.array.shiftSliceR(0);
				second.array.insertByIndex(0, 0);
			}
		}
		else if (state == -1) {
			while (array.getElmNum() < second.array.getElmNum()) {
				array.shiftSliceR(0);
				array.insertByIndex(0, 0);
			}
		}
	}

	/*
	* ����� �������� ���� BigInt �����:
	* 1)� ����������� �� ������ ����� ������� ��������� ������ ��������;
	* 2)�������� ����������� ����� �������� � �������, ��� ������ ������
	    ������������ � ����������� ���
	*/
	void BigInt::sumBigInt(BigInt& current, BigInt added) {
		long long rest = 0, result = 0;
		if (current.sign == 1 && added.sign == -1) {
			current.sign *= -1;
			subBigInt(current,added);
			current.sign *= -1;
		}
		else if (current.sign == -1 && added.sign == 1) {
			current.sign *= -1;
			subBigInt(current,added);
			current.sign *= -1;
		}
		else  {
			current.equalizeRanks(added);
			for (int i = current.array.getSize() - 1, j = i; i >= 0; i--, j--) {
				long long curElem = current.array.getByIndex(i);
				long long addedElem = j >= 0 ? added.array.getByIndex(i) : 0;
				result = curElem + addedElem + rest;
				rest = result >= BASE ? 1 : 0;
				result -= rest > 0 ? BASE : 0;
				current.array.deleteByIndex(i);
				current.array.insertByIndex(result, i);
				if (rest && current.array.getElmNum() == current.array.getSize() && i == 0) {
					current.array.shiftSliceR(i++);
					current.array.insertByIndex(0, 0);
				}
			}
		}
	}

	/*
	* ����� ��������� ���� BigInt �����:
	* 1)� ����������� �� ������ ����� ������� �������� ������ ���������
	* 2)��������� ����������� � �������, ��� ������ ������ ���������� � 
	    ����������� ���
	*/
	void BigInt::subBigInt(BigInt& current, BigInt subtracted) {
		long long rest = 0, result = 0;
		if (current.sign == -1 && subtracted.sign == 1) {
			current.sign *= -1;
			sumBigInt(current,subtracted);
			current.sign *= -1;
		}
		else if (current.sign == 1 && subtracted.sign == -1) {
			current.sign *= -1;
			sumBigInt(current,subtracted);
			current.sign *= -1;
		}
		else {
			current.equalizeRanks(subtracted);
			if (current.compareBigInt(subtracted) == -2) {
				current.swapBigInts(subtracted);
				current.sign = -1;
			}
			for (int i = current.array.getElmNum() - 1; i >= 0; i--) {
				long long curElem = current.array.getByIndex(i);
				long long subElem = subtracted.array.getByIndex(i);
				result = curElem - subElem - rest;
				rest = result < 0 ? 1 : 0;
				result += rest > 0 ? BASE : 0;
				current.array.deleteByIndex(i);
				current.array.insertByIndex(result, i);
			}
		}
	}

	/*
	* ����� ��������� ���� BigInt �����:
	* 1)� ������ ���� ���� �� ���������� ����� 0, �� �� ���������� ���������,
		����� ��������� ����� 0;
	  2)��������� ����������� � �������, ��� ������ ������ ����� ����������
		������������� �� ����� ������� ��������� ������� �����, ����� ����
		��������� �����������;
	  3)����� ��������� � ������ ����� ���������� �������� ����, ���� �������
		�������
	*/
	void BigInt::multiplyBigInt(BigInt& current, BigInt& factor) {
		BigInt bigMult;
		if (factor.array.isEmpty() || current.array.isEmpty()) {
			bigMult.array.addToEnd(0);
			current.swapBigInts(bigMult);
		}
		else {
			int thisLen = current.array.getSize();
			int factorLen = factor.array.getSize();
			long long resLen = thisLen + factorLen + 1, resMult = 0, rest = 0;
			bigMult.array.makeEqualSize(resLen, bigMult.array.getSize());
			for (int i = thisLen - 1; i >= 0; i--) {
				long long curElem = current.array.getByIndex(i);
				for (int j = factorLen - 1; j >= 0 || rest > 0; j--) {
					long long factorElem = (j >= 0) ? factor.array.getByIndex(j) : 0;
					long long resElem = bigMult.array.getByIndex(i + j + 2);
					resMult = resElem + (curElem * factorElem) + rest;
					if (resElem == 0) 
						bigMult.array.insertByIndex(resMult % BASE, i + j + 2);
					else {
						bigMult.array.deleteByIndex(i + j + 2);
						bigMult.array.insertByIndex(resMult % BASE, i + j + 2);
					}
					rest = resMult >= BASE ? resMult /= BASE : 0;
				}
			}
			while (bigMult.array.getByIndex(0) == 0) {
				bigMult.array.shiftSliceL(0);
				bigMult.array.redArrCap(1);
			}
			bigMult.sign = (current.sign == factor.sign) ? 1 : -1;
			current.swapBigInts(bigMult);
		}
	}

	/*
	* ����� ������� ���� BigInt �����:
	* 1)���� �������� ����� 0, �� ����������� ����������;
	* 2)���� �������� > ��������, �� �� ���������� �������, � ����� ���������� �
	    ��������� 0;
	  3)������� ����������� � �������: 
	    3.1)�������� ������ ��������;
	    3.2)����� ��������� ������ ����������� ����������� ������ ��������;
		3.3)�� �������� ������� �������� ���������� ��������� ������ ��������,
		    ����������� �� ��������;
		3.4)� ������������ �������� �������� ����.������ ��������
	  4)����� ������� ���������� ���������� ����� � ������, ���� ������� �������
	*/
	void BigInt::divBigInt(BigInt& current,BigInt& divr) {
		BigInt result, curDivr(divr);
		curDivr.sign = 1;
		if (curDivr.array.isEmpty())
			throw("Exception: division by zero");
		if (current.compareBigInt(curDivr) < 0) {
			result.array.addToEnd(0);
			current.swapBigInts(result);
			return;
		}
		result.array.makeEqualSize(result.array.getSize(), current.array.getSize());
		BigInt curDividend(result);
		for (int i = 0, j = 0; i < current.array.getSize(); i++, j++) {
			long long curElem = current.array.getByIndex(i);
			curDividend.array.insertByIndex(curElem, j);
			long long curFactor = 0, left = 0, right = BASE;
			while (left <= right) {
				long long mid = (left + right) / 2;
				BigInt tmp(mid);
				multiplyBigInt(tmp,curDivr);
				if (tmp.compareBigInt(curDividend) <= 0) { 
					curFactor = mid;
					left = mid + 1;
				}
				else right = mid - 1;
			}
			if (curFactor != 0) result.array.insertByIndex(curFactor, i);
			BigInt tmpFactor(curFactor);
			multiplyBigInt(tmpFactor,curDivr);
			subBigInt(curDividend,tmpFactor);
			while (curDividend.array.getByIndex(0) == 0) {
				if (curDividend.array.getElmNum() == 0) break;
				curDividend.array.deleteByIndex(0);
				curDividend.array.shiftSliceL(0);
				j--;
			}
		}
		while (result.array.getByIndex(0) == 0) result.array.shiftSliceL(0);
		result.array.redArrCap(current.array.getSize() - result.array.getElmNum());
		result.sign = (current.sign == divr.sign) ? 1 : -1;
		current.swapBigInts(result);
	}

	/*
	* ����� ������ ������ ��� ���� BigInt �����
	*/
	void BigInt::swapBigInts(BigInt& a) {
		this->array.swapArray(a.array);
		short tmpSign = sign;
		sign = a.sign;
		a.sign = tmpSign;
	}

	/*
	* ����� �������� ���-�� �������� ������������� ����� value
	*/
	int BigInt::countDigits(long long value) {
		int digits = 0;
		if (value == 0 || value < 0) digits++;
		while (value != 0) {
			digits++;
			value /= 10;
		}
		return digits;
	}