#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

// Simple Graph DP: Shortest path in a DAG
// Nodes: 0, 1, 2, 3, 4
// Edges: (0->1, 1), (0->2, 2), (1->3, 1), (2->3, 3), (3->4, 2)
struct Edge {
    int to;
    int weight;
};

void graph_dp() {
    int num_nodes = 5;
    std::vector<std::vector<Edge>> adj(num_nodes);
    adj[0] = {{1, 1}, {2, 2}};
    adj[1] = {{3, 1}};
    adj[2] = {{3, 3}};
    adj[3] = {{4, 2}};

    std::vector<int> dist(num_nodes, 1e9);
    dist[0] = 0;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Topological order is 0, 1, 2, 3, 4 for this simple DAG
    for (int u = 0; u < num_nodes; ++u) {
        if (dist[u] == 1e9) continue;
        for (const auto& edge : adj[u]) {
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "Algorithm: Graph-DP (Shortest Path DAG)" << std::endl;
    std::cout << "Nodes: " << num_nodes << std::endl;
    std::cout << "Final_Distance: " << dist[num_nodes - 1] << std::endl;
    std::cout << "Time_us: " << duration << std::endl;
}

int main() {
    graph_dp();
    return 0;
}
