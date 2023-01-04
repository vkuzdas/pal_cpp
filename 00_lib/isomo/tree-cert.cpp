#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

struct Node {
    int id;
    int parent;
    vector<int> children;
    string label;

    Node(int id, int parent, const vector<int> &children, const string &label) : id(id), parent(parent),
                                                                                 children(children), label(label) {}
};




struct G{
    vector<Node> nodes;
    vector<pair<int,int>> edges;

    G(const vector<Node> &nodes, const vector<pair<int, int>> &edges) : nodes(nodes), edges(edges) {}
};


int parent_from_edges(int p, vector<pair<int, int>> edges) {
    for(pair<int, int> edge : edges) {
        if(edge.second == p) return edge.first;
    }
    return 0;
}
int parent_from_nodes(int p, vector<Node> nodes) {
    return nodes[p].parent;
}

G tree_from_cert(string cert) {
    vector<Node> nodes;
    vector<pair<int,int>> edges;
    for (int i = 0; i < cert.size()/2; ++i)
        nodes.push_back({i, 0, {}, "01"});

    int dest = 0;
    int src = dest;

    // iterujeme pouze uvnitr certu
    for (int x = 1; x < cert.size() - 1; x++) {
        if (cert[x] == '0') {
            dest++;
            edges.emplace_back(src, dest);
            nodes[src].children.push_back(dest);
            nodes[dest].parent = src;
            src = dest;
        } else {
//            src = parent_from_edges(src, edges);
            /// much faster from nodes
            src = parent_from_nodes(src, nodes);
        }
    }

    return {nodes, edges};
}






// Function for generating the certificate for a given graph
string cert_from_tree(vector<Node> nodes) {

    // pozicni helper kvuli mazani potomku
    vector<int> pos;
    for(const Node& n : nodes)
        pos.push_back(n.id);

    //Dokud ma strom vic nez dva uzly
    while (nodes.size() > 2) {

        vector<Node> new_tree;
        for (Node x : nodes) {

            // Pro kazde uzel co neni listem
            if (!x.children.empty()) {

                // odstranit krajni 0 a 1, vnitrek labelu bude soucast noveho labelu
                vector<string> ch_lbls{x.label.substr(1, x.label.size()-2)};
                vector<int> new_ch;

                // label potomku lexikograficky seradit
                for (int child : x.children) {
                    int ch_pos = pos[child];
                    Node ch_node = nodes[ch_pos];
                    if (ch_node.children.empty())
                        ch_lbls.push_back(ch_node.label);
                    else
                        new_ch.push_back(child);
                }
                x.children = new_ch;
                sort(ch_lbls.begin(), ch_lbls.end());

                // potomky obalime 0 a 1
                x.label = "0";
                for(const string& label : ch_lbls)
                    x.label += label;
                x.label += "1";

                pos[x.id] = new_tree.size();
                new_tree.push_back(x);
            }
        }
        nodes = new_tree;
    }


    // Pokud zbyde jen jeden uzel, certifikat stromu je label tohoto uzlu
    if (nodes.size() == 1) {
        return nodes[0].label;
    }


    // Jinak musi zbyt dva uzly, lexikograficky serazene labely techto je certifikat
    string cert = min(nodes[0].label, nodes[1].label) + max(nodes[0].label, nodes[1].label);
    return cert;
}



int main() {
    // 1) Vsechny uzly initujeme na default label "01"
    vector<Node> nodes;
    nodes.push_back(Node(0, 0, {1,2,5,8}, "01"));
    nodes.push_back(Node(1, 0, {3}, "01"));
    nodes.push_back(Node(2, 0, {11,10}, "01"));
    nodes.push_back(Node(3, 1, {}, "01"));
    nodes.push_back(Node(4, 5, {}, "01"));
    nodes.push_back(Node(5, 0, {4,7}, "01"));
    nodes.push_back(Node(6, 7, {}, "01"));
    nodes.push_back(Node(7, 5, {6}, "01"));
    nodes.push_back(Node(8, 0, {9}, "01"));
    nodes.push_back(Node(9, 8, {}, "01"));
    nodes.push_back(Node(10, 2, {}, "01"));
    nodes.push_back(Node(11, 2, {}, "01"));

    string cert = cert_from_tree(nodes);
    cout << cert;


    cout << endl;
    G g = tree_from_cert(cert);
    string cert2 = cert_from_tree(g.nodes);
    cout << cert2;

    G g2 = tree_from_cert("0011");
    cout << "done";
}