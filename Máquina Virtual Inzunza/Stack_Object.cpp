#include "stdafx.h"
#include "Stack_Object.h"

// Constructor default.
Stack_Object::Stack_Object()
{
}

// Constructor; Asigna el tipo de dato del elemento como char y guarda el dato en la union.
Stack_Object::Stack_Object(char dato)
{
	tipo = DATA_TYPE::Char;
	Char = dato;
}

// Constructor; Asigna el tipo de dato del elemento como int y guarda el dato en la union.
Stack_Object::Stack_Object(int dato)
{
	tipo = DATA_TYPE::Integer;
	Int = dato;
}

// Constructor; Asigna el tipo de dato del elemento como double y guarda el dato en la union.
Stack_Object::Stack_Object(double dato)
{
	tipo = DATA_TYPE::Double;
	this->Double = dato;
}

// Constructor; Asigna el tipo de dato del elemento como string y guarda el dato en la union.
Stack_Object::Stack_Object(std::string dato)
{
	tipo = DATA_TYPE::String;
	String = dato;
}


Stack_Object::~Stack_Object()
{
	// Me da flojera pensar cómo se tenía que programar ésto xd.
}

Stack_Object & Stack_Object::operator=(const Stack_Object & so)
{
	this->tipo = so.tipo;
	switch (so.tipo)
	{
	case DATA_TYPE::Char:
		this->Char = so.Char;
		break;
	case DATA_TYPE::Integer:
		this->Int = so.Int;
		break;
	case DATA_TYPE::Double:
		this->Double = so.Double;
		break;
	case DATA_TYPE::String:
		this->String = so.String;
		break;
	default:
		break;
	}

	return *this;
}
