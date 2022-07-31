#pragma once



enum
{
	STATUS_FREE,
	STATUS_ALLOCATED,
	STATUS_DELETED
};



struct MemorySegment
{
	int status{ STATUS_FREE };
	int length{ 0 };
};



struct Node
{
	MemorySegment data;
	Node* next{ nullptr };
	Node* prev{ nullptr };
};



struct dList
{
	Node* head{ nullptr };
	Node* tail{ nullptr };



	~dList()
	{
		while (head)
		{
			auto temp = head->next;
			delete head;
			head = temp;
		}
	}



	void pushBack(const MemorySegment& segment)
	{
		auto node = new Node();
		node->data = segment;

		if (!head)
		{
			head = tail = node;
			return;
		}

		tail->next = node;
		node->prev = tail;
		tail = node;
	}



	void pushFront(const MemorySegment& segment)
	{
		auto node = new Node();
		node->data = segment;

		if (!head)
		{
			head = tail = node;
			return;
		}

		node->next = head;
		head->prev = node;
		head = node;
	}



	MemorySegment getData(int idx)
	{
		if (!head)
			return {};

		auto curNode = head;

		for (int i = 0; curNode->next && i < idx; i++) {
			curNode = curNode->next;
		}

		return curNode->data;
	}



	Node* get(int idx)
	{
		if (!head)
			return {};

		auto curNode = head;

		for (int i = 0; curNode->next && i < idx; i++) {
			curNode = curNode->next;
		}

		return curNode;
	}



	void set(int idx, MemorySegment val)
	{
		if (!head)
			return;

		auto curNode = head;

		for (int i = 0; curNode->next && i < idx; i++)
		{
			curNode = curNode->next;
		}

		curNode->data = val;
	}



	void popBack()
	{
		if (!head)
			return;

		auto temp = tail->prev;
		delete tail;
		tail = temp;
		tail->next = nullptr;
	}



	void popFront()
	{
		if (!head)
			return;

		auto temp = head->next;
		delete head;
		head = temp;
		head->prev = nullptr;
	}



	void remove(Node* node)
	{
		if (node->prev)
			node->prev->next = node->next;

		if (node->next)
			node->next->prev = node->prev;
	}



	MemorySegment back()
	{
		if (isEmpty())
			return {};

		return tail->data;
	}



	MemorySegment front()
	{
		if (isEmpty())
			return {};

		return head->data;
	}



	bool isEmpty()
	{
		return head == nullptr;
	}
};