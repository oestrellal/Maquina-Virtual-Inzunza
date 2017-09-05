#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
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

	//Get data from Data or Code Segment

	string getString(unsigned int);//gets a string from specified data address
	string getString();// gets string from code segment
	int getInt(unsigned int);//gets an int from specified data address
	int getInt();// gets int from next 4 lines of code
	double getDouble(unsigned int);//gets a double from specified data address
	double getDouble();// gets a double from next 8 lines of code

	//Save values into memory
	//This methods receive the value to save depending on the type od data, end the DS where they need to be saved
	void setInt(int, unsigned int dir);
	void setDouble(double, unsigned int dir);
	void setString(string, unsigned int dir);

	void setX(unsigned int);
	unsigned int getX();
	unsigned int getDir();



	unsigned int IP; //Instruction pointer
	unsigned int X; //offset register
	unsigned char* CS; //code segment
	unsigned char* DS; //data segment
	int flag; //value changes after cmp is casted
	
};

