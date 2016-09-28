#include "Wurst.h"
#include <stdio.h>


Wurst::Wurst()
{
	print();
}


void Wurst::print()
{
	printf("NEW WURST");
}

int Wurst::getNum()
{
	return 1337;
}

std::string Wurst :: getStuff()
{
	return "WURSTI";
}

Wurst::~Wurst()
{
}
