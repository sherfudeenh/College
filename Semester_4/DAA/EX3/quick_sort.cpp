#include <iostream>
#include <vector>

int hoarePartition(std::vector<int> &arr, int low, int high)
{
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;

    while (true)
    {
        do
        {
            i++;
        } while (arr[i] < pivot);

        do
        {
            j--;
        } while (arr[j] > pivot);

        if (i >= j)
        {
            return j;
        }

        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void quickSort(std::vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int p = hoarePartition(arr, low, high);
        quickSort(arr, low, p);
        quickSort(arr, p + 1, high);
    }
}

int main()
{
    std::vector<int> arr = {10, 7, 8, 9, 1, 5};
    quickSort(arr, 0, static_cast<int>(arr.size()) - 1);

    std::cout << "Sorted array: ";
    for (std::size_t i = 0; i < arr.size(); i++)
    {
        if (i)
        {
            std::cout << ' ';
        }
        std::cout << arr[i];
    }
    std::cout << '\n';

    return 0;
}
