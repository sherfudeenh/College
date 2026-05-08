#include <iostream>
#include <vector>

void heapify(std::vector<int> &H, int n, int k)
{
    int v = H[k];
    bool heap = false;

    while (!heap && 2 * k <= n)
    {
        int j = 2 * k;

        if (j < n && H[j] < H[j + 1])
        {
            j = j + 1;
        }

        if (v >= H[j])
        {
            heap = true;
        }
        else
        {
            H[k] = H[j];
            k = j;
        }
    }

    H[k] = v;
}

void buildHeap(std::vector<int> &H, int n)
{
    for (int i = n / 2; i >= 1; i--)
    {
        heapify(H, n, i);
    }
}

void heapSort(std::vector<int> &H, int n)
{
    buildHeap(H, n);

    for (int i = n; i > 1; i--)
    {
        int temp = H[1];
        H[1] = H[i];
        H[i] = temp;

        heapify(H, i - 1, 1);
    }
}

int main()
{
    int n;
    std::cin >> n;

    std::vector<int> H(n + 1);

    for (int i = 1; i <= n; i++)
    {
        std::cin >> H[i];
    }

    buildHeap(H, n);

    std::cout << "Max Heap:\n";

    for (int i = 1; i <= n; i++)
    {
        if (i > 1)
        {
            std::cout << ' ';
        }

        std::cout << H[i];
    }

    std::cout << '\n';

    heapSort(H, n);

    std::cout << "Sorted Max Heap:\n";

    for (int i = 1; i <= n; i++)
    {
        if (i > 1)
        {
            std::cout << ' ';
        }

        std::cout << H[i];
    }

    std::cout << '\n';

    return 0;
}
