//
// Created by kuzdavoj@fel.cvut.cz on 12/21/2022.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <queue>
#include <stack>

using namespace std;

///  #########################
///  ## Bredth-First Search ##
///  #########################

void iterativeBFS(int start, vector<vector<int>>& adjacencyList) {
    queue<int> Q;
    vector<bool> visited(adjacencyList.size(), false);

    Q.push(start);
    visited[start] = true;

    while (!Q.empty()) {
        int current = Q.front();
        Q.pop();

        // Process the current node here

        for (int neighbor : adjacencyList[current]) {
            if (!visited[neighbor]) {
                Q.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}




///  ############################################
///  ## Bredth-First Search  -- Shortest Paths ##
///  ############################################



void shortestBFS(int start, vector<vector<int>>& adjacencyList, vector<int>& parents) {
    queue<int> Q;
    vector<bool> visited(adjacencyList.size(), false);

    Q.push(start);
    visited[start] = true;

    while (!Q.empty()) {
        int curr = Q.front();        Q.pop();

        // Process the curr node here

        for (int neighbor : adjacencyList[curr]) {
            if (!visited[neighbor]) {
                Q.push(neighbor);
                visited[neighbor] = true;
                parents[neighbor] = curr;
            }
        }
    }
    // After the BFS traversal is complete,
    // the predecessors array will contain
    // the shortest path from the starting
    // node to each of the other nodes in the
    // graph. To reconstruct the shortest
    // path from the starting node to a specific target
    // node, you can follow the chain of predecessors
    // starting from the target node and working backwards.
}


///  #####################################
///  ## Bredth-First Search  -- CP-ALGO ##
///  #####################################


int _main() {
    vector<vector<int>> adj;  // adjacency list representation
    int n; // number of nodes
    int s; // source vertex

    queue<int> Q;
    vector<bool> visited(n);
    vector<int> shortestPaths(n);
    vector<int> parents(n);

    Q.push(s);
    visited[s] = true;
    parents[s] = -1;
    while (!Q.empty()) {
        int v = Q.front();
        Q.pop();
        for (int u : adj[v]) {
            if (!visited[u]) {
                visited[u] = true;
                Q.push(u);
                shortestPaths[u] = shortestPaths[v] + 1;
                parents[u] = v;
            }
        }
    }

    // shortest path from the source to some vertex V?
    // it can be done in the following manner:
    int V = 5;
    if (!visited[V]) {
        cout << "No path!";
    } else {
        vector<int> path;
        for (int v = V; v != -1; v = parents[v])
            path.push_back(v);
        reverse(path.begin(), path.end());
        cout << "Path: ";
        for (int v : path)
            cout << v << " ";
    }
}