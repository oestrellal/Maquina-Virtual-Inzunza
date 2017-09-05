#pragma once

#include <string>

using namespace std;

enum DATA_TYPE { Char, Integer, Double, String };

struct Stack_Object
{
	DATA_TYPE tipo;	//Data type corresponding to this element.
	union dato		//Data stores in this element.
	{
		char c;		//Data stored as char.
		int i;		//Data stored as int.
		double d;	//Data stored as double.
		string s;	//Data stored as string.
	};
};