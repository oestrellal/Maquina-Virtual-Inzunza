#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Stack_Object.h"
using namespace std;

class VMInzunza
{
public:
	VMInzunza();
	~VMInzunza();
	bool load(string);
	void run();

private:
	stack<Stack_Object> stack;	//The operation stack.

	const string identifier= "UM-MEAN-C";
	string getString(unsigned int);
	int getInt(unsigned int);	//gets an int from specified data address
	int getInt();	// gets int from next 4 lines of code

	void setX(unsigned int);
	unsigned int getX();
	unsigned int getDir();

	unsigned int IP; //Instruction pointer
	unsigned int X; //offset register
	unsigned char* CS; //code segment
	unsigned char* DS; //data segment
	int flag; //value changes after cmp is casted
	
};

