// Máquina Virtual Inzunza.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VMInzunza.h"


using namespace std;
int main(int argc, char* argv[]) {

	if (argc > 1 && argc <= 2) {
		try {
			VMInzunza vm;
			string dir;
			dir = argv[1];
			vm.load(dir);
			vm.run();
		}
		catch (int i) {

		}
	}
	else {
		cout << "Please provide source and destination params" << endl;
	}

	getchar();


	return 0;
}

