#pragma once

#include <string>

enum DATA_TYPE { Char, Integer, Double, String };

class Stack_Object	
{
public:
	Stack_Object();
	Stack_Object(char dato);
	Stack_Object(int dato);
	Stack_Object(double dato);
	Stack_Object(std::string dato);
	~Stack_Object();
	
	Stack_Object& operator=(const Stack_Object& so);

	DATA_TYPE tipo;	//Data type corresponding to this element.
	union			//Data stores in this element.
	{
		char Char;			//Data stored as char.
		int Int;			//Data stored as int.
		double Double;		//Data stored as double.
		std::string String;	//Data stored as string.
	};
};
