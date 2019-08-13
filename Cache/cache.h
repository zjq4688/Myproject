#pragma once
#include<iostream>
#include<map>
using namespace std;

class Node
{
public:
	Node() {}
	Node(int x)
	{
		val = x;
	}
	int val = 0;
	Node* prev = NULL;
	Node* next = NULL;
};

class DoubleList
{
public:

	DoubleList(int capacity = 100)
	{
		Listsize = 0;
		head = NULL;
		tail = NULL;
		Listcapcity = capacity;
	}
	//头插入
	bool add_head(Node* node)
	{
		if (head == NULL)
		{
			head = node;
			tail = node;
			head->prev = NULL;
			tail->next = NULL;
			++Listsize;
			return false;
		}
		else
		{
			node->next = head;
			head->prev = node;
			head = node;
			head->prev = NULL;

			++Listsize;
			return true;
		}

	}

	//尾插入
	bool add_tail(Node* node)
	{
		if (tail == NULL)
		{
			head = node;
			tail = node;
			head->prev = NULL;
			tail->next = NULL;
			++Listsize;
			return true;
		}
		else
		{
			node->prev = tail;
			tail->next = node;
			tail = node;
			tail->next = NULL;
			++Listsize;
			return true;
		}
	}

	//任意删除
	Node* removerad(Node* node)
	{
		//node为 NULL 默认删除尾部节点
		if (node == NULL)
			node = tail;
		if (node == tail)
		{
			remove_tail();
			return node;
		}

		else if (node == head)
		{
			remove_head();
			return node;
		}

		else
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			--Listsize;
			return node;
		}

	}
	//尾部删除
	Node* remove_tail()
	{
		if (Listsize == 0)
			return NULL;

		else if (Listsize == 1)
		{
			Node* node = tail;
			head = NULL;
			tail = NULL;
			--Listsize;
			return node;
		}
		else
		{
			Node* node = tail;
			tail = tail->prev;
			tail->next = NULL;
			--Listsize;
			return node;
		}
	}
	//头部删除
	Node* remove_head()
	{
		if (Listsize == 0)
			return NULL;

		else if (Listsize == 1)
		{
			Node* node = head;
			head = NULL;
			tail = NULL;
			--Listsize;
			return node;
		}
		else
		{
			Node* node = head;
			head = head->next;
			head->prev = NULL;
			--Listsize;
			return node;
		}
	}

	//外部接口：弹出头部
	Node* pop()
	{
		return remove_head();
	}

	//外部接口： 添加节点  从尾部
	bool append(Node* node)
	{
		return add_tail(node);
	}

	//外部接口： 添加节点  从头部
	bool append_front(Node* node)
	{
		return add_head(node);
	}

	//外部接口： 删除节点
	Node* remove(Node* node = NULL)
	{
		return removerad(node);
	}
	//外部接口： 打印链表
	void printList()
	{
		if (Listsize == 0)
			cout << "No Node..." << endl;
		else {
			Node* cur = head;
			cout << "{ ";
			while (cur != NULL)
			{
				cout << " -> " << cur->val ;
				cur = cur->next;
			}
			cout <<" }"<< endl;
		}

	}
	//数据成员部分
	int Listsize;
	int Listcapcity;
	Node* head;
	Node* tail;
};
