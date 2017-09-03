// Máquina Virtual Inzunza.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma region Instrucciones

#define HALT 0;		//Finish.
#define WR 1;		//Writes a newline.
#define WRC 2;		//Writes a char.
#define WRI 3;		//Writes an int.
#define WRD 4;		//Writes a double.
#define WRS 5;		//Writes a string.
#define PUSHC 6;	//Inserts a char on top of the stack.
#define PUSHI 7;	//Inserts an int on top of the stack.
#define PUSHD 8;	//Inserts a double on top of the stack.
#define PUSHS 9;	//Inserts a string on top of the stack.
#define PUSHKC 10;	//Inserts a constant char on top of the stack.
#define PUSHKI 11;	//Inserts a constant int on top of the stack.
#define PUSHKD 12;	//Inserts a constant double on top of the stack.
#define PUSHKS 13;	//Inserts a constant string on top of the stack.
#define POPC 14;	//Pops data from the stack as a char.
#define POPI 15;	//Pops data from the stack as an int.
#define POPD 16;	//Pops data from the stack as a double.
#define POPS 17;	//Pops data from the stack as a string.
#define POPX 18;	//Pops from stack to pointer register (int).
#define RDC 19;		//Reads a char into memory.
#define RDI 20;		//Reads an int into memory.
#define RDD 21;		//Reads a double into memory.
#define RDS 22;		//Reads a string into memory.
#define JMP 23;		//Jumps indonditionally to a memory address.
#define JMPE 24;	//Jumps if the top of the stack is (==) 0.
#define JMPNE 25;	//Jumps if the top of the stack is different than (!=) 0.
#define JMPG 26;	//Jumps if the top of the stack is greater than (>) 0.
#define JMPGE 27;	//Jumps if the top of the stack is greater or equal than (>=) 0.
#define JMPL 28;	//Jumps if the top of the stack is less than (<) 0.
#define JMPLE 29;	//Jumps if the top of the stack is less or equal than (<=) 0.
#define STX 30;		//Stores a variable in the pointer register.
#define STKX 31;	//Stores a constant int in the pointer register.
#define INC 32;		//Increases the value of a memory address by 1.
#define DEC 33;		//Decreases the value of a memory address by 1.
#define ADD 34;		//Pops the two topmost numbers from the stack and adds them. The result is stored in the stack.
#define SUB 35;		//Pops the two topmost numbers from the stack and subtracts them. The result is stored in the stack.
#define MUL 36;		//Pops the two topmost numbers from the stack and multiplies them. The result is stored in the stack.
#define DIV 37;		//Pops the two topmost numbers from the stack and divides them. The result is stored in the stack.
#define MOD 38;		//Pops the two topmost numbers from the stack and divides them. The remainder is stored in the stack.
#define CMP 39;		//Compares 2 stack values by subtracting them and stores the result in the stack.

#pragma endregion

#pragma region Constantes

#define MAX_STACK 500;		//Max quantity of objects that can be on the stack.

#pragma endregion

#include "VMInzunza.h"

using namespace std;
int main()
{
	VMInzunza vm;
	vm.load("C:/Users/admine0438883/Desktop/prueba");
    return 0;
}

