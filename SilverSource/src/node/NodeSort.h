#pragma once

#include "Node.h"

bool nodeSort (std::weak_ptr<Node> a, std::weak_ptr<Node> b)
{
	std::shared_ptr<Node> aS = a.lock();
	std::shared_ptr<Node> bS = b.lock();
	return aS->getZ() < bS->getZ();
}

