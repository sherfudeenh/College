/*
Problem: Floyd-Warshall Algorithm
Goal: Find shortest paths between all pairs of vertices

Approach:
- Use adjacency matrix
- Try every node as intermediate

Time Complexity: O(n^3)
Space Complexity: O(n^2)
*/

#include <iostream>
#include <vector>
using namespace std;

#define INF 1e9

int main()
{
    int n;
    cin >> n;

    vector<vector<int>> dist(n, vector<int>(n));

    // Input adjacency matrix
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            cin >> dist[i][j];
        }
    }

    // Floyd-Warshall Algorithm
    for(int k = 0; k < n; k++)
    {
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(dist[i][k] < INF && dist[k][j] < INF)
                {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Output result
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(dist[i][j] == INF)
                cout << "∞ ";
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }

    // Negative cycle check
    for(int i = 0; i < n; i++)
    {
        if(dist[i][i] < 0)
        {
            cout << "Negative cycle detected\n";
            break;
        }
    }

    return 0;
}