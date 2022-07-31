#pragma once
#include "Stack.h"
#include "DList.h"


const int N = 1000;


struct MemoryManager
{
	dList memorySegments;
	Stack freeSegments;
	std::vector<Node*> arr{ N, nullptr };
	int idx{ 0 };


	MemoryManager()
	{
		MemorySegment segment;
		segment.length = N - 1;
		segment.status = STATUS_FREE;

		Node* node = new Node();
		node->data = segment;

		memorySegments.pushBack(segment);
		freeSegments.push(node);
	}



	int malloc(const int n)
	{
		Node* freeNode = new Node();
		while (true)
		{
			if (freeSegments.freeSegments.empty())
				return -1;
			
			auto n = freeSegments.pop();

			if (n->data.status == STATUS_DELETED)
			{
				delete n;
				continue;
			}

			if (n->data.status == STATUS_FREE)
			{
				freeNode = n;
				break;
			}
		}

		if (freeNode->data.length < n)
			return -1;

		auto diff = freeNode->data.length - n;

		if (diff > 0)
		{
			auto newFreeSegment = new Node();
			newFreeSegment->prev = freeNode;
			newFreeSegment->next = freeNode->next;
			newFreeSegment->data.length = diff;
			newFreeSegment->data.status = STATUS_FREE;
			
			freeNode->next = newFreeSegment;
			freeSegments.push(newFreeSegment);
		}

		freeNode->data.length = n;
		freeNode->data.status = STATUS_ALLOCATED;

		arr[idx] = freeNode;

		auto ret = idx;
		idx++;

		return ret;
	}



	int free(const int n)
	{
		if (n >= arr.size())
			return -1;

		if (arr[n] == nullptr)
			return -1;

		if (arr[n]->data.status != STATUS_ALLOCATED)
			return -1;

		auto node = arr[n];
		arr[n] = nullptr;
		int size = 0;
		size += node->data.length;
		
		if (node->prev != nullptr)
		{
			if (node->prev->data.status == STATUS_FREE)
			{
				size += node->prev->data.length;
				node->prev->data.status = STATUS_DELETED;
				memorySegments.remove(node->prev);
			}
		}

		if (node->next != nullptr)
		{
			if (node->next->data.status == STATUS_FREE)
			{
				size += node->next->data.length;
				node->next->data.status = STATUS_DELETED;
				memorySegments.remove(node->next);
			}
		}

		memorySegments.remove(node);
		delete node;
		
		MemorySegment newSegment;
		newSegment.length = size;
		newSegment.status = STATUS_FREE;

		memorySegments.set(n, newSegment);
		auto newNode = memorySegments.get(n);
		freeSegments.push(newNode);

		return 0;
	}
};