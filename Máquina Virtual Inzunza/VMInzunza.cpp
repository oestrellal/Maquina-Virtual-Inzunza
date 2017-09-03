#include "stdafx.h"
#include "VMInzunza.h"


VMInzunza::VMInzunza()
{
}


VMInzunza::~VMInzunza()
{
}

//Prueba
bool VMInzunza::load(string dir) 
//Encargado de leer archivo, verificar identificador, cardar porción de código y datos, e iniciar IP en 0
{
	ifstream infile;
	size_t length = 0;

	infile.open(dir,ios::in|ios::binary);
	char* oData = 0;

	infile.seekg(0, infile.end);
	length = infile.tellg();
	infile.seekg(0, infile.beg);

	oData= new char[length+1];
	infile.read(oData, length);
	infile.close();
	oData[length] = '\0';

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
				cout << "ERROR";
			}
		}
		else {
			//error: longitud del archivo menor al identificador
		}
	}

	//asignar memoria para codigo
	unsigned int sizeCode = ((unsigned int)oData[i]<<8 )+ (unsigned int)oData[i + 1];
	i += 2;
	//asignar memoria para datos
	unsigned int sizeData = ( (int)oData[i]<<8) + (int)oData[i + 1];
	i += 2;
	
	CS = new char[sizeCode];
	DS = new char[sizeData];

	IP = 0;

	getchar();



	return false;
}

void VMInzunza::run()
{
}

void VMInzunza::setX(int direccion)
{
}

int VMInzunza::getX()
{
	return 0;
}

int VMInzunza::bin2dir(string direccion)
{
	return 0;
}
