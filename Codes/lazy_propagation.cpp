#include <iostream>
#include <vector>

using namespace std;


typedef long long llint;

int N;
class SegmentTree
{
public:
    vector<llint> vec, lazy;
    SegmentTree()
    {
        vec.resize(4 * N + 1);
        lazy.resize(4 * N + 1);
    }

    void update(int idx, llint val, int node = 1, int l = 0, int r = N - 1)
    {
        vec[node] += val;
        if (l >= r)
            return;

        int mid = (l + r) / 2;
        if (idx <= mid)
            update(idx, val, node * 2, l, mid);
        else
            update(idx, val, node * 2 + 1, mid + 1, r);
    }

    void update_range(int left, int right, llint val, int node = 1, int l = 0, int r = N - 1)
    {
        if (r < left || l > right)
            return;
        
        if (left <= l && r <= right) {
            lazy[node] += val;
            return;
        }

        int mid = (l + r) / 2;
        update_range(left, right, val, node * 2, l, mid);
        update_range(left, right, val, node * 2 + 1, mid + 1, r);
    }  

    llint query(int left, int right, int node = 1, int l = 0, int r = N - 1)
    {
        if (r < left || l > right)
            return 0;
        
        vec[node] += lazy[node] * (r - l + 1);
        if (left <= l && r <= right) {
            lazy[node] = 0;
            return vec[node];
        }

        lazy[node * 2] += lazy[node];
        lazy[node * 2 + 1] += lazy[node];
        lazy[node] = 0;

        int mid = (l + r) / 2;
        llint lval = query(left, right, node * 2, l, mid);
        llint rval = query(left, right, node * 2 + 1, mid + 1, r);
        return lval + rval;
    }  
};


int main(void)
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> N;
    SegmentTree segment_tree;
    for (int i = 0; i < N; i++)
    {
        llint val;
        cin >> val;
        segment_tree.update(i, val);
    }

    int M;
    cin >> M;
    for (int i = 0; i < M; i++)
    {
        int q;
        cin >> q;
        if (q == 1) {
            llint l, r, val;
            cin >> l >> r >> val;
            segment_tree.update_range(l - 1, r - 1, val);
        }
        else {
            int idx;
            cin >> idx;
            cout << segment_tree.query(idx - 1, idx - 1) << "\n";
        }
    }
}