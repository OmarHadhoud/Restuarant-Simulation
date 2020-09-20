#ifndef __QUEUE_H_
#define __QUEUE_H_

/*This code is an updated version from "Data Abstraction & Problem Solving with C++,WALLS AND MIRRORS ,SIXTH EDITION"*/

/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					-------------
					| item| next | --->
					-------------
General Queue case:

				 frontPtr																backPtr
					\											   						/
					 \											  					   /
					------------- 	  ------------- 	  ------------- 	  -------------
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------

Empty Case:

				 frontptr	 backptr
						\	 /
						 \	/
					---- NULL ------


Single Node Case:
				 frontPtr	 backPtr
					\		/
					 \	   /
					--------
					|	|nxt -->NULL
					--------

*/

#include "Node.h"

template <typename T>
class Queue
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	Queue();
	Queue(const Queue<T> &rhs);
	void operator = (const Queue<T> &rhs);
	bool isEmpty() const;
	bool enqueue(T& newEntry);
	bool dequeue(T*& frntEntry);
	bool peekFront(T*& frntEntry)  const;
	int getCount() const;
	const T ** toArray() const; //Return an array of pointers of items, constant function as we don't want other classes to modify it.
	~Queue();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
Queue<T>::Queue()
{
	backPtr = nullptr;
	frontPtr = nullptr;
	count = 0;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue( Queue rhs )
The copy constructor of the Queue class
needed for deep copying

*/
template <typename T>
Queue<T>::Queue(const Queue<T> &rhs)
{
	if (rhs->isEmpty())
	{
		frontPtr = nullptr; backPtr = nullptr;
	}
	else
	{
		Node<T> *scptr = rhs->frontPtr;
		Node<T> *tmp = new Node<T>(scptr->getItemPtr());
		Node<T> *tmp2;
		frontPtr = tmp;
		scptr = scptr->getNext();
		while (scptr)
		{
			tmp2 = new Node<T>(scptr->getItemPtr());
			tmp->setNext(tmp2);
			tmp = tmp2;
			scptr = scptr->getNext();
		}
		backPtr = tmp;
	}
	count = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: operator =
The assignment operator of the Queue class
needed for deep copying

*/
template <typename T>
void Queue<T>::operator=(const Queue<T> &rhs)
{
	if (rhs->isEmpty())
	{
		frontPtr = nullptr; backPtr = nullptr;
	}
	else
	{
		Node<T> *scptr = rhs->frontPtr;
		Node<T> *tmp = new Node<T>(scptr->getItemPtr());
		Node<T> *tmp2;
		frontPtr = tmp;
		scptr = scptr->getNext();
		while (scptr)
		{
			tmp2 = new Node<T>(scptr->getItemPtr());
			tmp->setNext(tmp2);
			tmp = tmp2;
			scptr = scptr->getNext();
		}
		backPtr = tmp;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool Queue<T>::isEmpty() const
{
	if (frontPtr == nullptr)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool Queue<T>::enqueue(T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(&newEntry);
	// Insert the new node
	if (isEmpty())
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty
	backPtr = newNodePtr; // New node is at back
	count++;
	return true;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool Queue<T>::dequeue(T*& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItemPtr();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr;

	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;

	count--;
	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if Queue is empty
*/
template <typename T>
bool Queue<T>::peekFront(T*& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItemPtr();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
int  Queue<T>::getCount() const {
	return count;
}

template <typename T>
const T ** Queue<T>::toArray() const
{
	const T **listContents = new const T*[count];
	Node<T> *scptr = frontPtr;
	int i = 0;
	while (scptr != nullptr && (i < count))
	{
		listContents[i] = scptr->getItemPtr();
		scptr = scptr->getNext();
		i++;
	}
	return listContents;
}

template <typename T>
Queue<T>::~Queue()
{
}
#endif
