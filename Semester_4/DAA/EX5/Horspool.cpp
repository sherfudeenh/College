#include <iostream>
#include <string>
#include <vector>

const int SIZE = 256;

void shiftTable(const std::string &P, std::vector<int> &table)
{
    int m = P.length();

    for (int i = 0; i < SIZE; i++)
    {
        table[i] = m;
    }

    for (int j = 0; j < m - 1; j++)
    {
        table[(unsigned char)P[j]] = m - 1 - j;
    }
}

int horspool(const std::string &P, const std::string &T)
{
    int m = P.length();
    int n = T.length();

    std::vector<int> table(SIZE);

    shiftTable(P, table);

    int i = m - 1;

    while (i <= n - 1)
    {
        int k = 0;

        while (k < m && P[m - 1 - k] == T[i - k])
        {
            k++;
        }

        if (k == m)
        {
            return i - m + 1;
        }
        else
        {
            i = i + table[(unsigned char)T[i]];
        }
    }

    return -1;
}

int main()
{
    std::string text;
    std::string pattern;

    std::getline(std::cin, text);
    std::getline(std::cin, pattern);

    int pos = horspool(pattern, text);

    if (pos == -1)
    {
        std::cout << "Pattern not found\n";
    }
    else
    {
        std::cout << "Pattern found at Index: " << pos << '\n';
        std::cout << "Pattern found at position: " << pos + 1 << '\n';
    }

    return 0;
}
