#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

typedef long long llint;


class SegmentTree
{
public:
    int n;
    vector<llint> nodes;

    SegmentTree(): n(0) { }
    SegmentTree(int n): n(n)
    {
        nodes.resize(4 * n + 1);
    }

    void update(int idx, llint val)
    {
        sub_update(idx, val, 1, 0, n - 1);
    }

    void sub_update(int idx, llint val, int node, int l, int r)
    {
        if (l >= r) {
            nodes[node] = val;
            return;
        }

        int mid = (l + r) / 2;
        int left_node = node * 2;
        int right_node = node * 2 + 1;
        if (idx <= mid)
            sub_update(idx, val, left_node, l, mid);
        else
            sub_update(idx, val, right_node, mid + 1, r);
        nodes[node] = max(nodes[left_node], nodes[right_node]);
    }

    llint query(int left, int right)
    {
        return sub_query(left, right, 1, 0, n - 1);
    }

    llint sub_query(int left, int right, int node, int l, int r)
    {
        if (left <= l && r <= right)
            return nodes[node];
        
        if (r < left || right < l)
            return 0;
        
        int mid = (l + r) / 2;
        return max(sub_query(left, right, node * 2, l, mid)
                , sub_query(left, right, node * 2 + 1, mid + 1, r));
    }
};