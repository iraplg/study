#pragma once
#include <vector>
#include "DList.h"


struct Stack
{
	std::vector<Node*> freeSegments;



	void push(Node* node)
	{
		freeSegments.push_back(node);
	}



	Node* pop()
	{
		if (freeSegments.empty())
			return nullptr;

		auto res = freeSegments.back();
		freeSegments.pop_back();
		
		return res;
	}
};