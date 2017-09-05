#pragma once

#include <stack>
#include <bitset>

using namespace std;

enum DATA_TYPE { Char, Integer, Double, String };

struct Stack_Object
{
	DATA_TYPE tipo;
	bitset<256> dato;
};