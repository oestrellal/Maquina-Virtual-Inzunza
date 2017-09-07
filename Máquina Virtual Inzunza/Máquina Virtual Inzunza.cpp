// Máquina Virtual Inzunza.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VMInzunza.h"


using namespace std;
int main()
{
	VMInzunza vm;

	vm.load("C:/Users/e0438883/Desktop/prueba.cea");
	vm.run();

	getchar();


    return 0;
}

