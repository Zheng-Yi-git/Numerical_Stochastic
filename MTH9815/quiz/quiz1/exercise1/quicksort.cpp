#include <iostream>
#include <vector>
using namespace std;

// Function to partition the array
int partition(vector<int> &arr, int low, int high)
{
    int pivot = arr[high]; // Choosing the last element as pivot
    int i = low - 1;       // Index of smaller element

    for (int j = low; j < high; j++)
    {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot)
        {
            i++; // Increment index of smaller element
            swap(arr[i], arr[j]);
        }
    }
    // Swap the pivot element with the element at i+1
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Function implementing quicksort
void quicksort(vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        // Partition the array
        int pi = partition(arr, low, high);

        // Recursively sort elements before and after partition
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Utility function to print the array
void printArray(const vector<int> &arr)
{
    for (int num : arr)
    {
        cout << num << " ";
    }
    cout << endl;
}

// Main function
int main()
{
    vector<int> arr = {10, 7, 8, 9, 1, 5};
    int n = arr.size();

    cout << "Original array: ";
    printArray(arr);

    quicksort(arr, 0, n - 1);

    cout << "Sorted array: ";
    printArray(arr);

    return 0;
}
