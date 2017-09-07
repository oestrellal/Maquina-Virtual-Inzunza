#include "stdafx.h"
#include "VMInzunza.h"

#pragma region Instrucciones

#define HALT 0		//Finish.
#define WR 1		//Writes a newline.
#define WRC 2		//Writes a char.
#define WRI 3		//Writes an int.
#define WRD 4		//Writes a double.
#define WRS 5		//Writes a string.
#define PUSHC 6		//Inserts a char on top of the stack.
#define PUSHI 7		//Inserts an int on top of the stack.
#define PUSHD 8		//Inserts a double on top of the stack.
#define PUSHS 9		//Inserts a string on top of the stack.
#define PUSHKC 10	//Inserts a constant char on top of the stack.
#define PUSHKI 11	//Inserts a constant int on top of the stack.
#define PUSHKD 12	//Inserts a constant double on top of the stack.
#define PUSHKS 13	//Inserts a constant string on top of the stack.
#define PUSHAC 14	//Inserts a char from an array on top of the stack.
#define PUSHAI 15	//Inserts an int from an array on top of the stack.
#define PUSHAD 16	//Inserts a double from an array on top of the stack.
#define PUSHAS 17	//Inserts a string from an array on top of the stack.
#define POPC 18		//Pops data from the stack as a char.
#define POPI 19		//Pops data from the stack as an int.
#define POPD 20		//Pops data from the stack as a double.
#define POPS 21		//Pops data from the stack as a string.
#define POPAC 22	//Pops data from the stack as a char using the offset register.
#define POPAI 23	//Pops data from the stack as a int using the offset register.
#define POPAD 24	//Pops data from the stack as a double using the offset register.
#define POPAS 25	//Pops data from the stack as a string using the offset register.
#define POPX 26		//Pops from stack to pointer register (int).
#define RDC 27		//Reads a char into memory.
#define RDI 28		//Reads an int into memory.
#define RDD 29		//Reads a double into memory.
#define RDS 30		//Reads a string into memory.
#define RDAC 31		//Reads a char into an array using the offset register.
#define RDAI 32		//Reads a int into an array using the offset register.
#define RDAD 33		//Reads a double into an array using the offset register.
#define RDAS 34		//Reads a string into an array using the offset register.
#define JMP 35		//Jumps indonditionally to a memory address.
#define JEQ 36		//Jumps if the top of the stack is (==) 0.
#define JNE 37		//Jumps if the top of the stack is different than (!=) 0.
#define JGT 38		//Jumps if the top of the stack is greater than (>) 0.
#define JGE 39		//Jumps if the top of the stack is greater or equal than (>=) 0.
#define JLT 40		//Jumps if the top of the stack is less than (<) 0.
#define JLE 41		//Jumps if the top of the stack is less or equal than (<=) 0.
#define STX 42		//Stores a variable in the pointer register.
#define STKX 43		//Stores a constant int in the pointer register.
#define INC 44		//Increases the value of a memory address by 1.
#define DEC 45		//Decreases the value of a memory address by 1.
#define ADD 46		//Pops the two topmost numbers from the stack and adds them. The result is stored in the stack.
#define SUB 47		//Pops the two topmost numbers from the stack and subtracts them. The result is stored in the stack.
#define MUL 48		//Pops the two topmost numbers from the stack and multiplies them. The result is stored in the stack.
#define DIV 49		//Pops the two topmost numbers from the stack and divides them. The result is stored in the stack.
#define MOD 50		//Pops the two topmost numbers from the stack and divides them. The remainder is stored in the stack.
#define CMP 51		//Compares 2 stack values by subtracting them and stores the result in the stack.

#pragma endregion Mapa de Instrucciones a Byte

#pragma region Constantes

#define MAX_STACK 500;		//Max quantity of objects that can be on the stack.

#pragma endregion

VMInzunza::VMInzunza()
{
	
}


VMInzunza::~VMInzunza()
{
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

	unsigned int byte1;
	unsigned int byte2;
	//asignar memoria para codigo
	unsigned int sizeCode;

	byte1 = oData[i];
	byte2 = oData[i + 1];
	sizeCode = byte1 << 8;
	sizeCode += byte2;

	i += 2;
	//asignar memoria para datos
	unsigned int sizeData;
	byte1 = oData[i];
	byte2 = oData[i + 1];
	sizeData = byte1 << 8;
	sizeData += byte2;
	i += 2;

	//error: check why ó is 242 instead of 168
	CS = new unsigned char[sizeCode];
	DS = new unsigned char[sizeData];

	//copy the code
	int start = i;
	for (i; (i < start + sizeCode && i < oData.size()); i++) {
		CS[i - start] = oData[i];
	}

	//start Instruction pointer
	IP = 0;
	return false;
}

void VMInzunza::run()
{
	//hERE WE CAN INPUT TEST CODES

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
	CS[21] = '\2';
	CS[22] = HALT;

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

#pragma region Push
		case PUSHC:
			break;
		case PUSHI:
			break;
		case PUSHD:
			break;
		case PUSHS:
			break;
#pragma endregion

#pragma region Pop
		/*case POPC:
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
			break;*/
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
			setInt(dir);
			break;
		case RDD:
			IP++;
			dir = getDir();
			cin >> doubleContainer;
			setDouble(dir);
			break;
		case RDS:
			IP++;
			dir = getDir();
			cin >> stringContainer;
			int i;
			for (i = 0; i < stringContainer.length(); i++) {
				DS[dir + i] = stringContainer[i];
			}
			DS[dir + i] = '\0';
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
			int i;
			dir = dir + getX()*maxStringSize;
			for (i = 0; i < stringContainer.length(); i++) {
				DS[dir + i] = stringContainer[i];
			}
			DS[dir + i] = '\0';
			break;*/

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

//gets a direction of memory based on the next 2 bytes in code segment and moves the IP
unsigned int VMInzunza::getDir() {
	unsigned int byte1 = CS[IP];
	unsigned int byte2 = CS[IP + 1];
	IP += 2;
	unsigned int dir = byte1 << 8;
	dir += byte2;
	return dir;
}

int VMInzunza::getInt(unsigned int dir) {

	union {
		int value;
		char bytes[sizeof(int)];
	} u;
	for (int i = 0; i < 4; i++) {
		u.bytes[i] = DS[dir + i];
	}
	return u.value;

}
int VMInzunza::getInt() {

	union {
		int value;
		char bytes[sizeof(int)];
	} u;
	for (int i = 0; i < 4; i++) {
		u.bytes[i] = CS[IP + i];
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
		u.bytes[i] = DS[dir + i];
	}
	return u.value;

}
double VMInzunza::getDouble() {

	union {
		double value;
		char bytes[sizeof(double)];
	} u;
	for (int i = 0; i < 8; i++) {
		u.bytes[i] = CS[IP + i];
	}
	IP += 8;
	return u.value;

}
void VMInzunza::setInt(int toSave, unsigned int dir)
{
	union 
	{
			int value;
			char bytes[sizeof(int)];
	} u;

	u.value = toSave;

	for (int i = 0; i < 4; i++) {
		DS[dir + i] = u.bytes[i];
	}	
}

void VMInzunza::setDouble(double toSave,unsigned int dir)
{
	union 
		{
			double value;
			char bytes[sizeof(int)];
		} u;
	u.value = toSave;
	for (int i = 0; i < 8; i++) {
		DS[dir + i] = u.bytes[i];
	}
}
void VMInzunza::setString(string toSave,unsigned int dir)
{
	int i = 0;
	for (i; i < toSave.length(); i++);
	DS[dir + i] = toSave[i];
	DS[dir + i] = '\0';
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
//validates that the offset register is valid
void VMInzunza::setX(unsigned int direccion)
{
	X = direccion;
}
// returns the offset register value
unsigned int VMInzunza::getX()
{
	return X;
}
//validates that the offset register is valid
void VMInzunza::setY(unsigned int direccion)
{
	Y = direccion;
}
// returns the offset register value
unsigned int VMInzunza::getY()
{
	return Y;
}
