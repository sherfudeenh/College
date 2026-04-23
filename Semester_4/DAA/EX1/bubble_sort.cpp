#include <iostream>
#include <vector>

int main()
{
    int n;
    std::cin >> n;

    std::vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
    }

    for (int i = 0; i < n - 1; i++)
    {
        bool flag = true;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                flag = false;
            }
        }
        if (flag)
        {
            break;
        }
    }

    std::cout << "Sorted array:\n";
    for (int i = 0; i < n; i++)
    {
        if (i)
        {
            std::cout << ' ';
        }
        std::cout << a[i];
    }
    std::cout << '\n';

    return 0;
}
