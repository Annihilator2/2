#include "BigInt.h"
#include <iostream>

using namespace std;

void main() {
	long long one = 1; //-8446744073709551617
	long long two = 3;
	BigInt first(one);
	BigInt second(two);
	BigInt th(8000000000000000000);
	cout << first.cStr() << endl;
	BigInt::divBigInt(first, th);
	cout << first.cStr() << endl;
	Array aboba = second.getBigInt();
	cout << aboba.cStr();
	
}