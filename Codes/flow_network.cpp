#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

const int INF = 987654321;


typedef long long llint;

class FlowNetwork
{
public:
    const llint INF = (1ll << 60ll);

    int n;
    vector<vector<llint>> capacities, flows;
    FlowNetwork(vector<vector<llint>> &capacities): capacities(capacities)
    {
        n = capacities.size();
        flows.assign(n, vector<llint>(n, 0));
    }

    vector<int> find_route(int source, int sink)
    {
        vector<int> parents(n, -1);
        parents[source] = source;
        queue<int> q;
        q.push(source);
        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            for (int next = 0; next < n; next++)
            {
                int remain = capacities[node][next] - flows[node][next];
                if (remain > 0 && parents[next] == -1) {
                    parents[next] = node;
                    if (next == sink)
                        return parents;
                    q.push(next);
                }
            }
        }

        return parents;
    }
    
    llint calculate_max_flow(int source, int sink)
    {
        llint total = 0;
        while (true)
        {
            vector<int> parents = find_route(source, sink);
            if (parents[sink] == -1)
                break;
            
            llint min_remain = INF;
            for (int node = sink; node != source; node = parents[node])
            {
                int parent = parents[node];
                min_remain = min(min_remain, capacities[parent][node] - flows[parent][node]);
            }

            for (int node = sink; node != source; node = parents[node])
            {
                int parent = parents[node];
                flows[parent][node] += min_remain;
                flows[node][parent] -= min_remain;
            }

            total += min_remain;
        }

        return total;
    }

    llint send_flow(int node, int sink, llint flow, vector<bool> &visited)
    {
        if (node == sink)
            return flow;

        visited[node] = true;
        for (int next = 0; next < n; next++)
        {
            llint remain = min(flow, capacities[node][next] - flows[node][next]);
            if (remain > 0ll && !visited[next]) {                
                llint ret = send_flow(next, sink, remain, visited);
                if (ret > 0ll) {
                    flows[node][next] += ret;
                    flows[next][node] -= ret;
                    return ret;
                }
            }
        }
        return 0;
    }

    llint calculate_max_flow_dfs(int source, int sink)
    {
        llint total = 0;
        vector<bool> visited(n, false);
        while (llint flow = send_flow(source, sink, INF, visited) > 0)
        {
            total += flow;
            visited.assign(n, false);
        }

        return total;
    }
};
