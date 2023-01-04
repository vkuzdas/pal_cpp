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



///  ########################
///  ## Depth-First Search ##
///  ########################


/// CHATGPT
void iterativeDFS(int start, vector<vector<int>>& adjacencyList) {
    stack<int> S;
    vector<bool> visited(adjacencyList.size(), false);

    S.push(start);
    visited[start] = true;

    while (!S.empty()) {
        int current = S.top();
        S.pop();

        // Process the current node here

        for (int neighbor : adjacencyList[current]) {
            if (!visited[neighbor]) {
                S.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}


/// CHATGPT
// One way to make the DFS traversal more efficient is to use a recursive
// approach, rather than an iterative one. Recursive DFS does not require
// the use of an explicit stack data structure, as the function calls themselves form the stack.
void recursiveDFS(int current, vector<vector<int>>& adjacencyList, vector<bool>& visited) {
    // Process the current node here
    visited[current] = true;

    for (int neighbor : adjacencyList[current]) {
        if (!visited[neighbor]) {
            recursiveDFS(neighbor, adjacencyList, visited);
        }
    }
}




///  ####################################
///  ## Depth-First Search -- COLORING ##
///  ####################################

// There are a few different ways that coloring can be
// used in DFS, but one common use is to mark nodes that
// have been visited or processed in some way. For example,
// you might use different colors to indicate nodes that have
// been visited but not yet finished processing, nodes that are
// currently being processed, and nodes that have been completely processed.

// use coloring in a DFS to solve a maze,
// to find all the connected components of a graph,
// or to check if a graph is bipartite.

enum Color { WHITE, GRAY, BLACK };
/// CHATGPT
void colorDFS(int start, vector<vector<int>>& adjacencyList) {
    stack<int> stack;
    vector<Color> colors(adjacencyList.size(), WHITE);

    stack.push(start);
    colors[start] = GRAY;

    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();

        // Process the current node here

        colors[current] = BLACK;

        for (int neighbor : adjacencyList[current]) {
            if (colors[neighbor] == WHITE) {
                stack.push(neighbor);
                colors[neighbor] = GRAY;
            }
            else if (colors[neighbor] == GRAY) {
                // Detect cycle
            }
        }
    }
}




///  ######################################
///  ## Depth-First Search -- Timestamps ##
///  ######################################

// Determine the order in which nodes were first visited by the DFS.
// Identify the shortest path between two nodes in a graph.
// Determine the length of the longest path in a graph.
// Calculate the total time it takes for the DFS to visit all the nodes in a graph.

int timeStamp = 0;
/// CHATGPT
void timestampDFS(int curr, vector<vector<int>>& adjacencyList, vector<int>& discoveryTimes, vector<int>& finishingTimes) {
    discoveryTimes[curr] = ++timeStamp;

    // Process the curr node here

    for (int neigh : adjacencyList[curr]) {
        if (discoveryTimes[neigh] == 0) {
            timestampDFS(neigh, adjacencyList, discoveryTimes, finishingTimes);
        }
    }

    finishingTimes[curr] = ++timeStamp;
}



