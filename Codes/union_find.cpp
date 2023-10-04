#include <vector>

using namespace std;


class UnionFind
{
public:
    vector<int> parents;
    UnionFind(int n)
    {
        parents.resize(n);
        for (int i = 0; i < n; i++)
            parents[i] = i;
    }

    int find(int node)
    {
        int parent = parents[node];
        if (parent == node)
            return node;
        return parents[node] = find(parent);
    }

    bool merge(int a, int b)
    {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb)
            return false;

        parents[ra] = rb;
        return true;
    }
};