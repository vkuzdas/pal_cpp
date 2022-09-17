//
// Created by kuzdavoj@fel.cvut.cz on 9/17/2022.
//
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <cassert>

using namespace std;

vector<int> iterative_DFS(vector<vector<int>> &adj);
stack<int> iterative_colored_topsort(vector<vector<int>> &adj, stack<int> &order);
vector<vector<int>> reverseEdges(vector<vector<int>> adj);
void kosar_SCC_DFS(vector<vector<int>> adj, vector<int> nodeSCCLeader, int curr);

int main() {
//    Vstup: Orientovaný graf G

//    1. Sestrojíme graf GT s obrácenými hranami.
//    2. Z   prázdný zásobník
//    3. Pro všechny vrcholy v nastavíme komp(v) nedefinováno.
//    4. Spouštíme DFS v GT opakovaně, než prozkoumáme všechny vrcholy. Kdykoliv
//    přitom opouštíme vrchol, vložíme ho do Z. Vrcholy v zásobníku jsou tedy setříděné podle out(v).
//    5. Postupně odebíráme vrcholy ze zásobníku Z a pro každý vrchol v:
//    6. Pokud komp(v) = nedefinováno:
//    7. Spustíme DFS(v) v G, vstupujeme pouze do vrcholů
//        s nedefinovanou hodnotou komp(...) a tuto hodnotu přepisujeme na v.
    vector<vector<int>> adjOrig{{6}, {4}, {8}, {0}, {7}, {2}, {3}, {1}, {5}};
    vector<vector<int>> adjRev = reverseEdges(adjOrig);
    vector<vector<int>> expected{{3}, {7}, {5}, {6}, {1}, {8}, {0}, {4}, {2}};
    assert(adjRev == expected);

    stack<int> stack;
    vector<int> sccOf(adjOrig.size(), INT16_MIN);
    iterative_colored_topsort(adjRev, stack);

    while(!stack.empty()) {
        int curr = stack.top();
        if (sccOf[curr] == INT16_MIN) continue;
        kosar_SCC_DFS(adjOrig, sccOf, curr);
    }

    return 0;
}

void kosar_SCC_DFS(vector<vector<int>> adj, vector<int> nodeSCCLeader, int startVertex) {
//    6. Pokud komp(v) = nedefinováno:
//    7. Spustíme DFS(v) v G, vstupujeme pouze do vrcholů
//        s nedefinovanou hodnotou komp(...) a tuto hodnotu přepisujeme na v.
    unordered_set<int> seen;

    for (int i = 0; i < adj.size(); ++i) {
        if (seen.find(i) == seen.end()) {
            stack<int> stack({startVertex}); // na stacku startVertex

            while (!stack.empty()) {
                int curr = stack.top();
                stack.pop();
                if(seen.find(curr) == seen.end()) { // curr not seen yet (not found in seen)
                    seen.insert(curr);
                    for (int neighbor : adj[curr]) {
                        if(seen.find(neighbor) == seen.end()) { // neighbor not seen yet (not found in seen)
                            stack.push(neighbor);
                        }
                    }
                }
            }
        }
    }
}

/// Note: Backwards dfs?
//      see:  https://cs.stackexchange.com/questions/100896/how-to-traverse-a-graph-in-reverse-with-dfs
//      Just take the code for DFS and,
//      every time it says "If there is an edge from x to y",
//      replace that with "If there is an edge from y to x."
vector<vector<int>> reverseEdges(vector<vector<int>> adj) {
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
 *
 * @param adj - graf jako adjList
 * @return topologicke ocislovani (poradi odzadu; tzn sink<-begin)
 */
stack<int> iterative_colored_topsort(vector<vector<int>> &adj, stack<int> &order) {
    int WHITE = 1; // fresh
    int GRAY = 2;  // current
    int BLACK = 3; // closed

    vector<int> colors(adj.size(), WHITE);
    stack<int> stack;

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
                order.push(curr);
            }

            for (int neighbor : adj[curr]) {      // 2) projdem sousedy
                if (colors[neighbor] == GRAY) {   // 3) pokud je soused zabarveny, nasli jsme cyklus
                    return {}; /// tohle mozna zmenit pokud delame Kosaraju
                } else if (colors[neighbor] == WHITE) { // 4) fresh souseda hodime na stack pro DFS
                    stack.push(neighbor);
                }
            }
            /// WARN: uzel jsme jeste nepopnuli!
        }
    }
    return order;
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
vector<int> iterative_DFS(vector<vector<int>> &adj) {
    vector<int> path;
    unordered_set<int> seen;

    for (int i = 0; i < adj.size(); ++i) {
        if (seen.find(i) == seen.end()) { // TODO: can I get to constant check?
            stack<int> stack({i}); // na stacku startVertex

            while (!stack.empty()) {
                int curr = stack.top();
                stack.pop();
                if(seen.find(curr) == seen.end()) { // curr not seen yet (not found in seen)
                    seen.insert(curr);
                    path.push_back(curr);
                    for (int neighbor : adj[curr]) {
                        if(seen.find(neighbor) == seen.end()) { // neighbor not seen yet (not found in seen)
                            stack.push(neighbor);
                        }
                    }
                }
            }
        }
    }

    return path;
}
