#include <iostream>
#include <vector>

std::vector<int> merge(const std::vector<int> &B, const std::vector<int> &C)
{
    std::size_t i = 0;
    std::size_t j = 0;
    std::vector<int> A;
    A.reserve(B.size() + C.size());

    while (i < B.size() && j < C.size())
    {
        if (B[i] <= C[j])
        {
            A.push_back(B[i++]);
        }
        else
        {
            A.push_back(C[j++]);
        }
    }

    while (i < B.size())
    {
        A.push_back(B[i++]);
    }
    while (j < C.size())
    {
        A.push_back(C[j++]);
    }

    return A;
}

std::vector<int> mergeSort(const std::vector<int> &arr)
{
    if (arr.size() <= 1)
    {
        return arr;
    }

    std::size_t mid = arr.size() / 2;
    std::vector<int> left(arr.begin(), arr.begin() + static_cast<long>(mid));
    std::vector<int> right(arr.begin() + static_cast<long>(mid), arr.end());

    return merge(mergeSort(left), mergeSort(right));
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

    std::vector<int> sorted = mergeSort(A);
    for (int i = 0; i < n; i++)
    {
        if (i)
        {
            std::cout << ' ';
        }
        std::cout << sorted[i];
    }
    std::cout << '\n';

    return 0;
}
