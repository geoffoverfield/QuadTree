#include <iostream>
using namespace std;

#ifndef STACK_H
#define STACK_H

//================================NODE FOR DATA STORAGE==================================================================
template <class T>
struct Node
{
public:
	T x;
	T data;
	Node *btmLeft;
	Node *btmRight;
	Node *topLeft;
	Node *topRight;
	Node *next;
	bool visited = false;
	Node();
	Node(T a)
	{
		x = a;
		data = a;
		btmLeft = NULL;
		btmRight = NULL;
		topLeft = NULL;
		topRight = NULL;
	}

	//Overloaded Operators
	bool operator < (Node<T> &pnt)
	{
		if (this->x < pnt.x) return false;
		else return true;
	}
	bool operator == (Node<T> &p)
	{
		if (this->x == p.x) return false;
		else return true;
	}
	bool operator = (Node<T> &p)
	{
		if (this->x = p.x) return false;
		else return true;
	}
private:
};

template <class T>
Node<T>::Node()
{
	btmLeft = NULL;
	btmRight = NULL;
	topbtmLeft = NULL;
	topbtmRight = NULL;
	next = NULL;
}
//=======================================================================================================================

//================================STACK FOR TRAVERSAL====================================================================
template <class T>
class Stack
{
public:
	Stack();
	~Stack();
	bool isStackEmpty();
	void Push(T &);
	void Pop();
	void Display();
	Node<T> *GetNewNode(T &);
private:
	Node<T> *top;
};

template <class T>
Stack<T>::Stack()
{
	top = NULL;
}

template <class T>
Stack<T>::~Stack()
{
	if (!isStackEmpty())
	{
		Node<T> *curr = top;
		Node<T> *tempPtr;
		while (curr != 0)
		{
			tempPtr = curr;
			curr = curr->next;
			delete tempPtr;
		}
	}
}

template <class T>
bool Stack<T>::isStackEmpty()
{
	return (top == NULL);
}

template <class T>
void Stack<T>::Push(T &a)
{
	Node<T> *ptr = GetNewNode(a);
	if (isStackEmpty())
	{
		top = ptr;
	}
	else
	{
		ptr->next = top;
		top = ptr;
	}
}

template <class T>
void Stack<T>::Pop()

{
	if (isStackEmpty())
		return false;
	else
	{
		Node<T> *temp = top;
		if (top->next == NULL)
			top = NULL;
		else
			top = top->next;
		x = temp->data;
		return true;
	}
}

template <class T>
void Stack<T>::Display()
{
	if (isStackEmpty())
	{
		Pyramid<T> msg;
		msg.ErrorMsg();
		delete msg;
		return;
	}
	Node<T> *curr = top;
	while (curr)
	{
		cout << curr->data << endl;
		curr = curr->next;
	}
	cout << endl << endl <<
		"===============================================================" << endl << endl;
}

template <class T>
Node<T> *Stack<T>::GetNewNode(T &c)
{
	Node<T> *ptr = new Node<T>(c);
	return ptr;
}

//========================================================================================================================


#endif