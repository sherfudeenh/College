#include <iostream>
#include <string>

int main()
{
    std::string text, pattern;
    std::cin >> text >> pattern;

    bool found = false;
    int n = static_cast<int>(text.length());
    int m = static_cast<int>(pattern.length());

    for (int i = 0; i <= n - m; i++)
    {
        int j = 0;
        for (; j < m; j++)
        {
            if (text[i + j] != pattern[j])
            {
                break;
            }
        }

        if (j == m)
        {
            std::cout << "Pattern found at starting index: " << i << '\n';
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "Pattern not found\n";
    }

    return 0;
}
