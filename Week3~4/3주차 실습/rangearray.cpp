#include <iostream>
using namespace std;
#include "RangeArray.h"

RangeArray::RangeArray(int i,int j):Array(j-i+1)
{
	low = i;
	high = j; 
}

RangeArray::~RangeArray()
{
    //아무것도 하지 않으면 파생 클래스 파괴자 -> 기본 클래스 파괴자의 순서로 호출됨.
}

int RangeArray::baseValue()
{
	return low;
}
int RangeArray::endValue()
{
	return high;
}
int& RangeArray::operator[](int i)
{
	return Array::operator[](i-low);
}
int RangeArray::operator[](int i) const
{	
	return Array::operator[](i-low);
}
