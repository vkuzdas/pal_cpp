//
// Created by kuzdavoj@fel.cvut.cz on 9/17/2022.
//

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <cassert>

using namespace std;

vector<int> dfs(vector<vector<int>> &adj);
stack<int> top_sort(vector<vector<int>> &adj);
vector<vector<int>> reverse_edges(vector<vector<int>> adj);
void find_components(vector<vector<int>> &adj, vector<int> &compOf, int startVertex);
bool inSet(unordered_set<int> set, int v);

int main() {
    vector<vector<int>> adjOrig{{6}, {4}, {8}, {0}, {7}, {2}, {3}, {1}, {5}};
    vector<vector<int>> adjRev = reverse_edges(adjOrig);
    vector<vector<int>> expected{{3}, {7}, {5}, {6}, {1}, {8}, {0}, {4}, {2}};
    assert(adjRev == expected);

    int undefined = INT16_MIN;
    stack<int> rSort = top_sort(adjRev);
    vector<int> compOf(adjOrig.size(), undefined);
    while(!rSort.empty()) {
        if (compOf[rSort.top()] == undefined) {
            find_components(adjOrig, compOf, rSort.top());
        }
        rSort.pop();
    }
    vector<int> expectedComponents{0,1,2,0,1,2,0,1,2};
    assert(compOf == expectedComponents);

    return 0;
}

/**
 * kosaraju's implementation
 */
void find_components(vector<vector<int>> &adj, vector<int> &compOf, int startVertex) {
    int WHITE = 1, GRAY = 2, BLACK = 3;
    vector<int> colors(adj.size(), WHITE);
    stack<int> dfsStack;
    dfsStack.push(startVertex);

    while (!dfsStack.empty()) {
        int curr = dfsStack.top();
        if (colors[curr] == WHITE) {
            colors[curr] = GRAY;
        }
        else { // uzel je GRAY -> backTrack
            colors[curr] = BLACK;
            dfsStack.pop();
            compOf[curr] = startVertex;
        }
        for (int neighbor : adj[curr]) {
            if (colors[neighbor] == WHITE) {
                dfsStack.push(neighbor);
            }
        }
    }
}

vector<vector<int>> reverse_edges(vector<vector<int>> adj) {
    vector<vector<int>> reversed;
    for (vector<int> vec : adj) {
        reversed.emplace_back();
    }
    for (int srcVert = 0; srcVert < adj.size(); ++srcVert) {
        for (int i = 0; i < adj[srcVert].size(); ++i) {
            int destVert = adj[srcVert][i];
            reversed[destVert].push_back(srcVert);
        }
    }
    return reversed;
}

/**
 * iterativni (bez rekurze) topSort vyuzivajici zabarveni vertexu
 * WHITE = uzel ktery jsme jeste nezpracovali
 * GRAY  = uzel kteremu sousedy *budeme hazet na stack
 * BLACK = uzel ktereho sousedy jsme *hodili na stack
 *
 *      kazdy nenavstiveny uzel (WHITE) hodi na stack
 *      dokud stack neni prazdny, popuje jeho vrch
 *      white -> gray || gray -> black
 */
stack<int> top_sort(vector<vector<int>> &adj) {
    int WHITE = 1; // fresh
    int GRAY = 2;  // current
    int BLACK = 3; // closed

    vector<int> colors(adj.size(), WHITE);
    stack<int> dfsStack;
    stack<int> sort;

    for (int i = 0; i < adj.size(); i++) {
        if (colors[i] != WHITE) continue;
        dfsStack.push(i);

        while (!dfsStack.empty()) {
            int curr = dfsStack.top();

            if (colors[curr] == WHITE) { // 1) nenavstiveny node zabarvime
                colors[curr] = GRAY;
            }
            else { // 5) nyni je uzel sedivy -> sousedy jsme zpracovali
                colors[curr] = BLACK;
                dfsStack.pop();
                sort.push(curr);
            }

            for (int neighbor : adj[curr]) {      // 2) projdem sousedy
                if (colors[neighbor] == GRAY) {   // 3) pokud je soused zabarveny, nasli jsme cyklus
//                    return {}; /// tohle mozna zmenit pokud delame Kosaraju
                } else if (colors[neighbor] == WHITE) { // 4) fresh souseda hodime na dfsStack pro DFS
                    dfsStack.push(neighbor);
                }
            }
        }
    }
    return sort;
}

/**
 * iterativni (bez rekurze) DFS
 * pro kazdy uzel vola DFS:
 *      1) prida uzel na stack
 *      2) dokud stack neni prazdnej tak stack popuje
 *      3) zkontroluje jestli popnutej uzel nebyl navstiven a hodi na stack jeho sousedy (pokud nebyli navstiveni)
 */
vector<int> dfs(vector<vector<int>> &adj) {
    vector<int> path;
    unordered_set<int> seen;

    for (int startV = 0; startV < adj.size(); ++startV) { // pro uzly v grafu
        if (inSet(seen, startV)) continue;
        stack<int> stack({startV});
        while (!stack.empty()) { // pro uzly na stacku
            int curr = stack.top();
            stack.pop();
            seen.insert(curr);
            path.push_back(curr);
            for (int neighbor : adj[curr]) {
                if (inSet(seen, neighbor)) continue; // neighbor not seen yet (not found in seen)
                stack.push(neighbor);
            }
        }
    }
    return path;
}

bool inSet(unordered_set<int> set, int v) {
    if(set.find(v) != set.end()) return true;
    return false;
}
