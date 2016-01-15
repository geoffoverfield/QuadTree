//Geoff Overfield
//Templated Pyramid Tree

#include <iostream>
#include "Stack.h"
#include <list>
using namespace std;

#ifndef HEADER_H
#define HEADER_H


//===============================================PYRAMID STRUCTURE========================================================
template <class T>
class Pyramid
{
public:
	T x;
	Pyramid();
	~Pyramid();
	int Parent(Node<T> &, Node<T> &);
	void Insert(T &);
	void Delete(T &);
	Node<T> *Search(T &);
	size_t Size();
	bool isEmpty();
	void Clear(Node<T> *);
	void DepthFirstSearch();
	Stack<Node<T>> myStack;
	Node<T> Apex()
	{
		return apex;
	}
private:
	size_t size;
	Node<T> *apex;
	int parentNum = 0;
	int currNum = 0;
	bool visit = false;
	void ErrorMsg();
	void depthFirstAssist(Node<T> &, bool);
};

template <class T>
Pyramid<T>::Pyramid()
{
	apex = NULL;
	size = 0;
}

template <class T>
Pyramid<T>::~Pyramid()
{
	Clear(apex);
}

template <class T>
int Pyramid<T>::Parent(Node<T> &par, Node<T> &cur)
{
	if (par->btmLeft == cur) parentNum = 1;
	if (par->btmRight == cur) parentNum = 2;
	if (par->topLeft == cur) parentNum = 3;
	if (par->topRight == cur) parentNum = 4;
	
	return parentNum;
}

template <class T>
void Pyramid<T>::Insert(T &data)
{
	Node<T> *newNode = new Node<T>(data);
	Node<T> *parent = NULL;
	Node<T> *curr = apex;
	if (isEmpty())
	{
		apex = newNode;
		size++;
		return;
	}

	//Traverse to bottom of pyramid
	while (curr)
	{
		parent = curr;
		//If duplicate - delete record
		if (curr->x == newNode->x)
		{
			newNode = NULL;
			delete newNode;
			break;
		}
		else if (newNode->x > curr->x)
		{
			if (curr->btmRight != NULL)
				curr = curr->btmRight;
			else
				curr = curr->topRight;
		}
		else if (newNode->x < curr->x)
		{
			if (curr->btmLeft != NULL)
				curr = curr->btmLeft;
			else
				curr = curr->topLeft;
			
		}
	}
	
	//Insert new node
	if (newNode)
	{
		if (newNode->x < parent->x)
		{
			if (parent->btmLeft == NULL)
				parent->btmLeft = newNode;
			else parent->topLeft = newNode;
			size++;
		}
		else if (newNode->x > parent->x)
		{
			if (parent->btmRight == NULL)
				parent->btmRight = newNode;
			else parent->topRight = newNode;
			size++;
		}
	}
}

template <class T>
void Pyramid<T>::Delete(T &data)
{
	if (IsEmpty())
	{
		throw std::runtime_error("Invalid Action");
	}

	Node<T> *curr = apex;
	Node<T> *parent = NULL;

	//============================================================================================================================
	//====================================================SEARCHING FOR LEAF======================================================
	//============================================================================================================================
	while (curr)
	{
		if (curr->x == data)
			break;
		else
		{
			parent = curr;
			if (curr->x > data)
			{
				if (curr->btmRight != NULL)
					curr = curr->btmRight;
				else curr = curr->topRight;
			}
			else
			{
				if (curr->btmLeft != NULL)
					curr = curr->btmLeft;
				else curr = curr->topLeft;
			}
		}
	}
	if (curr == NULL)
	{
		ErrorMsg();
		return;
	}

	//============================================================================================================================
	//==================================================CASE 1 - DELETING LEAF====================================================
	//============================================================================================================================
	if (curr->btmLeft == NULL && curr->btmRight == NULL && curr->topLeft == NULL && curr->topRight == NULL)
	{
		if (parent->btmLeft == curr)
			parent->btmLeft = NULL;
		else if (parent->btmRight == curr)
			parent->btmRight = NULL;
		else if (parent->topLeft == curr)
			parent->topLeft == NULL;
		else parent->topRight == NULL;
		delete curr;
		size--;
		return;
	}

	//============================================================================================================================
	//==============================================CASE 2 - NODE WITH SINGLE CHILD===============================================
	//============================================================================================================================
	if ((curr->btmLeft == NULL && curr->topLeft == NULL && curr->topRight == NULL && curr->btmRight != NULL) || 
		(curr->btmLeft != NULL && curr->btmRight == NULL && curr->topLeft == NULL && curr->topRight == NULL) ||
		(curr->topLeft != NULL && curr->topRight == NULL && curr->btmLeft == NULL && curr->btmRight == NULL) || 
		(curr->topRight != NULL && curr->topLeft == NULL && curr->btmLeft == NULL && curr->btmRight == NULL))
	{
		if (curr->btmRight) currNum = 1;
		else if (curr->btmLeft ) currNum = 2;
		else if (curr->topLeft ) currNum = 3;
		else if (curr->topRight) currNum = 4;
		else ErrorMsg();
		switch (currNum)
		{
		case 1:
			if (Parent(parent, curr) == 1)
				parent->btmLeft = curr->btmRight;
			break;
		case 2:
			if (Parent(parent, curr) == 2)
				parent->btmRight = curr->btmRight;
			break;
		case 3:
			if (Parent(parent, curr) == 3)
				parent->topLeft = curr->btmRight;
			break;
		case 4:
			if (Parent(parent, curr) == 4)
				parent->topRight = curr->btmRight;
			break;
		default: ErrorMsg();
		}

		delete curr;
		size--;
	}

	//============================================================================================================================
	//==============================================CASE 3 - NODE WITH TWO CHILDREN===============================================
	//============================================================================================================================

	else if ((curr->btmLeft != NULL && curr->btmRight != NULL && curr->topLeft == NULL && curr->topRight == NULL) ||
		(curr->topLeft != NULL && curr->topRight != NULL && curr->btmLeft == NULL && curr->btmRight == NULL) ||
		(curr->btmLeft != NULL && curr->topLeft != NULL && curr->btmRight == NULL && curr->topRight == NULL) ||
		(curr->btmLeft != NULL && curr->topRight != NULL && curr->btmRight == NULL && curr->topLeft == NULL) ||
		(curr->btmRight != NULL && curr->topLeft != NULL && curr->btmLeft == NULL && curr->topRight == NULL) ||
		(curr->btmLeft == NULL && curr->btmRight != NULL && curr->topLeft == NULL && curr->topRight != NULL))

	{
		Node<T> *temp1 = new Node<T>;
		Node<T> *temp2 = new Node<T>;
		Node<T> *temp3 = new Node<T>;

		currNum = 0;
		if (curr->btmLeft && curr->btmRight) currNum = 1;
		else if (curr->topLeft && curr->topRight) currNum = 2;
		else if (curr->btmLeft && curr->topLeft) currNum = 3;
		else if (curr->btmLeft && curr->topRight) currNum = 4;
		else if (curr->btmRight && curr->topLeft) currNum = 5;
		else if (curr->btmRight && curr->topRight) currNum = 6;
		else ErrorMsg();

		switch (currNum)
		{
		case 1:
			temp1 = curr->btmLeft;
			temp2 = curr->btmRight;
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else
			{
				parent->topRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			break;
		case 2:
			temp1 = curr->topLeft;
			temp2 = curr->topRight;
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else
			{
				parent->topRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			break;
		case 3:
			temp1 = curr->btmLeft;
			temp2 = curr->topLeft;
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else
			{
				parent->topRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			break;
		case 4:
			temp1 = curr->btmLeft;
			temp2 = curr->topRight;
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else
			{
				parent->topRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			break;
		case 5:
			temp1 = curr->btmRight;
			temp2 = curr->topLeft;
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else
			{
				parent->topRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			break;
		case 6:temp1 = curr->btmRight;
			temp2 = curr->topRight;
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = temp2;
				temp3 = temp2;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			else
			{
				parent->topRight = temp2;
				temp3 = temp2;
				do
				temp3 = temp3->btmLeft;
				while (temp3->btmLeft != NULL);
				temp1 = temp3->btmLeft;
			}
			break;
		default: ErrorMsg();
		}

		temp1 = temp2 = temp3 = NULL;
		delete temp1, temp2, temp3;
		delete curr;
		size --;
	}


	//============================================================================================================================
	//==============================================CASE 4 - NODE WITH THREE CHILDREN=============================================
	//============================================================================================================================

	else if ((curr->topRight == NULL && curr->btmLeft != NULL && curr->topLeft != NULL && curr->btmRight != NULL)||
		(curr->topLeft == NULL && curr->btmLeft != NULL && curr->topRight != NULL && curr->btmRight != NULL)||
		(curr->btmRight == NULL && curr->btmLeft != NULL && curr->topLeft != NULL && curr->topRight != NULL)||
		(curr->btmLeft == NULL && curr->topLeft != NULL && curr->btmRight != NULL && curr->topRight != NULL))
	{
		Node<T> *temp1 = new Node<T>;
		Node<T> *temp2 = new Node<T>;
		Node<T> *temp3 = new Node<T>;
		currNum = 0;
		if (curr->topRight == NULL) currNum = 1;
		else if (curr->topLeft == NULL) currNum = 2;
		else if (curr->btmRight == NULL) currNum = 3;
		else if (curr->btmLeft == NULL) currNum = 4;
		else ErrorMsg();
		switch (currNum)
		{
		case 1:
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topLeft;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->btmLeft = temp1;
				temp3->topLeft = temp2;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topLeft;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->btmLeft = temp1;
				temp3->topLeft = temp2;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topLeft;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->btmLeft = temp1;
				temp3->topLeft = temp2;
			}
			else if (parent->topRight == curr)
			{
				parent->topRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topLeft;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->btmLeft = temp1;
				temp3->topLeft = temp2;
			}
			break;
		case 2:
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->btmLeft = temp1;
				temp3->topLeft = temp2;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->btmLeft = temp1;
				temp3->topLeft = temp2;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->btmLeft = temp1;
				temp3->topLeft = temp2;
			}
			else if (parent->topRight == curr)
			{
				parent->topRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->topLeft = temp2;
				temp3->btmLeft = temp1;
			}
			break;
		case 3:
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = curr->topRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topLeft;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp3->topLeft = temp2;
				temp3->btmLeft = temp1;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp1 = temp3->btmLeft;
				temp2 = temp3->topRight;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp1 = temp3->btmLeft;
				temp2 = temp3->topRight;
			}
			else if (parent->topRight == curr)
			{
				parent->topRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp1 = temp3->btmLeft;
				temp2 = temp3->topRight;
			}
			break;
		case 4:
			if (parent->btmLeft == curr)
			{
				parent->btmLeft = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp1 = temp3->btmLeft;
				temp2 = temp3->topRight;
			}
			else if (parent->topLeft == curr)
			{
				parent->topLeft = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp1 = temp3->btmLeft;
				temp2 = temp3->topRight;
			}
			else if (parent->btmRight == curr)
			{
				parent->btmRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp1 = temp3->btmLeft;
				temp2 = temp3->topRight;
			}
			else if (parent->topRight == curr)
			{
				parent->topRight = curr->btmRight;
				temp3 = parent->btmLeft;
				temp1 = curr->btmLeft;
				temp2 = curr->topRight;
				do
				{
					temp3 = temp3->btmLeft;
				} while (temp3);
				temp1 = temp3->btmLeft;
				temp2 = temp3->topRight;
			}
			break;
		default: ErrorMsg();
		}
		temp1 = temp2 = temp3 = curr = NULL;
		delete temp1, temp2, temp3, curr;
		size--;
	}

	//============================================================================================================================
	//==============================================CASE 5 - NODE WITH FOUR CHILDREN==============================================
	//============================================================================================================================

	else if (curr->btmLeft && curr->btmRight && curr->topLeft && curr->topRight)
	{
		Node<T> *temp1 = new Node<T>;
		Node<T> *temp2 = new Node<T>;
		Node<T> *temp3 = new Node<T>;
		Node<T> *temp4 = new Node<t>;

		switch (Parent(parent, curr))
		{
		case 1:
			parent->btmLeft = curr->btmRight;
			temp1 = curr->topRight;
			temp2 = curr->btmLeft;
			temp3 = curr->topLeft;
			temp4 = curr->btmLeft;
			do
			{
				temp4 = temp4->btmLeft;
			} while (temp4);
			temp4->btmRight = temp1;
			temp4->topRight = temp2;
			temp4->btmLeft = temp3;
			break;
		case 2:
			parent->btmRight = curr->btmRight;
			temp1 = curr->topRight;
			temp2 = curr->btmLeft;
			temp3 = curr->topLeft;
			temp4 = curr->btmLeft;
			do
			{
				temp4 = temp4->btmLeft;
			} while (temp4);
			temp4->btmRight = temp1;
			temp4->topRight = temp2;
			temp4->btmLeft = temp3;
			break;
		case 3:
			parent->topLeft = curr->btmRight;
			temp1 = curr->topRight;
			temp2 = curr->btmLeft;
			temp3 = curr->topLeft;
			temp4 = curr->btmLeft;
			do
			{
				temp4 = temp4->btmLeft;
			} while (temp4);
			temp4->btmRight = temp1;
			temp4->topRight = temp2;
			temp4->btmLeft = temp3;
			break;
		case 4:
			parent->topRight = curr->btmRight;
			temp1 = curr->topRight;
			temp2 = curr->btmLeft;
			temp3 = curr->topLeft;
			temp4 = curr->btmLeft;
			do
			{
				temp4 = temp4->btmLeft;
			} while (temp4);
			temp4->btmRight = temp1;
			temp4->topRight = temp2;
			temp4->btmLeft = temp3;
			break;
		default: ErrorMsg();
		}
		temp1 = temp2 = temp3 = temp4 = NULL;
		delete, temp1, temp2, temp3, temp4, curr;
		size--;
	}
}


template <class T>
Node<T> *Pyramid<T>::Search(T &data)
{
	Node<T> *current = apex;
	while (current)
	{
		if (current->data == data)
		{
			return current;
		}
		else
		{
			if (data < current->data)
			{
				if (current->btmLeft != NULL)
					current = current->btmLeft;
				else current = current->topLeft;
			}
			else
			{
				if (current->btmRight != NULL) 
					current = current->btmRight;
				else current = current->topRight;
			}

		}
	}
	return NULL;
}

template <class T>
size_t Pyramid<T>::Size()
{
	return size;
}

template <class T>
bool Pyramid<T>::isEmpty()
{
	return bool(apex == NULL);
}

template <class T>
void Pyramid<T>::Clear(Node<T> *node)
{
	if (node)
	{
		if (node->btmLeft) Clear(node->btmLeft);
		if (node->btmRight) Clear(node->btmRight);
		if (node->topLeft) Clear(node->topLeft);
		if (node->topRight) Clear(node->topRight);
		delete node;
	}
}

template <class T>
void Pyramid<T>::ErrorMsg()
{
	cerr << "Unhandled Exception!\tInformation Not Found...." << endl << endl << system("pause") << system("cls");
}

template <class T>
void Pyramid<T>::DepthFirstSearch()
{
	
	if (Apex())
		visit = true;
	depthFirstAssist(Apex(), visit);
	while (!myStack.isStackEmpty())
	{
		myStack.Display();
		myStack.Pop();
	}

}

template <class T>
void Pyramid<T>::depthFirstAssist(Node<T> &thisNode, bool visited)
{
	if (visited)
	{
		myStack.Push(thisNode);
	}
	while (thisNode)
	{
		if (thisNode->btmLeft)
		{
			thisNode->visited = true;
			depthFirstAssist(thisNode->btmLeft, thisNode->visited);
		}
		else if (thisNode->topLeft)
		{
			thisNode->visited = true;
			DepthFirstSearch(thisNode->topLeft, thisNode->visited);
		}
		if (thisNode->btmRight)
		{
			thisNode->visited = true;
			depthFirstAssist(thisNode->btmRight, thisNode->visited);
		}
		else if (thisNode->topRight)
		{
			thisNode->visited = true;
			depthFirstAssist(thisNode->topRight, thisNode->visited);
		}
	}
	
	
}

//========================================================================================================================
#endif

