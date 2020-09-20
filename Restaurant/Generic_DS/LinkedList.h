#pragma once
#include "Node.h"
template <typename T>
//A sorted list data structure implemented by Linked lists.

class LinkedList
{
private:
	int count;
	Node<T>* head;
public:
	LinkedList();
	LinkedList(const LinkedList<T> &rhs);
	void operator = (const LinkedList<T> &rhs);

	int getCount() const;
	bool isEmpty() const;
	bool insert(int pos, T &item); //Inserts an entry in the list in given position.
	int getPosition(T &item) const; //Index is starting at 1. If not found, return the index of it if it existed, but in negative.
	bool remove(int pos);
	void clear(); //clears the list
	T* getEntry(int pos);
	void setEntry(int pos, T&item);
	const T ** toArray() const; //Return an array of pointers of items, constant function as we don't want other classes to modify it.
	~LinkedList();
};

template <typename T>
LinkedList<T>::LinkedList() : count(0), head(nullptr)
{
}

/*
Function: SortedList( SortedList rhs )
The copy constructor of the SortedList class
needed for deep copying
*/
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &rhs)
{
	ascending = rhs.ascending;
	if (rhs->isEmpty())
		head = nullptr;
	else
	{
		Node<T> *scptr = rhs->head;
		Node<T> *tmp = new Node<T>(scptr->getItemPtr());
		if (!tmp) //If run out of memory.
		{
			clear();
			return;
		}count++;
		Node<T> *tmp2;
		head = tmp;
		scptr = scptr->getNext();
		while (scptr)
		{
			tmp2 = new Node<T>(scptr->getItemPtr());
			if (!tmp2) //If run out of memory
			{
				clear();
				return;
			}
			count++;
			tmp->setNext(tmp2);
			tmp = tmp2;
			scptr = scptr->getNext();
		}
	}
}

/*
Function: operator =
The assignment operator of the Queue class
needed for deep copying

*/
template <typename T>
void LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
	clear();
	if (rhs->isEmpty())
		head = nullptr;
	else
	{
		Node<T> *scptr = rhs->head;
		Node<T> *tmp = new Node<T>(scptr->getItemPtr());
		if (!tmp) return;
		count++;
		Node<T> *tmp2;
		head = tmp;
		scptr = scptr->getNext();
		while (scptr)
		{
			tmp2 = new Node<T>(scptr->getItemPtr());
			if (!tmp2) {
				tmp->setNext(nullptr);
				return;
			}
			count++;
			tmp->setNext(tmp2);
			tmp = tmp2;
			scptr = scptr->getNext();
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
int LinkedList<T>::getCount() const {
	return count;
}

template <typename T>
bool LinkedList<T>::isEmpty() const
{
	return count == 0;
}

template <typename T>
bool LinkedList<T>::insert(int pos, T &item)
{
	if (pos > count+1 || pos < 1) return false;
	Node<T> *entry = new Node<T>;
	//If no memory available
	if (!entry) return false;
	count++;
	entry->setItemPtr(&item);
	//If list is empty.
	if (!head)
	{
		head = entry;
		head->setNext(nullptr);
		return true;
	}
	if (pos == 1)
	{
		entry->setNext(head);
		head = entry;
		return true;
	}
	//Using a scan pointer to loop on list until position we need.
	Node<T> *scptr = head;
	int i = 1;
	while (i < pos-1)
	{
		scptr = scptr->getNext();
		i++;
	}
	entry->setNext(scptr->getNext());
	scptr->setNext(entry);
	return true;
}

template <typename T>
int LinkedList<T>::getPosition(T &item) const
{
	if (!head) return -1;
	int index = 1;
	Node<T> *scptr = head;

	while (scptr)
	{
		if (*scptr->getItemPtr() == item)
			break;
		index++; scptr = scptr->getNext();
	}
	if (scptr && *scptr->getItemPtr() == item)
		return index;
	return -1


}

template <typename T>
void LinkedList<T>::setEntry(int pos, T&item)
{
	if (pos > count || pos < 1) return;
	Node<T>*scptr = head;
	for (int i = 1; i < pos; i++)
	{
		scptr = scptr->getNext();
	}
	scptr->setItemPtr(item);
}



template <typename T>
T* LinkedList<T>::getEntry(int pos) {
	//if the "pos" is invalid
	if (pos > count || pos < 1)
		return NULL;
	Node<T> *scptr = head;
	for (int i = 1; i < pos; i++)
	{
		scptr = scptr->getNext();
	}
	return scptr->getItemPtr();
}

template <typename T>
bool LinkedList<T>::remove(int pos) {
	//if the "pos" is invalid
	if (pos > count || pos < 1)
		return false;
	Node<T> *scptr = head;
	if (pos == 1)
	{
		head = head->getNext();
		delete scptr;
		count--;
		return true;
	}
	//traverses till the element before the pos to delete
	for (int i = 1; i < pos - 1; i++)
	{
		scptr = scptr->getNext();
	}
	Node<T> *toDelete = scptr->getNext();
	scptr->setNext(toDelete->getNext());
	delete toDelete;
	count--;
	return true;
}

template <typename T>
const T ** LinkedList<T>::toArray() const
{
	const T **listContents = new const T*[count];
	Node<T> *scptr = head;
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
void LinkedList<T>::clear() {
	Node<T> *scptr = head;
	Node<T> *temp;
	while (scptr)
	{
		temp = scptr;
		scptr = scptr->getNext();
		delete temp;
	}
	count = 0;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
	clear();
}

