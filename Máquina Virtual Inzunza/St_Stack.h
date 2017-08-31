#pragma once

enum DATA_TYPE
{
	Char, 
	Integer, 
	Double, 
	String
};

struct St_Stack
{
	DATA_TYPE tipo;
	union data
	{
		char ch;
		int i;
		
	};
};