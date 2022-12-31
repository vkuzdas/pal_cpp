//
// Created by kuzdavoj@fel.cvut.cz on 12/30/2022.
//

#include <iostream>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

typedef struct edge {
    int predIdx;
    int endIdx;
    int value;
    int edgeIdx;

    bool operator<(const edge &rhs) const {
        return value < rhs.value;
    }

    bool operator>(const edge &rhs) const {
        return value > rhs.value;
    }
} edge;



typedef struct vert {
    int parrent;
    int capacity;
    int val;
    int mark;
    vector<int> neib;
} UFvert;


int find(vector<vert> &UF, int vert) {
    if (UF[vert].parrent == vert) {
        return vert;
    } else {
        return UF[vert].parrent = find(UF, UF[vert].parrent);
    }

}


void unite(vector<vert> &UF, int &counter, int v1, int v2, int eVal, bool force) {
    // not same commponent
    int t1 = find(UF, v1);
    int t2 = find(UF, v2);
    if (t1 != t2) {
        if (t1 < t2) {
            UF[t2].parrent = t1;
            UF[t1].capacity += UF[t2].capacity;
            UF[t1].val = UF[t1].val + eVal + UF[t2].val;

        } else {
            UF[t1].parrent = t2;
            UF[t2].capacity += UF[t1].capacity;
            UF[t2].val = UF[t2].val + eVal + UF[t1].val;

        }
        --counter;
    }

    if (t1 == t2 && force) {
        UF[t1].val += eVal;

        --counter;
    }
}


void dfs(vector<vert> &UF, int v, int mark) {
    UF[v].mark = mark;
    for (auto e: UF[v].neib) {
        if (UF[e].mark == -1) dfs(UF, e, mark);
    }
}


int find_b(vector<vert> &UFB, int vert) {
    if (UFB[vert].parrent == vert) {
        return vert;
    } else {
        return UFB[vert].parrent = find_b(UFB, UFB[vert].parrent);
    }

}


bool unite_B(vector<vert> &UFB, int v1, int v2, int eVal, bool force) {
    // not same commponent
    int t1 = find_b(UFB, v1);
    int t2 = find_b(UFB, v2);
    if (t1 != t2) {
        if (t1 < t2) {
            UFB[t2].parrent = t1;
            UFB[t1].capacity += UFB[t2].capacity;
            UFB[t1].val = UFB[t1].val + eVal + UFB[t2].val;

        } else {
            UFB[t1].parrent = t2;
            UFB[t2].capacity += UFB[t1].capacity;
            UFB[t2].val = UFB[t2].val + eVal + UFB[t1].val;

        }
        return true;
    }
    return false;
}


std::priority_queue<edge, std::vector<edge>, std::greater<edge> > eCrossRiv0;
std::priority_queue<edge, std::vector<edge>, std::greater<edge> > eCrossRiv;
std::priority_queue<edge, std::vector<edge>, std::greater<edge> > eOther;

int counter;
int comp = 0;

int main() {
    int M, N, D, B;
    vector<UFvert> UF;
    vector<UFvert> UFB;

    cin >> M >> N >> D >> B;
    counter = N;
    UFvert t;
    t.neib = vector<int>();
    t.val = 0;
    t.mark = -1;
    t.capacity = 1;
    UF = vector<UFvert>(N, t);
    for (int i = 0; i < M; i++) {
        int V1, V2, C;
        cin >> V1 >> V2 >> C;

        edge tmp;
        tmp.edgeIdx = i;
        tmp.predIdx = V1;
        tmp.endIdx = V2;
        tmp.value = C;

        // init Union find struct
        UF[V1].parrent = V1;
        UF[V2].parrent = V2;

        // check if is the river edge
        if (V1 <= D && V2 > D || V1 > D && V2 <= D) {
            // add to river queue
            eCrossRiv0.push(tmp);

        } else {
            // add to other queue
            eOther.push(tmp);
            UF[V1].neib.push_back(V2);
            UF[V2].neib.push_back(V1);
        }
    }

    UFB = vector<UFvert>();
    // find Components
    for (int i = 0; i < N; i++) {
        if (UF[i].mark == -1) {

            dfs(UF, i, comp);
            UFvert ut;

            ut.parrent = comp;
            ut.capacity = 1;
            UFB.push_back(ut);
            comp += 1;

        }
    }


    while (eCrossRiv0.size() > 0) {
        edge tmp = eCrossRiv0.top();
        eCrossRiv0.pop();

        // if i can add it
        if (unite_B(UFB, UF[tmp.predIdx].mark, UF[tmp.endIdx].mark, tmp.value, 1)) {

            // if i can add it
            unite(UF, counter, tmp.predIdx, tmp.endIdx, tmp.value, 1);
        } else {
            eCrossRiv.push(tmp);
        }
    }

    // Kruscal's alg cross the river
    while (counter > N - B) {
        edge tmp = eCrossRiv.top();
        eCrossRiv.pop();

        // if i can add it
        unite(UF, counter, tmp.predIdx, tmp.endIdx, tmp.value, 1);
    }

    // Kruscal's alg to other
    while (UF[find(UF, 0)].capacity != N) {
        edge tmp = eOther.top();
        eOther.pop();

        // if i can add it
        unite(UF, counter, tmp.predIdx, tmp.endIdx, tmp.value, 0);
    }

    cout << UF[find(UF, 0)].val << endl;
}


