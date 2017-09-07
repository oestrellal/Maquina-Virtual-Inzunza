#include "stdafx.h"
#include "VMInzunza.h"
#include <string>

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
#define PUSHC 10	//Inserts a char on top of the stack.
#define PUSHI 11	//Inserts an int on top of the stack.
#define PUSHD 12	//Inserts a double on top of the stack.
#define PUSHS 13	//Inserts a string on top of the stack.
#define PUSHKC 14	//Inserts a constant char on top of the stack.
#define PUSHKI 15	//Inserts a constant int on top of the stack.
#define PUSHKD 16	//Inserts a constant double on top of the stack.
#define PUSHKS 17	//Inserts a constant string on top of the stack.
#define PUSHAC 18	//Inserts a char from an array on top of the stack.
#define PUSHAI 19	//Inserts an int from an array on top of the stack.
#define PUSHAD 20	//Inserts a double from an array on top of the stack.
#define PUSHAS 21	//Inserts a string from an array on top of the stack.
#define POPC 22		//Pops data from the stack as a char.
#define POPI 23		//Pops data from the stack as an int.
#define POPD 24		//Pops data from the stack as a double.
#define POPS 25		//Pops data from the stack as a string.
#define POPAC 26	//Pops data from the stack as a char using the offset register.
#define POPAI 27	//Pops data from the stack as a int using the offset register.
#define POPAD 28	//Pops data from the stack as a double using the offset register.
#define POPAS 29	//Pops data from the stack as a string using the offset register.
#define POPX 30		//Pops from stack to pointer register (int).
#define RDC 31		//Reads a char into memory.
#define RDI 32		//Reads an int into memory.
#define RDD 33		//Reads a double into memory.
#define RDS 34		//Reads a string into memory.
#define RDAC 35		//Reads a char into an array using the offset register.
#define RDAI 36		//Reads a int into an array using the offset register.
#define RDAD 37		//Reads a double into an array using the offset register.
#define RDAS 38		//Reads a string into an array using the offset register.
#define JMP 39		//Jumps indonditionally to a memory address.
#define JEQ 40		//Jumps if the top of the stack is (==) 0.
#define JNE 41		//Jumps if the top of the stack is different than (!=) 0.
#define JGT 42		//Jumps if the top of the stack is greater than (>) 0.
#define JGE 43		//Jumps if the top of the stack is greater or equal than (>=) 0.
#define JLT 44		//Jumps if the top of the stack is less than (<) 0.
#define JLE 45		//Jumps if the top of the stack is less or equal than (<=) 0.
#define STX 46		//Stores a variable in the pointer register.
#define STKX 47		//Stores a constant int in the pointer register.
#define STY 48		//
#define	STKY 49
#define INC 50		//Increases the value of a memory address by 1.
#define RED 51		//Decreases the value of a memory address by 1.
#define ADD 52		//Pops the two topmost numbers from the stack and adds them. The result is stored in the stack.
#define SUB 53		//Pops the two topmost numbers from the stack and subtracts them. The result is stored in the stack.
#define MUL 54		//Pops the two topmost numbers from the stack and multiplies them. The result is stored in the stack.
#define DIV 55		//Pops the two topmost numbers from the stack and divides them. The result is stored in the stack.
#define MOD 56		//Pops the two topmost numbers from the stack and divides them. The remainder is stored in the stack.
#define CMP 57		//Compares 2 stack values by subtracting them and stores the result in the stack.

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


//Encargado de leer archivo, verificar identificador, cardar porci�n de c�digo y datos, e iniciar IP en 0
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

	//oData= new unsigned char[length+1];
	//infile.read( (char*)oData, length);

	/*
	for (int i = 0; i < length+1; i++) {
		cout << oData[i] << endl;
	}
	*/
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

	//error: check why � is 242 instead of 168
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
	string stringContainer;
	Stack_Object *nuevo;		// Elemento que se va a agregar al stack;

	//Loop
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
			std::cout << intContainer;
			intContainer = 0;
			break;
		case WRD:
			IP++;
			dir = getDir();
			doubleContainer = getDouble(dir);
			std::cout << doubleContainer;
			doubleContainer = 0;
			break;
		case WRS:
			IP++;
			dir = getDir();
			stringContainer = getString(dir);
			cout << stringContainer;
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
			cin >> intContainer;
			setInt(intContainer,dir);
			break;
		case RDD:
			IP++;
			dir = getDir();
			cin >> doubleContainer;
			setDouble(doubleContainer,dir);
			break;
		case RDS:
			IP++;
			dir = getDir();
			cin >> stringContainer;//error: change to read full line
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
			setInt(intContainer,dir);
			break;
		case RDAD:
			IP++;
			dir = getDir();
			cin >> doubleContainer;
			dir = dir + getX()*8;
			setDouble(doubleContainer,dir);
			break;
		case RDAS:
			IP++;
			dir = getDir();
			cin >> stringContainer;
			dir = dir + getX()*getY();
			int i;
			for (i = 0; i < stringContainer.length(); i++) {
				DS[dir + i] = stringContainer[i];
			}
			DS[dir + i] = '\0';
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
			operationADD();
			break;
		case SUB:
			break;
		case MUL:
			break;
		case DIV:
			break;
		case MOD:
			break;
		case CMP:
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

#pragma region Operators' helper functions

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
	char c, int i, double d;
	string s;
	Stack_Object so1, so2, *nuevo;
								// QUE PASA SI SE HACE UN POP Y EL STACK ESTA VACIO???
	so1 = stack.top();
	stack.pop();
	so2 = stack.top();
	stack.pop();

	
}

//Gets a string until it finds the null char
string VMInzunza::getString(unsigned int dir) {
	unsigned int next = dir;
	string value = "";
	char temp = DS[next];

	while (DS[next] != '\0') {
		value += DS[next];
		next++;
		temp = DS[next];
	}
	return value;
// Method that corresponds to the smart operator SUB.
void VMInzunza::operationSUB()
{
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
}

// Determines the data type of the resulting operation to be efectuated in the stack.
DATA_TYPE VMInzunza::getOperationDataType(Stack_Object so1, Stack_Object so2)
{
	//TODO ESTE COCHINERO SE PUEDE HACER MAS EFICIENTE.
	switch (so1.tipo)
	{
	case DATA_TYPE::Char:
		switch (so2.tipo)
		{
		case DATA_TYPE::Char:
			return DATA_TYPE::Char;
			break;
		case DATA_TYPE::Integer:
			return DATA_TYPE::Integer;
			break;
		case DATA_TYPE::Double:
			return DATA_TYPE::Double;
			break;
		case DATA_TYPE::String:
			return DATA_TYPE::String;
			break;
		default:
			break;
		}
		break;
	case DATA_TYPE::Integer:
		switch (so2.tipo)
		{
		case DATA_TYPE::Char:
		case DATA_TYPE::Integer:
			return DATA_TYPE::Integer;
			break;
		case DATA_TYPE::Double:
			return DATA_TYPE::Double;
			break;
		case DATA_TYPE::String:
			return DATA_TYPE::String;
			break;
		default:
			break;
		}
		break;
	case DATA_TYPE::Double:
		switch (so2.tipo)
		{
		case DATA_TYPE::Char:
		case DATA_TYPE::Integer:
		case DATA_TYPE::Double:
			return DATA_TYPE::Double;
			break;
		case DATA_TYPE::String:
			return DATA_TYPE::String;
			break;
		default:
			break;
		}
		break;
	case DATA_TYPE::String:
		return DATA_TYPE::String;
		break;
	default:
		break;
	}

	return DATA_TYPE::Double; //En caso de que algo salga mal.
}

#pragma endregion





