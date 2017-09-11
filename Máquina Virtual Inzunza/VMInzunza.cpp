#include "stdafx.h"
#include "VMInzunza.h"
#include <string>
#include <iostream>

using namespace std;

#pragma region Instrucciones

#define HALT 0		//Finish.
#define WR 1		//Writes a newline.
#define WRC 2		//Writes a char.
#define WRI 3		//Writes an int.
#define WRD 4		//Writes a double.
#define WRS 5		//Writes a string.
#define WRAC 6		//
#define WRAI 7		//
#define WRAD 8		//
#define WRAS 9		//
#define WRKS 10
#define PUSHC 11	//Inserts a char on top of the stack.
#define PUSHI 12	//Inserts an int on top of the stack.
#define PUSHD 13	//Inserts a double on top of the stack.
#define PUSHS 14	//Inserts a string on top of the stack.
#define PUSHKC 15	//Inserts a constant char on top of the stack.
#define PUSHKI 16	//Inserts a constant int on top of the stack.
#define PUSHKD 17	//Inserts a constant double on top of the stack.
#define PUSHKS 18	//Inserts a constant string on top of the stack.
#define PUSHAC 19	//Inserts a char from an array on top of the stack.
#define PUSHAI 20	//Inserts an int from an array on top of the stack.
#define PUSHAD 21	//Inserts a double from an array on top of the stack.
#define PUSHAS 22	//Inserts a string from an array on top of the stack.
#define POPC 23		//Pops data from the stack as a char.
#define POPI 24		//Pops data from the stack as an int.
#define POPD 25		//Pops data from the stack as a double.
#define POPS 26		//Pops data from the stack as a string.
#define POPAC 27	//Pops data from the stack as a char using the offset register.
#define POPAI 28	//Pops data from the stack as a int using the offset register.
#define POPAD 29	//Pops data from the stack as a double using the offset register.
#define POPAS 30	//Pops data from the stack as a string using the offset register.
#define POPX 31		//Pops from stack to pointer register (int).
#define RDC 32		//Reads a char into memory.
#define RDI 33		//Reads an int into memory.
#define RDD 34		//Reads a double into memory.
#define RDS 35		//Reads a string into memory.
#define RDAC 36		//Reads a char into an array using the offset register.
#define RDAI 37		//Reads a int into an array using the offset register.
#define RDAD 38		//Reads a double into an array using the offset register.
#define RDAS 39		//Reads a string into an array using the offset register.
#define JMP 40		//Jumps indonditionally to a memory address.
#define JEQ 41		//Jumps if the top of the stack is (==) 0.
#define JNE 42		//Jumps if the top of the stack is different than (!=) 0.
#define JGT 43		//Jumps if the top of the stack is greater than (>) 0.
#define JGE 44		//Jumps if the top of the stack is greater or equal than (>=) 0.
#define JLT 45		//Jumps if the top of the stack is less than (<) 0.
#define JLE 46		//Jumps if the top of the stack is less or equal than (<=) 0.
#define STX 47		//Stores a variable in the pointer register.
#define STKX 48		//Stores a constant int in the pointer register.
#define STY 49		//
#define	STKY 50		//
#define INC 51		//Increases the value of a memory address by 1.
#define RED 52		//Decreases the value of a memory address by 1.
#define ADD 53		//Pops the two topmost numbers from the stack and adds them. The result is stored in the stack.
#define SUB 54		//Pops the two topmost numbers from the stack and subtracts them. The result is stored in the stack.
#define MUL 55		//Pops the two topmost numbers from the stack and multiplies them. The result is stored in the stack.
#define DIV 56		//Pops the two topmost numbers from the stack and divides them. The result is stored in the stack.
#define MOD 57		//Pops the two topmost numbers from the stack and divides them. The remainder is stored in the stack.
#define CMP 58		//Compares 2 stack values by subtracting them and stores the result in the stack.
#pragma endregion Mapa de Instrucciones a Byte

#pragma region Constantes

#define MAX_STACK 500		//Max quantity of objects that can be on the stack.
#define SIZEOFCHAR 1		//Size in bytes of a char.
#define SIZEOFINT 4			//Size in bytes of an int.
#define SIZEOFDOUBLE 8		//Size in bytes of a double.

#pragma endregion

VMInzunza::VMInzunza()
{
}


VMInzunza::~VMInzunza()
{
}
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		printf("Ctrl-C event\n\n");
		Beep(750, 300);
		return(TRUE);

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT:
		Beep(600, 200);
		printf("Ctrl-Close event\n\n");
		return(TRUE);

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT:
		Beep(900, 200);
		printf("Ctrl-Break event\n\n");
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		Beep(1000, 200);
		printf("Ctrl-Logoff event\n\n");
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		Beep(750, 500);
		printf("Ctrl-Shutdown event\n\n");
		return FALSE;

	default:
		return FALSE;
	}
}

//Encargado de leer archivo, verificar identificador, cardar porción de código y datos, e iniciar IP en 0
bool VMInzunza::load(string dir)
{
	ifstream infile;
	size_t length = 0;

	infile.open(dir, ios::in | ios::binary);

	infile.seekg(0, infile.end);
	length = infile.tellg();
	infile.seekg(0, infile.beg);

	if (length < 13) {
		cout << "FATAL ERROR: Min file size is 13 bytes" << endl;
		return true;
		//error Minimum file size is 13
	}

	vector<unsigned char> oData(length);
	infile.read((char*)&oData[0], length);

	int i = 0;


	//verificar identificador
	for (i = 0; i < identifier.length(); i++) {
		if (i < length) {
			if (identifier[i] != oData[i]) {
				//error: no es el identificador
				cout << "[FATAL ERROR] Incorrect file type" << endl;
				return true;
			}
		}
		else {
			//error: longitud del archivo menor al identificador
		}
	}
	union {
		unsigned int value;
		char byte[4];
	} sizeCode;
	union {
		unsigned int value;
		char byte[4];
	} sizeData;
	//asignar memoria para codigo
	sizeCode.byte[0] = oData[i+1];
	sizeCode.byte[1]= oData[i ];
	sizeCode.byte[2] = 0;
	sizeCode.byte[3] = 0;

	i += 2;
	//asignar memoria para datos
	sizeData.byte[0] = oData[i+1];
	sizeData.byte[1] = oData[i ];
	sizeData.byte[2] = 0;
	sizeData.byte[3] = 0;
	i += 2;

	//error: check why ó is 242 instead of 168
	CS = new unsigned char[sizeCode.value];
	DS = new unsigned char[sizeData.value];

	//copy the code
	int start = i;
	for (i; (i < start + sizeCode.value && i < oData.size()); i++) {
		CS[i - start] = oData[i];
	}

	//start Instruction pointer
	IP = 0;
	return false;
}

void VMInzunza::run()
{
	//hERE WE CAN INPUT TEST CODES
	/*
	//Code Segment
	CS[0] = WR;
	CS[1] = WRC;
	CS[2] = '\0';
	CS[3] = '\0';
	CS[4] = WR;
	CS[5] = WRI;
	CS[6] = '\0';
	CS[7] = '\0';
	CS[8] = WR;
	CS[9] = WRS;
	CS[10] = '\0';
	CS[11] = '\0';
	CS[12] = WR;
	CS[13] = RDC;
	CS[14] = '\0';
	CS[15] = '\0';
	CS[16] = RDI;
	CS[17] = '\0';
	CS[18] = '\1';
	CS[19] = RDS;
	CS[20] = '\0';
	CS[21] = '\5';
	CS[22] = WR;
	CS[23] = WRC;
	CS[24] = '\0';
	CS[25] = '\0';
	CS[26] = WR;
	CS[27] = WRI;
	CS[28] = '\0';
	CS[29] = '\1';
	CS[30] = WR;
	CS[31] = WRS;
	CS[32] = '\0';
	CS[33] = '\5';
	CS[34] = WR;
	CS[35] = HALT;
	*/
	//Data Segment
	DS[0] = 'U';
	DS[1] = 'n';
	DS[2] = 'i';
	DS[3] = 't';
	DS[4] = 'e';
	DS[5] = 'd';
	DS[6] = ' ';
	DS[7] = 'W';
	DS[8] = 'e';
	DS[9] = ' ';
	DS[10] = 'S';
	DS[11] = 't';
	DS[12] = 'a';
	DS[13] = 'n';
	DS[14] = 'd';
	DS[15] = '\0';
	

	unsigned int dir;
	unsigned char charContainer;
	int intContainer;
	double doubleContainer;
	bool correctEnd = false;
	string stringContainer;
	Stack_Object *nuevo;		// Elemento que se va a agregar al stack;

	//Loop
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
		cout << "Error: couldnt load ctrl handler" << endl;
		return;
	}

	while (CS[IP] != '\0') {
		switch (CS[IP])
		{
#pragma region Halt

		case HALT:
			break;

#pragma endregion

#pragma region Write

		case WR:
			std::cout << endl;
			IP++;
			break;
		case WRC:
			IP++;
			dir = getDir();
			std::cout << (char)DS[dir];
			break;
		case WRI:
			IP++;
			dir = getDir();
			intContainer = getInt(dir);
			std::cout << to_string(intContainer);
			intContainer = 0;
			break;
		case WRD:
			IP++;
			dir = getDir();
			doubleContainer = getDouble(dir);
			
			std::cout << to_string(doubleContainer);
			doubleContainer = 0;
			break;
		case WRS:
			IP++;
			dir = getDir();
			stringContainer = getString(dir);
			cout << stringContainer;
			stringContainer = '\0';
			break;
		case WRKS:
			IP++;
			charContainer = CS[IP];
			IP++;
			unsigned char stringComponent;
			for (unsigned int i = 0; i < (unsigned int)charContainer; i++) {
				stringComponent = CS[IP];
				IP++;
				cout << stringComponent;
			}
			stringContainer = '\0';
			break;

#pragma endregion
			/*
#pragma region Push
		case PUSHC:
			IP++;
			dir = getDir();
			charContainer = DS[dir];		//CHECAR
			nuevo = new Stack_Object(charContainer);
			this->stack.push(*nuevo);
			break;
		case PUSHI:
			IP++;
			dir = getDir();
			intContainer = DS[dir];		//CHECAR
			nuevo = new Stack_Object(intContainer);
			this->stack.push(*nuevo);
			break;
		case PUSHD:
			IP++;
			dir = getDir();
			doubleContainer = DS[dir];		//CHECAR
			nuevo = new Stack_Object(doubleContainer);
			this->stack.push(*nuevo);
			break;
		case PUSHS:
			IP++;
			dir = getDir();
			stringContainer = DS[dir];		//CHECAR
			nuevo = new Stack_Object(stringContainer);
			this->stack.push(*nuevo);
			break;
		case PUSHAC:
			IP++;
			dir = getDir();
			charContainer = DS[dir + getX() * SIZEOFCHAR];		//CHECAR
			nuevo = new Stack_Object(charContainer);
			this->stack.push(*nuevo);
			break;
		case PUSHAI:
			IP++;
			dir = getDir();
			intContainer = DS[dir + getX() * SIZEOFINT];		//CHECAR
			nuevo = new Stack_Object(intContainer);
			this->stack.push(*nuevo);
			break;
		case PUSHAD:
			IP++;
			dir = getDir();
			doubleContainer = DS[dir + getX() * SIZEOFDOUBLE];		//CHECAR
			nuevo = new Stack_Object(doubleContainer);
			this->stack.push(*nuevo);
			break;*/
		case PUSHAS:
			// PENDIENTE
			break;
#pragma endregion

#pragma region Pop
		case POPC:
			IP++;
			dir = getDir();
			charContainer=this->stack.pop().Char;
			DS[ dir]=charContainer;
			break;
		case POPI:
			IP++;
			dir = getDir();
			intContainer = (this->stack.pop())->Int;
			setInt( intContainer, dir);
			break;
		case POPD:
			IP++;
			dir = getDir();
			doubleContainer = (this->stack.pop())->Double;
			setDouble(doubleContainer, dir);
			break;
		case POPS:
			IP++;
			dir = getDir();
			stringContainer = (this->stack.pop())->String;
			setString(stringContainer, dir);
			break;
		case POPAC:
			IP++;
			dir = getDir();
			charContainer = this->stack.pop.Char;
			DS[dir + getX()] = charContainer;
			break;
		case POPAI:
			IP++;
			dir = getDir();
			intContainer = this->stack.pop.Int;
			DS[dir + (getX()*4)] = intContainer;
			break;
		case POPAD:
			IP++;
			dir = getDir();
			doubleContainer = (this->stack.pop()).Double;
			DS[dir + getX()*8] = doubleContainer;
			break;
		case POPAS:
			IP++;
			dir = getDir();
			stringContainer = (this->stack.pop()).String;
			int i;
			for (i = 0; i < stringContainer.length; i++)
			{
				DS[dir + i] = stringContainer[i];
			}//for
			DS[dir + i] = '\0';
			break;
		case POPX:
			IP++;
			intContainer = (this->stack.pop()).Int;
			setX(intContainer);
			break;
#pragma endregion

#pragma region Read

		case RDC:
			IP++;
			dir = getDir();
			cin >> charContainer;
			DS[dir] = charContainer;
			break;
		case RDI:
			IP++;
			dir = getDir();
			while (!correctEnd) {
				try {
					getline(cin, stringContainer);
					intContainer = stoi(stringContainer);
					correctEnd = true;
				}
				catch (std::exception const & e) {
					//cout << "error: " << e.what() << endl;
					cout << "error: Not an integer.Try again" << endl;
				}
			}
			correctEnd = false;
			setInt(intContainer,dir);
			break;
		case RDD:
			IP++;
			dir = getDir();
			while (!correctEnd) {
				try {
					getline(cin, stringContainer);
					doubleContainer = stod(stringContainer);
					correctEnd = true;
				}
				catch (std::exception const & e) {
					//cout << "error: " << e.what() << endl;
					cout << "error: Not a double.Try again" << endl;
				}
			}
			correctEnd = false;
			setDouble(doubleContainer,dir);
			break;
		case RDS:
			IP++;
			dir = getDir();
			getline(cin, stringContainer);
			setString(stringContainer, dir);
			break;
			
		case RDAC:
			IP++;
			dir = getDir();
			cin >> charContainer;
			DS[dir+getX()] = charContainer;
			break;
		case RDAI:
			IP++;
			dir = getDir();
			cin >> intContainer;
			dir=dir+getX()*4;
			while (!correctEnd) {
				try {
					getline(cin, stringContainer);
					intContainer = stoi(stringContainer);
					correctEnd = true;
				}
				catch (std::exception const & e) {
					//cout << "error: " << e.what() << endl;
					cout << "error: Not an integer.Try again" << endl;
				}
			}
			correctEnd = false;
			setInt(intContainer,dir);
			break;
		case RDAD:
			IP++;
			dir = getDir();
			while (!correctEnd) {
				try {
					getline(cin, stringContainer);
					doubleContainer = stod(stringContainer);
					correctEnd = true;
				}
				catch (std::exception const & e) {
					//cout << "error: " << e.what() << endl;
					cout << "error: Not a double.Try again" << endl;
				}
			}
			correctEnd = false;			
			dir = dir + getX()*8;
			setDouble(doubleContainer,dir);
			break;
		case RDAS:
			IP++;
			dir = getDir();
			intContainer = getDir();
			getline(cin, stringContainer);
			dir = dir + getX()*intContainer;
			setString(stringContainer, dir);
			break;

#pragma endregion

#pragma region Offset Register Manipulation

		case STX:
			IP++;
			dir = getDir();
			intContainer = getInt(dir);
			if (intContainer < 0) {
				cout << "[FATAL ERROR] Index lower than 0" << endl;
				return;
			}
			setX(intContainer);
			break;
		case STKX:
			IP++;
			intContainer = getInt();
			if (intContainer < 0) {
				cout << "[FATAL ERROR] Index lower than 0" << endl;
				return;
			}
			setX(intContainer);
			break;

#pragma endregion

#pragma region Operators
		case INC:

			break;
		case RED:
			break;
		case ADD:
			IP++;
			//operationADD();
			break;
		case SUB:
			IP++;
			operationSUB();
			break;
		case MUL:
			break;
		case DIV:
			break;
		case MOD:
			break;
		case CMP:
			IP++;
			operationCMP();
			break;
#pragma endregion

#pragma region Jumps

		case JMP:
			IP++;
			dir = getDir();
			IP = dir;
			break;
		case JEQ:
			IP++;
			dir = getDir();
			if (flag == 0)
				IP = dir;
			break;
		case JNE:
			IP++;
			dir = getDir();
			if (flag != 0)
				IP = dir;
			break;
		case JGT:
			IP++;
			dir = getDir();
			if (flag > 0)
				IP = dir;
			break;
		case JGE:
			IP++;
			dir = getDir();
			if (flag >= 0)
				IP = dir;
			break;
		case JLT:
			IP++;
			dir = getDir();
			if (flag < 0)
				IP = dir;
			break;
		case JLE:
			IP++;
			dir = getDir();
			if (flag <= 0)
				IP = dir;
			break;

#pragma endregion
	
		}
	}
	std::cout << "Halted!" << endl;
}

#pragma region Getters

//gets a direction of memory based on the next 2 bytes in code segment and moves the IP
unsigned int VMInzunza::getDir() {
	union {
		unsigned int value;
		unsigned char byte[4];
	} u;
	u.byte[0] = CS[IP+1];
	u.byte[1]= CS[IP];
	u.byte[2] = '\0';
	u.byte[3] = 0;
	IP += 2;
	return u.value;
}

int VMInzunza::getInt(unsigned int dir) {

	union {
		int value;
		char bytes[sizeof(int)];
	} u;
	for (int i = 0; i < 4; i++) {
		u.bytes[i] = DS[dir + 3-i];
	}
	return u.value;

}
int VMInzunza::getInt() {

	union {
		int value;
		char bytes[sizeof(int)];
	} u;
	for (int i = 0; i < 4; i++) {
		u.bytes[i] = CS[IP + 3-i];
	}
	IP += 4;

	return u.value;


}

double VMInzunza::getDouble(unsigned int dir) {

	union {
		double value;
		char bytes[sizeof(double)];
	} u;
	for (int i = 0; i < 8; i++) {
		u.bytes[i] = DS[dir + 7-i];
	}
	return u.value;

}

double VMInzunza::getDouble() {

	union {
		double value;
		char bytes[sizeof(double)];
	} u;
	for (int i = 0; i < 8; i++) {
		u.bytes[i] = CS[IP +7- i];
	}
	IP += 8;
	return u.value;

}

void VMInzunza::reportError(string msg)
{
	cout << msg << endl;
}

//Gets a string until it finds the null char
string VMInzunza::getString(unsigned int dir) {
	unsigned int next = dir;
	string value = "";
	while (DS[next] != '\0') {
		value += DS[next];
		next++;
	}
	return value;
}
//gets a string until it finds the null char
string VMInzunza::getString() {
	unsigned int next = IP;
	string value = "";
	while (DS[next] != '\0') {
		value += DS[next];
		next++;
		IP++;
	}
	IP++;
	return value;
}

// returns the offset register value
unsigned int VMInzunza::getX()
{
	return X;
}

// returns the offset register value
unsigned int VMInzunza::getY()
{
	return Y;
}

#pragma endregion

#pragma region Setters

void VMInzunza::setInt(int toSave, unsigned int dir)
{
	union
	{
		int value;
		char bytes[sizeof(int)];
	} u;

	u.value = toSave;

	for (int i = 0; i < 4; i++) {
		DS[dir + i] = u.bytes[3-i];
	}	
}

void VMInzunza::setDouble(double toSave, unsigned int dir)
{
	union
	{
		double value;
		char bytes[sizeof(int)];
	} u;
	u.value = toSave;
	for (int i = 0; i < 8; i++) {
		DS[dir + i] = u.bytes[7-i];
	}
}
void VMInzunza::setString(string toSave, unsigned int dir)
{
	int i = 0;
	for (i; i < toSave.length(); i++)
	DS[dir + i] = toSave[i];
	DS[dir + i] = '\0';
}

//validates that the offset register is valid
void VMInzunza::setX(unsigned int direccion)
{
	X = direccion;
}

//validates that the offset register is valid
void VMInzunza::setY(unsigned int direccion)
{
	Y = direccion;
}

#pragma endregion

#pragma region Operator helper functions

// Method that corresponds to the operator INC.
void VMInzunza::operationINC(unsigned int dir)
{
}

// Method that corresponds to the operator RED.
void VMInzunza::operationRED(unsigned int dir)
{
}

// Method that corresponds to the smart operator ADD.
void VMInzunza::operationADD()
{
	char c;
	int i;
	double d;
	string s;
	Stack_Object so1, so2, *nuevo;

	so2 = stack.top();
	stack.pop();
	so1 = stack.top();
	stack.pop();

	switch (so1.Char)
	{
	case Char:
		switch (so2.tipo)
		{
		case Char:
			c = so1.Char + so2.Char;
			nuevo = new Stack_Object(c);
			break;
		case Integer:
			i = (int)so1.Char + so2.Int;
			nuevo = new Stack_Object(i);
			break;
		case Double:
			d = (double)so1.Char + so2.Double;
			nuevo = new Stack_Object(d);
			break;
		case String:
			s = so1.Char;
			s.append(so2.String);
			nuevo = new Stack_Object(s);
			break;
		default:
			reportError("ERROR INESPERADO EN ADD!!!!1!");
			break;
		}
		break;
	case Integer:
		switch (so2.tipo)
		{
		case Char:
			i = so1.Int + (int)so2.Char;
			nuevo = new Stack_Object(i);
			break;
		case Integer:
			i = so1.Int + so2.Int;
			nuevo = new Stack_Object(i);
			break;
		case Double:
			d = (double)so1.Int + so2.Double;
			nuevo = new Stack_Object(d);
			break;
		case String:
			s = so1.Int;
			s.append(so2.String);
			nuevo = new Stack_Object(s);
			break;
		default:
			reportError("ERROR INESPERADO EN ADD!!!!1!");
			break;
		}
		break;
	case Double:
		switch (so2.tipo)
		{
		case Char:
			d = so1.Double + (double)so2.Char;
			nuevo = new Stack_Object(d);
			break;
		case Integer:
			d = so1.Double + so2.Int;
			nuevo = new Stack_Object(d);
			break;
		case Double:
			d = so1.Double + so2.Double;
			nuevo = new Stack_Object(d);
			break;
		case String:
			s = so1.Double;
			s.append(so2.String);
			nuevo = new Stack_Object(s);
			break;
		default:
			reportError("ERROR INESPERADO EN ADD!!!!1!");
			break;
		}
		break;
	case String:
		switch (so2.tipo)
		{
		case Char:
			s = so1.Char;
			break;
		case Integer:
			s = so1.Int;
			break;
		case Double:
			s = so1.Double;
			break;
		default:
			s = so1.String;
			break;
		}
		s.append(so2.String);
		nuevo = new Stack_Object(s);
		break;
	default:
		reportError("ERROR INESPERADO EN ADD!!!!1!");
		break;
	}

	this->stack.push(*nuevo);
}

// Method that corresponds to the smart operator SUB.
void VMInzunza::operationSUB()
{
	char c;
	int i;
	double d;
	string s;
	Stack_Object so1, so2, *nuevo;

	so2 = stack.top();
	stack.pop();
	so1 = stack.top();
	stack.pop();

	switch (so1.Char)
	{
	case Char:
		switch (so2.tipo)
		{
		case Char:
			c = so1.Char - so2.Char;
			nuevo = new Stack_Object(c);
			break;
		case Integer:
			i = (int)so1.Char - so2.Int;
			nuevo = new Stack_Object(i);
			break;
		case Double:
			d = (double)so1.Char - so2.Double;
			nuevo = new Stack_Object(d);
			break;
		case String:
			reportError("ERROR: no se puede restar un string.");
			break;
		default:
			reportError("ERROR INESPERADO EN SUB!!!!1!");
			break;
		}
		break;
	case Integer:
		switch (so2.tipo)
		{
		case Char:
			i = so1.Int - (int)so2.Char;
			nuevo = new Stack_Object(i);
			break;
		case Integer:
			i = so1.Int - so2.Int;
			nuevo = new Stack_Object(i);
			break;
		case Double:
			d = (double)so1.Int - so2.Double;
			nuevo = new Stack_Object(d);
			break;
		case String:
			reportError("ERROR: no se puede restar un string.");
			break;
		default:
			reportError("ERROR INESPERADO EN SUB!!!!1!");
			break;
		}
		break;
	case Double:
		switch (so2.tipo)
		{
		case Char:
			d = so1.Double - (double)so2.Char;
			nuevo = new Stack_Object(d);
			break;
		case Integer:
			d = so1.Double - so2.Int;
			nuevo = new Stack_Object(d);
			break;
		case Double:
			d = so1.Double - so2.Double;
			nuevo = new Stack_Object(d);
			break;
		case String:
			reportError("ERROR: no se puede restar un string.");
			break;
		default:
			reportError("ERROR INESPERADO EN SUB!!!!1!");
			break;
		}
		break;
	case String:
		reportError("ERROR: no se puede restar un string.");
		break;
	default:
		reportError("ERROR INESPERADO EN SUB!!!!1!");
		break;
	}

	this->stack.push(*nuevo);
}

// Method that corresponds to the smart operator MUL.
void VMInzunza::operationMUL()
{
}

// Method that corresponds to the smart operator DIV.
void VMInzunza::operationDIV()
{
}

// Method that corresponds to the smart operator MOD.
void VMInzunza::operationMOD()
{
}


// Method that corresponds to the operator CMP.
void VMInzunza::operationCMP()
{
	// Igual que la resta en algunos casos
	char c;
	int i;
	double d;
	string s;
	Stack_Object so1, so2;

	so2 = stack.top();
	stack.pop();
	so1 = stack.top();
	stack.pop();

	switch (so1.Char)
	{
	case Char:
		switch (so2.tipo)
		{
		case Char:
			c = so1.Char - so2.Char;
			flag = (double)c;
			break;
		case Integer:
		case Double:
		case String:
			flag = -1;
			break;
		default:
			reportError("ERROR INESPERADO EN CMP!!!!1!");
			break;
		}
		break;
	case Integer:
		switch (so2.tipo)
		{
		case Char:
			flag = -1;
			break;
		case Integer:
			i = so1.Int - so2.Int;
			flag = i;
			break;
		case Double:
			d = so1.Int - so2.Double;
			flag = d;
			break;
		case String:
			flag = -1;
			break;
		default:
			reportError("ERROR INESPERADO EN CMP!!!!1!");
			break;
		}
		break;
	case Double:
		switch (so2.tipo)
		{
		case Char:
			flag = -1;
			break;
		case Integer:
			d = so1.Double - so2.Int;
			flag = d;
			break;
		case Double:
			d = so1.Double - so2.Double;
			flag = d;
			break;
		case String:
			flag = -1;
			break;
		default:
			reportError("ERROR INESPERADO EN CMP!!!!1!");
			break;
		}
		break;
	case String:
		switch (so2.tipo)
		{
		case Char:
		case Integer:
		case Double:
			flag = -1;
		case String:
			flag = so1.String.compare(so2.String);
			break;
		default:
			reportError("ERROR INESPERADO EN CMP!!!!1!");
			break;
		}
		break;
	default:
		reportError("ERROR INESPERADO EN CMP!!!!1!");
		break;
	}
}



#pragma endregion





