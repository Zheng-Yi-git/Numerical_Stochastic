#include <iostream>
#include <vector>
using namespace std;

class MaxHeap
{
private:
    vector<int> heap; // Vector to store heap elements

    // Helper function to maintain max-heap property by moving up
    void heapifyUp(int index)
    {
        while (index > 0)
        {
            int parentIndex = (index - 1) / 2;
            if (heap[index] > heap[parentIndex])
            {
                swap(heap[index], heap[parentIndex]);
                index = parentIndex;
            }
            else
            {
                break;
            }
        }
    }

public:
    // Constructor
    MaxHeap() {}

    // Add an element to the heap
    void Add(int value)
    {
        heap.push_back(value);      // Add the new element at the end
        heapifyUp(heap.size() - 1); // Fix the heap property from the new element upward
    }

    // Utility function to print the heap
    void PrintHeap()
    {
        for (int val : heap)
        {
            cout << val << " ";
        }
        cout << endl;
    }
};

int main()
{
    MaxHeap maxHeap;

    // Adding elements to the heap
    maxHeap.Add(10);
    maxHeap.Add(20);
    maxHeap.Add(5);
    maxHeap.Add(30);
    maxHeap.Add(15);

    // Print the heap
    cout << "Max-Heap: ";
    maxHeap.PrintHeap();

    return 0;
}
