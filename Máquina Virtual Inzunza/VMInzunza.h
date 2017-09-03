#pragma once

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class VMInzunza
{
public:
	VMInzunza();
	~VMInzunza();
	bool load(string);
	void run();

private:
	const string identifier= "UM-MEAN-C";
	void setX(int direccion);
	int getX();
	int bin2dir(string direccion);
	int IP; //Instruction pointer
	char* CS; //code segment
	char* DS; //data segment
};

