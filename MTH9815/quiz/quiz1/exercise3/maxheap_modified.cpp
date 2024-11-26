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

    // Helper function to maintain max-heap property by moving down
    void heapifyDown(int index)
    {
        int size = heap.size();
        while (index < size)
        {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int largest = index;

            if (leftChild < size && heap[leftChild] > heap[largest])
            {
                largest = leftChild;
            }

            if (rightChild < size && heap[rightChild] > heap[largest])
            {
                largest = rightChild;
            }

            if (largest != index)
            {
                swap(heap[index], heap[largest]);
                index = largest;
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

    // Remove the maximum element (root) from the heap
    void Remove()
    {
        if (heap.empty())
        {
            cout << "Heap is empty. Cannot remove element." << endl;
            return;
        }

        // Replace the root with the last element
        heap[0] = heap.back();
        heap.pop_back(); // Remove the last element
        heapifyDown(0);  // Fix the heap property from the root downward
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

    cout << "Max-Heap after adding elements: ";
    maxHeap.PrintHeap();

    // Removing elements from the heap
    maxHeap.Remove();
    cout << "Max-Heap after removing the max element: ";
    maxHeap.PrintHeap();

    maxHeap.Remove();
    cout << "Max-Heap after removing another max element: ";
    maxHeap.PrintHeap();

    return 0;
}
