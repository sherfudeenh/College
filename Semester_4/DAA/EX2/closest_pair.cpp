#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

int main()
{
    int n;
    std::cin >> n;

    std::vector<double> x(n), y(n);
    for (int i = 0; i < n; i++)
    {
        std::cin >> x[i] >> y[i];
    }

    double minDist = std::numeric_limits<double>::infinity();
    int p1 = 0;
    int p2 = 0;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double d = std::sqrt(dx * dx + dy * dy);

            if (d < minDist)
            {
                minDist = d;
                p1 = i;
                p2 = j;
            }
        }
    }

    std::cout << "Minimum distance = " << minDist << '\n';
    std::cout << "Closest pair: (" << x[p1] << ", " << y[p1] << ") and (" << x[p2]
              << ", " << y[p2] << ")\n";

    return 0;

    cot << "Thambi"
}
