#include <iostream>
#include <vector>

int n;
int W;

std::vector<int> item;
std::vector<int> w;
std::vector<int> v;
std::vector<float> ratio;

int maxProfit = 0;

std::vector<int> bestSet;
std::vector<int> include;

float bound(int level, int weight, int profit)
{
    if (weight >= W)
    {
        return 0;
    }

    float ub = profit;

    if (level + 1 < n)
    {
        ub += (W - weight) * ratio[level + 1];
    }

    return ub;
}

void knapsack(int level, int weight, int profit)
{
    if (weight <= W && profit > maxProfit)
    {
        maxProfit = profit;

        for (int i = 0; i < n; i++)
        {
            bestSet[i] = include[i];
        }
    }

    if (level == n - 1)
    {
        return;
    }

    float ub = bound(level, weight, profit);

    if (ub <= maxProfit)
    {
        return;
    }

    include[level + 1] = 1;

    knapsack(level + 1,
             weight + w[level + 1],
             profit + v[level + 1]);

    include[level + 1] = 0;

    knapsack(level + 1, weight, profit);
}

int main()
{
    std::cin >> n;

    item.resize(n);
    w.resize(n);
    v.resize(n);
    ratio.resize(n);

    bestSet.resize(n, 0);
    include.resize(n, 0);

    for (int i = 0; i < n; i++)
    {
        std::cin >> item[i]
                 >> w[i]
                 >> v[i]
                 >> ratio[i];
    }

    std::cin >> W;

    knapsack(-1, 0, 0);

    std::cout << "Selected items:\n";

    for (int i = 0; i < n; i++)
    {
        if (bestSet[i] == 1)
        {
            std::cout << "Item " << item[i]
                      << " (W=" << w[i]
                      << ", V=" << v[i] << ")\n";
        }
    }

    std::cout << "Maximum Profit = ";
    std::cout << maxProfit << '\n';

    return 0;
}
