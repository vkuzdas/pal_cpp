#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <list>

using namespace std;

struct Edge {
    int from;
    int to;
    char letter;

    Edge(int f, int t, char l) : from(f), to(t), letter(l) {};

};

struct Node {
    int cost_from_start = -1;
    string sequence_from_start;
    int cost_to_end = -1;
    string sequence_to_end;
};

vector<Node> nodes_info;
string result_string;
int result_cost = INT32_MAX;

void BFSFromStart(vector<vector<Edge>> adj, int n, vector<string> word) {

    list<int> queue;
    bool visited[n];

    for (int i = 0; i < n; i++) {
        visited[i] = false;
        word[i] = "";
    }

    visited[0] = true;
    queue.push_back(0);
    nodes_info[0].cost_from_start = 0;

    while (!queue.empty()) {
        int current_node_id = queue.front();
        queue.pop_front();
        for (int i = 0; i < (int)adj[current_node_id].size(); i++) {
            if (!visited[adj[current_node_id][i].to] || (word[adj[current_node_id][i].to].size() > word[current_node_id].size() + 1 )) {
                visited[adj[current_node_id][i].to] = true;
                word[adj[current_node_id][i].to] = word[current_node_id] + adj[current_node_id][i].letter;
                nodes_info[adj[current_node_id][i].to].sequence_from_start = word[adj[current_node_id][i].to];
                nodes_info[adj[current_node_id][i].to].cost_from_start = (int)word[adj[current_node_id][i].to].size();
                queue.push_back(adj[current_node_id][i].to);
            } else if (word[adj[current_node_id][i].to].size() == word[current_node_id].size() + 1) {
                string tmp = word[current_node_id] + adj[current_node_id][i].letter;
                string tmp2 = word[adj[current_node_id][i].to];

                if (lexicographical_compare(tmp.begin(), tmp.end(), tmp2.begin(), tmp2.end())) {
                    word[adj[current_node_id][i].to] = tmp;
                    nodes_info[adj[current_node_id][i].to].sequence_from_start = tmp;
                    nodes_info[adj[current_node_id][i].to].cost_from_start = (int)tmp.size();
                    queue.push_back(adj[current_node_id][i].to);
                }
            }
        }
    }
}

void BFSFromEnd(vector<vector<Edge>> adj, const vector<int>& finalStates, int n, vector<string> word) {

    list<int> queue;
    bool visited[n];

    for (int i = 0; i < n; i++) {
        word[i] = "";
        visited[i] = false;
    }


    for (int finalState : finalStates) {
        visited[finalState] = true;
        queue.push_back(finalState);
        nodes_info[finalState].cost_to_end = 0;
    }

    while (!queue.empty()) {
        int current_node_id = queue.front();
        queue.pop_front();
        for (int i = 0; i < (int)adj[current_node_id].size(); i++) {

            if (nodes_info[adj[current_node_id][i].to].cost_from_start == -1) {
                /// pokud node nema cenu ze startu, neresim ani pro backBFS
                continue;
            }

            if ((word[adj[current_node_id][i].to].size() > word[current_node_id].size()+1 || !visited[adj[current_node_id][i].to])) {
                /// pokud path sousedu je vetsi nez delka curr+1 -> update
                /// tohle staci jen !visited

                if (visited[adj[current_node_id][i].to]) {
                    cout << "HELLO!!!!" << endl;
                }

                visited[adj[current_node_id][i].to] = true;
                string tmp = adj[current_node_id][i].letter + word[current_node_id];
                word[adj[current_node_id][i].to] = tmp;

                nodes_info[adj[current_node_id][i].to].sequence_to_end = tmp;
                nodes_info[adj[current_node_id][i].to].cost_to_end = (int)tmp.size();
                queue.push_back(adj[current_node_id][i].to);

            } else if (word[adj[current_node_id][i].to].size() == word[current_node_id].size() + 1) {
                /// vlevo delka substringu ulozena v childu
                /// vpravo delka currentu
                // pokud soused se rovna currentu+1
                // tak je porovnam

                string tmp = adj[current_node_id][i].letter + word[current_node_id];
                string tmp2 = word[adj[current_node_id][i].to];


                if (lexicographical_compare(tmp.begin(), tmp.end(), tmp2.begin(), tmp2.end())) {
                    /// pokud curr+edge je lex lepsi, udelej bfs
                    word[adj[current_node_id][i].to] = tmp;
                    nodes_info[adj[current_node_id][i].to].sequence_to_end = tmp;
                    nodes_info[adj[current_node_id][i].to].cost_to_end = (int)tmp.size();
                    queue.push_back(adj[current_node_id][i].to);
                }

            }
        }
    }
}



void DFS(vector<vector<Edge>> graph, string word, int start, int pos, Edge edge) {

    if (pos == (int)word.size() - 1) {

        string current_result;
        current_result.append(nodes_info[start].sequence_from_start);
        current_result.append(word);
        current_result.append(nodes_info[edge.to].sequence_to_end);

        if (result_string.empty() || (int)current_result.size() < result_cost) {
            result_string = current_result;
            result_cost = (int) current_result.size();
        } else if (current_result.size() == result_string.size()) {

            if (lexicographical_compare(current_result.begin(), current_result.end(), result_string.begin(),
                                        result_string.end())) {
                result_string = current_result;
                result_cost = (int) current_result.size();
            }
        }
        return;
    }

    for (int i = 0; i < (int)graph[edge.to].size(); i++) {
        Edge e = graph[edge.to][i];
        Node n = nodes_info[e.to]; // soused

        if (e.letter == word[pos + 1]) {
            if ((nodes_info[start].cost_from_start + pos+1 + max(n.cost_to_end, (int) word.size() - (pos+2))) <=
                result_cost) {
                DFS(graph, word, start, pos + 1, graph[edge.to][i]);
            }
        }
    }
}

int main() {

    int number_of_nodes, number_of_chars;
    cin >> number_of_nodes >> number_of_chars >> ws;
    vector<vector<Edge>> graph(number_of_nodes);
    vector<vector<Edge>> reverse_graph(number_of_nodes);
    nodes_info = vector<Node>(number_of_nodes);
    vector<int> delimiters = vector<int>();
    map<int, bool> isFinalMap = map<int, bool>();
    vector<int> finalStates = vector<int>();

    for (int i = 0; i < number_of_nodes; i++) {
        graph[i] = vector<Edge>();

        string input;
        int id;
        char isFinal;

        getline(cin, input);

        char letter = 'a';
        if (delimiters.empty()) {
            for (int j = 0; j < number_of_chars; j++) {
                delimiters.push_back((int)input.find(letter));
                letter++;
            }
            delimiters.push_back((int)input.size());
        } else {
            for (int j = 0; j < number_of_chars; j++) {
                delimiters[j] = (int)input.find(letter);
                letter++;
            }
        }

        delimiters[number_of_chars] = (int)input.size();

        string tmp;
        stringstream ss(input.substr(0, delimiters[0]));
        ss >> id;
        ss >> isFinal;

        ss.clear();

        if (isFinal == 'F') {
            isFinalMap[i] = true;
            finalStates.push_back(i);
        } else {
            isFinalMap[i] = false;
        }

        letter = 'a';
        for (int j = 0; j < number_of_chars; j++) {

            string sub_string = input.substr(delimiters[j]+1, delimiters[j+1]-delimiters[j]-1);
            stringstream ss_tmp (sub_string);

            int a;
            string temp;
            while(!ss_tmp.eof()) {
                ss_tmp >> temp;
                if (stringstream(temp) >> a) {
                    Edge e = Edge(i, a, letter);
                    Edge e2 = Edge(a, i, letter);
                    graph[i].push_back(e);
                    reverse_graph[a].push_back(e2);
                }
                temp = "";
            }
            letter++;
        }
    }

    string word;
    cin >> word;

    BFSFromStart(graph, number_of_nodes, vector<string>(number_of_nodes));
    BFSFromEnd(reverse_graph, finalStates, number_of_nodes, vector<string>(number_of_nodes));

    char firstLetter = word[0];

    for(int i = 0; i < number_of_nodes; i++) {
        for(int j = 0; j < (int)graph[i].size(); j++) {
            if (graph[i][j].letter == firstLetter) {
                Node n = nodes_info[i];
                if (((n.cost_from_start + max(n.cost_to_end, (int)word.size())) <= result_cost)) {
                    DFS(graph, word, graph[i][j].from, 0, graph[i][j]);
                }
            }
        }
    }
    cout << result_string << endl;

    return 0;
}