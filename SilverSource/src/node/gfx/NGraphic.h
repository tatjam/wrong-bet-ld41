#pragma once
#include "../Node.h"

class NGraphic : public Node
{
private:

public:

	int z = 0;

	NGraphic(std::string name, int z);
	NGraphic(std::string name);
	~NGraphic();
};

