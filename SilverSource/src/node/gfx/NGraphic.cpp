#include "NGraphic.h"



NGraphic::NGraphic(std::string name, int z) : Node(name)
{
	this->z = z;
}

NGraphic::NGraphic(std::string name) : Node(name)
{
	this->z = 0;
}



NGraphic::~NGraphic()
{
}
