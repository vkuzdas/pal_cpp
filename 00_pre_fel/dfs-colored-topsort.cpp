//
// Created by kuzdavoj@fel.cvut.cz on 9/17/2022.
//

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <cassert>
#include <queue>

using namespace std;

vector<int> dfs(vector<vector<int>> &adj);
vector<int> bfs(vector<vector<int>> &adj);
vector<int> top_sort(vector<vector<int>> &adj);
bool inSet(unordered_set<int> set, int v);

int main() {
    vector<vector<int>> adj1 {{},{3},{1},{2,3,4},{3}};
    vector<vector<int>> adj2 {{3},{0},{},{2,5},{0,1},{},{},{6}};

    vector<int> dfsPath = dfs(adj2);
    vector<int> expected{0,3,5,2,1,4,6,7};
    assert(dfsPath == expected);

    vector<int> topsort = top_sort(adj2);
    expected = {5,2,3,0,1,4,6,7};
    assert(topsort == expected);

    vector<vector<int>> adj3{{3}, {7}, {5}, {6}, {1}, {8}, {0}, {4}, {2}};
    vector<int> topsortedadj = top_sort(adj3);
    vector<vector<int>> bfsArg{{1,2},{3},{4},{},{}};

    vector<int> dfsPath2 = dfs(bfsArg);
    expected = {0,2,4,1,3};
    assert(dfsPath2 == expected);

    vector<int> bfsPath = bfs(bfsArg);
    expected = {0,1,2,3,4};
    assert(bfsPath == expected);

    return 0;
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
 *
 * @param adj - graf jako adjList
 * @return topologicke ocislovani (poradi odzadu; tzn sink<-begin)
 */
vector<int> top_sort(vector<vector<int>> &adj) {
    int WHITE = 1; // fresh
    int GRAY = 2;  // curr ent
    int BLACK = 3; // closed

    vector<int> colors(adj.size(), WHITE);
    stack<int> stack;
    vector<int> sorted;

    for (int i = 0; i < adj.size(); i++) {
        if (colors[i] != WHITE) continue;
        stack.push(i);

        while (!stack.empty()) {
            int curr = stack.top();

            if (colors[curr] == WHITE) { // 1) nenavstiveny node zabarvime
                colors[curr] = GRAY;
            }
            else { // 5) nyni je uzel sedivy -> sousedy jsme zpracovali
                colors[curr] = BLACK;
                stack.pop();
                sorted.push_back(curr);
            }

            for (int neighbor : adj[curr]) {      // 2) projdem sousedy
                if (colors[neighbor] == GRAY) {   // 3) pokud je soused zabarveny, nasli jsme cyklus
//                    return {}; /// tohle mozna zmenit pokud delame Kosaraju
                } else if (colors[neighbor] == WHITE) { // 4) fresh souseda hodime na stack pro DFS
                    stack.push(neighbor);
                }
            }
            /// WARN: uzel jsme jeste nepopnuli!
        }
    }
    return sorted;
}


/**
 * iterativni (bez rekurze) DFS
 * pro kazdy uzel vola DFS:
 *      1) prida uzel na stack
 *      2) dokud stack neni prazdnej tak stack popuje
 *      3) zkontroluje jestli popnutej uzel nebyl navstiven a hodi na stack jeho sousedy (pokud nebyli navstiveni)
 *
 * @param adj - graf jako adjList
 * @return cesta grafem
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

vector<int> bfs(vector<vector<int>> &adj) {
    vector<int> path;
    unordered_set<int> seen;

    for (int startV = 0; startV < adj.size(); ++startV) { // pro uzly v grafu
        if (inSet(seen, startV)) continue;
        queue<int> q({startV});

        while (!q.empty()) { // pro uzly na stacku
            int curr = q.front();
            q.pop();
            seen.insert(curr);
            path.push_back(curr);
            for (int neighbor : adj[curr]) {
                if (inSet(seen, neighbor)) continue; // neighbor not seen yet (not found in seen)
                q.push(neighbor);
            }
        }
    }
    return path;
}

bool inSet(unordered_set<int> set, int v) {
    if(set.find(v) != set.end()) return true;
    return false;
}
