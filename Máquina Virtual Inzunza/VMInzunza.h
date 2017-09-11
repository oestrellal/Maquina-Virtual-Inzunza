#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <Windows.h>
#include <stdio.h>
#include <string>
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

	// Error handling. Its separated in case we want to change this in our compilers.
	void reportError(string msg);

	//Get data from Data or Code Segment

	string getString(unsigned int);//gets a string from specified data address
	string getString();// gets string from code segment
	int getInt(unsigned int);//gets an int from specified data address
	int getInt();// gets int from next 4 lines of code
	double getDouble(unsigned int);//gets a double from specified data address
	double getDouble();// gets a double from next 8 lines of code

	//Save values into memory
	//These methods receive the value to save depending on the type od data, end the DS where they need to be saved
	void setInt(int, unsigned int dir);
	void setDouble(double, unsigned int dir);
	void setString(string, unsigned int dir);

	//Methods for operations.
	//These methods exists to make the operators'code clearer.
	void operationINC(unsigned int dir);
	void operationRED(unsigned int dir);
	void operationADD();
	void operationSUB();
	void operationMUL();
	void operationDIV();
	void operationMOD();
	void operationCMP();
	DATA_TYPE getOperationDataType(Stack_Object so1, Stack_Object so2);

	void setX( int);
	unsigned int getX();
	void setY(unsigned int direccion);
	unsigned int getY();

	unsigned int getDir();
	unsigned int getDir(int);




	unsigned int IP; //Instruction pointer
	unsigned int X; //offset register
	unsigned int Y; //size of strings in string array
	unsigned char* CS; //code segment
	unsigned char* DS; //data segment
	double flag; //value changes after cmp is casted
	
};

