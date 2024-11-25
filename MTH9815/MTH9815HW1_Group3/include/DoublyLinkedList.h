#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "LinkedList.h"

// Node class for doubly linked list implementation
// Contains data and pointers to both next and previous nodes
// T: Type of data stored in the node
template <typename T>
class DNode {
public:
	T& data;              // Reference to the stored data
	DNode<T>* next;       // Pointer to the next node
	DNode<T>* prev;       // Pointer to the previous node

	// Constructor initializes node with data and null pointers
	// Parameters:
	//   value: Reference to data to be stored
	DNode(T& value) : data(value), next(nullptr), prev(nullptr) {}
};

// Forward declaration of DoublyListIterator for use in DoublyLinkedList class
template <typename T>
class DoublyListIterator;

// DoublyLinkedList class implements a doubly linked list data structure
// Allows bidirectional traversal through prev/next pointers
// T: Type of elements stored in the list
template <typename T>
class DoublyLinkedList {
private:
	int size;           // Current number of elements in the list
	DNode<T>* head;     // Pointer to the first node
	DNode<T>* tail;     // Pointer to the last node

public:
	// Constructor initializes an empty list
	DoublyLinkedList() : size(0), head(nullptr), tail(nullptr) {}

	// Adds an element to the end of the list
	// Parameters:
	//   value: Reference to the element to be added
	void Add(T& value);

	// Inserts an element at the specified position in the list
	// Parameters:
	//   value: Reference to the element to be inserted
	//   index: Position at which to insert the element (0-based)
	// Throws: std::out_of_range if index is invalid
	void Insert(T& value, int index);

	// Retrieves the element at the specified position
	// Parameters:
	//   index: Position of the element to retrieve (0-based)
	// Returns: Reference to the element at the specified position
	// Throws: std::out_of_range if index is invalid
	T& Get(int index);

	// Searches for the specified element in the list
	// Parameters:
	//   value: Reference to the element to search for
	// Returns: Index of the first occurrence of the element, or -1 if not found
	int IndexOf(T& value);

	// Removes and returns the element at the specified position
	// Parameters:
	//   index: Position of the element to remove (0-based)
	// Returns: Reference to the removed element
	// Throws: std::out_of_range if index is invalid
	T& Remove(int index);

	// Creates and returns an iterator for this list
	// Returns: DoublyListIterator pointing to the beginning of the list
	DoublyListIterator<T> Iterator();

	// Returns the current number of elements in the list
	// Returns: Integer representing the list size
	int Size() { return size; }
};

// DoublyListIterator class provides iteration functionality for DoublyLinkedList
// T: Type of elements being iterated over
template <typename T>
class DoublyListIterator {
private:
	DNode<T>* current;     // Pointer to the current node in iteration

public:
	// Constructor initializes iterator with starting node
	// Parameters:
	//   start: Pointer to the first node to iterate from
	DoublyListIterator(DNode<T>* start) : current(start) {}

	// Checks if there are more elements to iterate over
	// Returns: true if there are more elements, false otherwise
	bool HasNext();

	// Returns the next element in the iteration
	// Returns: Reference to the next element
	// Throws: std::out_of_range if there are no more elements
	T& Next();
};

// Implementation of DoublyLinkedList methods
template <typename T>
void DoublyLinkedList<T>::Add(T& value) {
	DNode<T>* newNode = new DNode<T>(value);
	if (!size) {
		// If list is empty, set both head and tail to new node
		head = newNode;
	}
	else {
		// Link new node to tail and update tail pointer
		tail->next = newNode;
		newNode->prev = tail;
	}
	tail = newNode;
	size++;
}

template <typename T>
void DoublyLinkedList<T>::Insert(T& value, int index) {
	// Validate index bounds
	if (index < 0 || index > this->size)
		throw std::out_of_range("Index out of range");

	DNode<T>* newNode = new DNode<T>(value);

	if (index == 0) {
		// Handle insertion at the beginning of the list
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
	else if (index == size) {
		// Handle insertion at the end of the list
		tail->next = newNode;
		tail = newNode;
	}
	else {
		// Handle insertion in the middle of the list
		DNode<T>* temp = head;
		for (int i = 0; i < index - 1; ++i)
			temp = temp->next;
		DNode<T>* temp2 = temp->next;
		// Update bidirectional links
		newNode->next = temp2;
		temp2->prev = newNode;
		temp->next = newNode;
		newNode->prev = temp2;
	}
	size++;
}

template <typename T>
T& DoublyLinkedList<T>::Get(int index) {
	// Validate index bounds
	if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	// Traverse to the requested index
	DNode<T>* temp = head;
	for (int i = 0; i < index; ++i)
		temp = temp->next;

	return temp->data;
}

template <typename T>
int DoublyLinkedList<T>::IndexOf(T& value) {
	DNode<T>* temp = head;
	int index = 0;
	// Traverse list looking for matching value
	while (temp) {
		if (temp->data == value)
			return index;
		temp = temp->next;
		index++;
	}
	return -1;  // Value not found
}

template <typename T>
T& DoublyLinkedList<T>::Remove(int index) {
	// Validate index bounds
	if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	DNode<T>* toDelete;

	if (index == 0) {
		// Handle removal from the beginning of the list
		toDelete = head;
		toDelete->next->prev = nullptr;
		head = head->next;
	}
	else if (index == size-1) {
		// Handle removal from the end of the list
		toDelete = tail;
		toDelete->prev->next = nullptr;
		tail = tail->prev;
	}
	else {
		// Handle removal from the middle of the list
		DNode<T>* temp = head;
		for (int i = 0; i < index - 1; ++i)
			temp = temp->next;
		toDelete = temp->next;
		DNode<T>* temp2 = toDelete->next;
		// Update bidirectional links
		temp->next = temp2;
		temp2->prev = temp;
	}

	// Store data before deleting node
	T& data = toDelete->data;
	delete toDelete;
	size--;

	return data;
}

// Implementation of DoublyListIterator methods
template <typename T>
DoublyListIterator<T> DoublyLinkedList<T>::Iterator() {
	return DoublyListIterator<T>(head);
}

template <typename T>
bool DoublyListIterator<T>::HasNext() {
	return current != nullptr;
}

template <typename T>
T& DoublyListIterator<T>::Next() {
	// Check if there are more elements
	if (!current)
		throw std::out_of_range("No more elements");

	// Store current data and advance iterator
	T& data = current->data;
	current = current->next;
	return data;
}

#endif // DOUBLYLINKEDLIST_H
