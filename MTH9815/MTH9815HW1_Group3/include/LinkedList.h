#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

// Node class represents a single element in the linked list
// T: Type of data stored in the node
template <typename T>
class Node {
public:
	T& data;              // Reference to the stored data
	Node<T>* next;        // Pointer to the next node in the list

	// Constructor initializes node with data and null next pointer
	// Parameters:
	//   value: Reference to data to be stored
	Node(T& value) : data(value), next(nullptr) {}
};

// Forward declaration of ListIterator for use in LinkedList class
template <typename T>
class ListIterator;

// LinkedList class implements a singly linked list data structure
// T: Type of elements stored in the list
template <typename T>
class LinkedList {
private:
	Node<T>* head;        // Pointer to the first node in the list
	int size;             // Current number of elements in the list

public:
	// Constructor initializes an empty list
	LinkedList() : head(nullptr), size(0) {}

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
	// Returns: ListIterator pointing to the beginning of the list
	ListIterator<T> Iterator();

	// Returns the current number of elements in the list
	// Returns: Integer representing the list size
	int Size() { return size; }
};

// ListIterator class provides iteration functionality for LinkedList
// T: Type of elements being iterated over
template <typename T>
class ListIterator {
private:
	Node<T>* current;     // Pointer to the current node in iteration

public:
	// Constructor initializes iterator with starting node
	// Parameters:
	//   start: Pointer to the first node to iterate from
	ListIterator(Node<T>* start) : current(start) {}

	// Checks if there are more elements to iterate over
	// Returns: true if there are more elements, false otherwise
	bool HasNext();

	// Returns the next element in the iteration
	// Returns: Reference to the next element
	// Throws: std::out_of_range if there are no more elements
	T& Next();
};

// Implementation of LinkedList methods
template <typename T>
void LinkedList<T>::Add(T& value) {
	Node<T>* newNode = new Node<T>(value);
	if (!head) {
		head = newNode;    // If list is empty, set new node as head
	}
	else {
		// Traverse to the end of the list
		Node<T>* temp = head;
		while (temp->next)
			temp = temp->next;
		temp->next = newNode;
	}
	size++;
}

template <typename T>
void LinkedList<T>::Insert(T& value, int index) {
	// Validate index bounds
	if (index < 0 || index > size)
		throw std::out_of_range("Index out of range");

	Node<T>* newNode = new Node<T>(value);

	// Handle insertion at the beginning of the list
	if (index == 0) {
		newNode->next = head;
		head = newNode;
	}
	else {
		// Traverse to insertion point
		Node<T>* temp = head;
		for (int i = 0; i < index - 1; ++i)
			temp = temp->next;
		newNode->next = temp->next;
		temp->next = newNode;
	}
	size++;
}

template <typename T>
T& LinkedList<T>::Get(int index) {
	// Validate index bounds
	if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	// Traverse to the requested index
	Node<T>* temp = head;
	for (int i = 0; i < index; ++i)
		temp = temp->next;

	return temp->data;
}

template <typename T>
int LinkedList<T>::IndexOf(T& value) {
	Node<T>* temp = head;
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
T& LinkedList<T>::Remove(int index) {
	// Validate index bounds
	if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	Node<T>* toDelete;

	// Handle removal from the beginning of the list
	if (index == 0) {
		toDelete = head;
		head = head->next;
	}
	else {
		// Traverse to the node before the one to be deleted
		Node<T>* temp = head;
		for (int i = 0; i < index - 1; ++i)
			temp = temp->next;
		toDelete = temp->next;
		temp->next = toDelete->next;
	}

	// Store data before deleting node
	T& data = toDelete->data;
	delete toDelete;
	size--;

	return data;
}

// Implementation of ListIterator methods
template <typename T>
ListIterator<T> LinkedList<T>::Iterator() {
	return ListIterator<T>(head);
}

template <typename T>
bool ListIterator<T>::HasNext() {
	return current != nullptr;
}

template <typename T>
T& ListIterator<T>::Next() {
	// Check if there are more elements
	if (!current)
		throw std::out_of_range("No more elements");

	// Store current data and advance iterator
	T& data = current->data;
	current = current->next;
	return data;
}

#endif // LINKEDLIST_H