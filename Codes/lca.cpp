#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;


vector<vector<int>> get_sparse_table(vector<int> &vec, int size)
{
    vector<vector<int>> table(size, vector<int>(vec.size()));
    for (int i = 0; i < vec.size(); i++)
        table[0][i] = vec[i];

    for (int i = 1; i < size; i++)
        for (int j = 0; j < vec.size(); j++)
            table[i][j] = table[i - 1][table[i - 1][j]];
    return table;
}

void dfs(int node, int depth, vector<vector<int>> &edges, vector<bool> &visited, vector<int> &parents, vector<int> &depths)
{
    visited[node] = true;
    for (int next:edges[node])
        if (!visited[next]) {
            parents[next] = node;
            depths[next] = depth + 1;
            dfs(next, depth + 1, edges, visited, parents, depths);
        }
}

int get_lca(int u, int v, vector<vector<int>> &table, vector<int> &depths)
{
    if (depths[v] > depths[u])
        swap(v, u);

    int du = depths[u];
    int dv = depths[v];
    if (du > dv) {
        int gap = du - dv;
        for (int i = 0; i < table.size(); i++)
            if (gap & (1 << i))
                u = table[i][u];
    }
    if (u == v)
        return u;
        
    for (int i = table.size() - 1; i >= 0; i--)
    {
        if (table[i][u] != table[i][v]) {
            u = table[i][u];
            v = table[i][v];
        }
    }

    return table[0][u];
}