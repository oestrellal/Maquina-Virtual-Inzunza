#pragma once

#include <string>

using namespace std;

class VMInzunza
{
public:
	VMInzunza();
	~VMInzunza();

	bool load();
	void run();

	private:
		void setX(int direccion);
		int getX();
		int bin2dir(string direccion);
		
};

