#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

const int INF = (1 << 30) - 1;


class Edge
{
public:
    Edge *reverse_edge = NULL;
    int dest, capacity, flow = 0;
    Edge(int dest, int capacity)
        : dest(dest), capacity(capacity) { }
    void add_flow(int value)
    {
        flow += value;
        reverse_edge->flow -= value;
    }
};
typedef Edge* EdgePtr;

class FlowNetwork
{
public:
    int n;
    vector<vector<EdgePtr>> edges;
    FlowNetwork(vector<vector<pair<int, int>>> &adj)
    {
        n = adj.size();
        edges.resize(n);
        for (int node = 0; node < n; node++)
        {
            for (auto &it:adj[node])
            {
                Edge *edge = new Edge(it.first, it.second);
                Edge *reverse_edge = new Edge(node, 0);
                edge->reverse_edge = reverse_edge;
                reverse_edge->reverse_edge = edge;

                edges[node].push_back(edge);
                edges[it.first].push_back(reverse_edge);
            }
        }   
    }

    pair<vector<int>, vector<int>> find_route(int source, int sink)
    {
        vector<int> parents(n, -1);
        vector<int> indicies(n, -1);
        parents[source] = source;
        indicies[source] = source;
        queue<int> q;
        q.push(source);
        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            for (int i = 0; i < edges[node].size(); i++)
            {
                auto &edge = *edges[node][i];
                int remain = edge.capacity - edge.flow;
                int next = edge.dest;
                if (remain > 0 && parents[next] == -1) {
                    parents[next] = node;
                    indicies[next] = i;
                    if (next == sink)
                        return {parents, indicies};
                    q.push(next);
                }
            }
        }

        return {parents, indicies};
    }
    
    int calculate_max_flow(int source, int sink)
    {
        int total = 0;
        while (true)
        {
            auto [parents, indicies] = find_route(source, sink);
            if (parents[sink] == -1)
                break;
            
            int min_remain = INF;
            for (int node = sink; node != source; node = parents[node])
            {
                int parent = parents[node];
                auto &edge = *edges[parent][indicies[node]];
                min_remain = min(min_remain, edge.capacity - edge.flow);
            }

            for (int node = sink; node != source; node = parents[node])
            {
                int parent = parents[node];
                auto &edge = *edges[parent][indicies[node]];
                edge.add_flow(min_remain);
            }
            total += min_remain;
        }
        return total;
    }
};