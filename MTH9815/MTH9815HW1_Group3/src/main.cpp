#include <iostream>
#include <string>
#include "LinkedList.h"
#include "DoublyLinkedList.h"
#include "HashTable.h"

int main() {
    
    // Exercise 1
    {
        std::cout << "LinkedList Tests:" << std::endl;
        // Create a LinkedList of integers
        LinkedList<int> list;

        // Test Add function
        int value1 = 10;
        int value2 = 20;
        int value3 = 30;
        list.Add(value1);
        list.Add(value2);
        list.Add(value3);
        std::cout << "After adding elements 10, 20, 30: ";
        ListIterator<int> it1 = list.Iterator();
        while (it1.HasNext()) {
            std::cout << it1.Next() << " ";
        }
        std::cout << "\n";

        // Test Insert function
        int value4 = 15;
        list.Insert(value4, 1); // Insert 15 at index 1
        std::cout << "After inserting 15 at index 1: ";
        ListIterator<int> it2 = list.Iterator();
        while (it2.HasNext()) {
            std::cout << it2.Next() << " ";
        }
        std::cout << "\n";

        // Test Get function
        int getIndex = 2;
        int& getValue = list.Get(getIndex);
        std::cout << "Element at index " << getIndex << ": " << getValue << "\n";

        // Test IndexOf function
        int indexOfValue = list.IndexOf(value2);
        std::cout << "Index of value 20: " << indexOfValue << "\n";

        // Test Remove function
        int removeIndex = 1;
        int& removedValue = list.Remove(removeIndex);
        std::cout << "Removed element at index " << removeIndex << ": " << removedValue << "\n";
        std::cout << "After removal: ";
        ListIterator<int> it3 = list.Iterator();
        while (it3.HasNext()) {
            std::cout << it3.Next() << " ";
        }
        std::cout << "\n";

        // Test Size function
        int listSize = list.Size();
        std::cout << "Size of the list: " << listSize << "\n";

        // Test Iterator
        std::cout << "Iterating through the list: ";
        ListIterator<int> iterator = list.Iterator();
        while (iterator.HasNext()) {
            std::cout << iterator.Next() << " ";
        }
        std::cout << "\n";
    }
    
    
    // Exercise 2
    {
        std::cout << "\nDoublyLinkedList Tests:" << std::endl;
        // Create a LinkedList of integers
        DoublyLinkedList<int> list;

        // Test Add function
        int value1 = 10;
        int value2 = 20;
        int value3 = 30;
        list.Add(value1);
        list.Add(value2);
        list.Add(value3);
        std::cout << "After adding elements 10, 20, 30: ";
        DoublyListIterator<int> it1 = list.Iterator();
        while (it1.HasNext()) {
            std::cout << it1.Next() << " ";
        }
        std::cout << "\n";

        // Test Insert function
        int value4 = 15;
        list.Insert(value4, 1); // Insert 15 at index 1
        std::cout << "After inserting 15 at index 1: ";
        DoublyListIterator<int> it2 = list.Iterator();
        while (it2.HasNext()) {
            std::cout << it2.Next() << " ";
        }
        std::cout << "\n";

        // Test Get function
        int getIndex = 2;
        int& getValue = list.Get(getIndex);
        std::cout << "Element at index " << getIndex << ": " << getValue << "\n";

        // Test IndexOf function
        int indexOfValue = list.IndexOf(value2);
        std::cout << "Index of value 20: " << indexOfValue << "\n";

        // Test Remove function
        int removeIndex = 1;
        int& removedValue = list.Remove(removeIndex);
        std::cout << "Removed element at index " << removeIndex << ": " << removedValue << "\n";
        std::cout << "After removal: ";
        DoublyListIterator<int> it3 = list.Iterator();
        while (it3.HasNext()) {
            std::cout << it3.Next() << " ";
        }
        std::cout << "\n";

        // Test Size function
        int listSize = list.Size();
        std::cout << "Size of the list: " << listSize << "\n";

        // Test Iterator
        std::cout << "Iterating through the list: ";
        DoublyListIterator<int> iterator = list.Iterator();
        while (iterator.HasNext()) {
            std::cout << iterator.Next() << " ";
        }
        std::cout << "\n";
    }

    // Exercise 3
    {
        std::cout << "\nHastable Tests:" << std::endl;
        // Create a hashtable for string keys and int values
    Hashtable<std::string, int> hashtable;

    // Insert some key-value pairs
    hashtable.insert("apple", 1);
    hashtable.insert("banana", 2);
    hashtable.insert("cherry", 3);

    // Find and print values
    int value;
    if (hashtable.find("apple", value)) {
        std::cout << "Value for 'apple': " << value << std::endl;
    } else {
        std::cout << "'apple' not found." << std::endl;
    }

    if (hashtable.find("banana", value)) {
        std::cout << "Value for 'banana': " << value << std::endl;
    } else {
        std::cout << "'banana' not found." << std::endl;
    }

    // Remove a key-value pair
    hashtable.remove("banana");

    // Try to find the removed key
    if (hashtable.find("banana", value)) {
        std::cout << "Value for 'banana': " << value << std::endl;
    } else {
        std::cout << "'banana' not found after removal." << std::endl;
    }

    // Test finding a non-existent key
    if (hashtable.find("grape", value)) {
        std::cout << "Value for 'grape': " << value << std::endl;
    } else {
        std::cout << "'grape' not found." << std::endl;
    }

    }

    return 0;
}