#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;


vector<vector<int>> edges, reversed_edges, components;
vector<bool> visited;
stack<int> visit_log;
void dfs(int node)
{
    visited[node] = true;
    for (int next:edges[node])
        if (!visited[next])
            dfs(next);
    visit_log.push(node);
}

void scc(int node)
{
    visited[node] = true;
    for (int next:reversed_edges[node])
        if (!visited[next])
            scc(next);
    components.back().push_back(node);
}

int main(void)
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int V, E;
    cin >> V >> E;

    edges.resize(V);
    reversed_edges.resize(V);
    for (int i = 0; i < E; i++)
    {
        int u, v;
        cin >> u >> v;
        u--; v--;
        edges[u].push_back(v);
        reversed_edges[v].push_back(u);
    }

    visited = vector<bool>(V, false);
    for (int node = 0; node < V; node++)
        if (!visited[node])
            dfs(node);

    visited = vector<bool>(V, false);
    while (!visit_log.empty())
    {
        int node = visit_log.top();
        visit_log.pop();
        if (!visited[node]) {
            components.push_back(vector<int>());
            scc(node);
        }
    }

    for (int i = 0; i < components.size(); i++)
        sort(components[i].begin(), components[i].end());
    sort(components.begin(), components.end());

    cout << components.size() << "\n";
    for (int i = 0; i < components.size(); i++)
    {
        for (int node:components[i])
            cout << node + 1 << " ";
        cout << "-1\n";
    }
}