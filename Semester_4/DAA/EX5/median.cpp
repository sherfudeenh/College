#include <iostream>
#include <vector>

int lomutoPartition(std::vector<int> &A, int l, int r)
{
    int p = A[l];
    int s = l;

    for (int i = l + 1; i <= r; i++)
    {
        if (A[i] < p)
        {
            s++;

            int temp = A[s];
            A[s] = A[i];
            A[i] = temp;
        }
    }

    int temp = A[l];
    A[l] = A[s];
    A[s] = temp;

    return s;
}

int quickselect(std::vector<int> &A, int l, int r, int k)
{
    int s = lomutoPartition(A, l, r);

    if (s == l + k - 1)
    {
        return A[s];
    }
    else if (s > l + k - 1)
    {
        return quickselect(A, l, s - 1, k);
    }
    else
    {
        return quickselect(A, s + 1, r, k - (s - l + 1));
    }
}

int main()
{
    int n;
    std::cin >> n;

    std::vector<int> A(n);

    for (int i = 0; i < n; i++)
    {
        std::cin >> A[i];
    }

    int k;
    std::cin >> k;

    int result = quickselect(A, 0, n - 1, k);

    std::cout << k << "th smallest element is: ";
    std::cout << result << '\n';

    return 0;
}
