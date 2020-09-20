#pragma once
#include "Node.h"
template <typename T, bool _asc = true>
//A sorted list data structure implemented by Linked lists.

class SortedList
{
private:
	bool ascending;
	int count;
	Node<T>* head;
public:
	SortedList();
	SortedList(const SortedList<T,_asc> &rhs);
	void operator = (const SortedList<T,_asc> &rhs);

	int getCount() const;
	bool isEmpty() const;
	bool insertSorted(T &item); //Inserts an entry in its position to keep the list sorted.
	int getPosition(T &item) const; //Index is starting at 1. If not found, return the index of it if it existed, but in negative.
	bool removeSorted(T &item);
	void clear(); //clears the list
	T* getEntry(int pos);
	bool remove(int pos);
	const T ** toArray() const; //Return an array of pointers of items, constant function as we don't want other classes to modify it.
	~SortedList();
};

template <typename T, bool _asc = true>
SortedList<T,_asc>::SortedList() : count(0), head(nullptr), ascending(_asc)
{
}

/*
Function: SortedList( SortedList rhs )
The copy constructor of the SortedList class
needed for deep copying
*/
template <typename T, bool _asc = true>
SortedList<T,_asc>::SortedList(const SortedList<T, _asc> &rhs)
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
template <typename T, bool _asc = true>
void SortedList<T,_asc>::operator=(const SortedList<T,_asc> &rhs)
{
	ascending = rhs.ascending;
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
template <typename T, bool _asc = true>
int SortedList<T,_asc>::getCount() const {
	return count;
}

template <typename T, bool _asc = true>
bool SortedList<T, _asc>::isEmpty() const
{
	return count == 0;
}


template <typename T, bool _asc = true>
bool SortedList<T, _asc>::insertSorted(T &item)
{
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
	//If this is an ascendingly sorted list.
	if (ascending)
	{
		//If item should be inserted at the beginning of the list
		if (item < *head->getItemPtr())
		{
			entry->setNext(head);
				head = entry;
			return true;
		}
		//Using a scan pointer to loop on list until item is smaller than scptr value to insert it.
		Node<T> *scptr = head;
		while (scptr->getNext() && *scptr->getNext()->getItemPtr() < item)
		{
			scptr = scptr->getNext();
		}
		entry->setNext(scptr->getNext());
		scptr->setNext(entry);
		return true;
	}
	else if(!ascending)
	{
		//If item should be inserted at the beginning of the list
		if (*head->getItemPtr() < item)
		{
			entry->setNext(head);
			head = entry;
			return true;
		}
		//Using a scan pointer to loop on list until item is smaller than scptr value to insert it.
		Node<T> *scptr = head;
		while (scptr->getNext() && item < *scptr->getNext()->getItemPtr())
		{
			scptr = scptr->getNext();
		}
		entry->setNext(scptr->getNext());
		scptr->setNext(entry);
		return true;
	}
}

template <typename T, bool _asc = true>
int SortedList<T,_asc>::getPosition(T &item) const
{
 	if (!head) return -1;
	int index = 1;
	Node<T> *scptr = head;
	if (ascending)
	{
		while (scptr)
		{
			if (*scptr->getItemPtr() == item || item < *scptr->getItemPtr())
				break;
			index++; scptr = scptr->getNext();
		}
		if (scptr && *scptr->getItemPtr() == item)
			return index;
		else
			return (-1 * index);
	}
	else if(!ascending)
	{
		while (scptr)
		{
			if (*scptr->getItemPtr() == item || *scptr->getItemPtr()<item)
				break;
			index++; scptr = scptr->getNext();
		}
		if (scptr && *scptr->getItemPtr() == item)
			return index;
		else
			return (-1 * index);
	}
}

template <typename T, bool _asc = true>
bool SortedList<T,_asc>::removeSorted(T &item) {
	Node<T> *scptr = head;
	//If it's the first element
	if (*head->getItemPtr() == item)
	{
		head = head->getNext();
		delete scptr;
		count--;
		return true;
	}
	if (ascending)
	{
		//Using a scan pointer to loop on list until the item value is passed or found to remove it.
		while (scptr &&scptr->getNext() && scptr->getNext()->getItemPtr() <= item)
		{
			if (*scptr->getNext()->getItemPtr() == item)
			{
				Node<T> toDelete = scptr->getNext();
				scptr->setNext(toDelete->getNext);
				delete toDelete;
				count--;
				return true;
			}
			scptr = scptr->getNext();
		}
		//if not found
		return false;
	}
	else if (!ascending)
	{
		//Using a scan pointer to loop on list until the item value is passed or found to remove it.
		while (scptr &&scptr->getNext() && item <= scptr->getNext()->getItemPtr() )
		{
			if (*scptr->getNext()->getItemPtr() == item)
			{
				Node<T> toDelete = scptr->getNext();
				scptr->setNext(toDelete->getNext);
				delete toDelete;
				count--;
				return true;
			}
			scptr = scptr->getNext();
		}
		//if not found
		return false;
	}
}

template <typename T, bool _asc = true>
T* SortedList<T,_asc>::getEntry(int pos) {
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

template <typename T, bool _asc = true>
bool SortedList<T,_asc>::remove(int pos) {
	//if the "pos" is invalid
	if (pos > count || pos<1)
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

template <typename T, bool _asc = true>
const T ** SortedList<T,_asc>::toArray() const
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


template <typename T, bool _asc = true>
void SortedList<T,_asc>::clear() {
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

template <typename T, bool _asc = true>
SortedList<T,_asc>::~SortedList()
{
	clear();
}

