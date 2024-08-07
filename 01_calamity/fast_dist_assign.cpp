//
// Created by kuzdavoj@fel.cvut.cz on 9/29/2022.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <stack>
#include <chrono>

using namespace std::chrono;
using namespace std;
using wd_pair = pair<int, int>;

bool inSet(unordered_set<int> set, int v);
void getInput(int R, vector<vector<pair<int,int>>>& adj);
void fast_assign(vector<vector<pair<int, int>>> &adj, int D);
vector<int> ts(vector<vector<pair<int, int>>> &adj);
void print_time(const char *string, std::chrono::time_point<std::chrono::system_clock> point);

int main() {
    std::chrono::time_point<std::chrono::system_clock> start = high_resolution_clock::now();
    int T, D, R; // V < 250k, D-towns < 2000, E < 450k
    cin >> T >> D >> R;
    vector<vector<pair<int,int>>> adj(T+1); // src -> { {w1, dest1}, {w2, dest2} }
    getInput(R, adj);
    fast_assign(adj, D);
    print_time("fastassign ends at: ", start);
    ts(adj);
    print_time("ts ends at: ", start);


}

vector<int> ts(vector<vector<pair<int, int>>> &adj) {
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

            for (pair<int, int> nei : adj[curr]) {      // 2) projdem sousedy
                if (colors[nei.second] == GRAY) {   // 3) pokud je soused zabarveny, nasli jsme cyklus
//                    return {}; /// tohle mozna zmenit pokud delame Kosaraju
                } else if (colors[nei.second] == WHITE) { // 4) fresh souseda hodime na stack pro DFS
                    stack.push(nei.second);
                }
            }
            /// WARN: uzel jsme jeste nepopnuli!
        }
    }
    return sorted;
}


void fast_assign(vector<vector<pair<int, int>>> &adj, int D) {
    vector<int> districts(adj.size());
    vector<int> traversals(adj.size());
    unordered_set<int> seen;
    queue<int> q;
    for (int i = 1; i <= D; ++i) {
        traversals[i] = 0;
        districts[i] = i;
        q.push(i);
    }
    while (!q.empty()) {
        int curr = q.front(); q.pop();
        for(auto nei : adj[curr]) {
//            if(!inSet(seen, nei.second)) {
                if(districts[nei.second] == 0) {
                    traversals[nei.second] = traversals[curr] + 1;
                    districts[nei.second] = districts[curr];
//    setbuf(stdout, 0); cout << curr; cout << "    " << nei.second; cout << " " << traversals[nei.second];                    cout << "|" << districts[nei.second] <<"\n";
                    q.push(nei.second);
                    seen.insert(nei.second);
                }

//            }
        }
    }
}

bool inSet(unordered_set<int> set, int v) {
    if(set.find(v) != set.end()) return true;
    return false;
}

void print_time(const char *string, std::chrono::time_point<std::chrono::system_clock> point) {
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - point);
    setbuf(stdout, 0);
    cout << string;
    cout << duration.count()*0.000000001 << " s \n";
}

void getInput(int R, vector<vector<pair<int,int>>>& adj) {
    for (int i = 0; i < R; ++i) {
        int src, dest, weight;
        cin >> src >> dest >> weight;
        adj[src].push_back({weight, dest});
        adj[dest].push_back({weight, src});
    }
}
