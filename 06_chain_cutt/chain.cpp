//
// Created by kuzdavoj@fel.cvut.cz on 12/17/2022.
//

#include <vector>
#include <iostream>
#include <string>
#include <stack>
#include <set>


using namespace std;


struct C_Scheme {
    vector<int> pos;
    int clip_cost;
    int len;
    bool dont_clip;
    string value;

    C_Scheme() {}

};

struct Result {
    int pos;
    int len;
    int cost;

    Result() {};

    Result(int pos, int len, int cost) : pos(pos), len(len), cost(cost) {}

    // pokud je cost1 = cost2, vybereme ten kratsi
    // pokud jsou stejne dlouhe, vybereme ten blizsi
    bool operator < (const Result& res) const { // tento res je mensi
        if (cost < res.cost)
            return true;
        if (cost > res.cost)
            return false;

        /// cost se rovnaji
        if (len < res.len)
            return true;
        if (len > res.len)
            return false;

        /// len se rovnaji
        if (pos < res.pos)
            return true;

        return false;
    }
};



vector<vector<char>> read_mtx(int R, int C) {
    vector<vector<char>> mtx;
    for (int i = 0; i < R; ++i) {
        vector<char> row;
        for (int j = 0; j < C; ++j) {
            char c;
            cin >> c;
            row.push_back(c);
        }
        mtx.push_back(row);
    }
    return mtx;
}

int minimum(int a, int b, int c) {
    return min(min(a, b), c);
}

int get_replace_cost(string word1, string word2, int RF) {
    int total = 0;
    for (int i = 0; i < word1.length(); ++i) {
        int diff = word1[i] - word2[i];
        if(diff < 0)
            diff = diff * -1;
        total += (diff * RF);
    }
    return total;
}

vector<int> boyerMoore(string pattern, string text) {
    vector<int> positions;
    int n = (int) text.length();
    int m = (int) pattern.length();
    int badchar[256];
    for (int & el : badchar)
        el = -1;
    for (int i = 0; i < m; i++)
        badchar[(int) pattern[i]] = i;

    int shift = 0; // is shift of the pattern with respect to text
    while (shift <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;
        if (j < 0) {
            positions.push_back(shift);
//            std::cout << "pattern occurs at shift = " << shift << std::endl;
            shift += (shift + m < n) ? m - badchar[text[shift + m]] : 1;
        } else {
            // Shift the pattern so that the bad character in text aligns with the last occurrence of it in pattern
            shift += std::max(1, j - badchar[text[shift + j]]);
        }
    }
    return positions;
}


vector<string> read_clip_schemes(int CS) {
    vector<string> clip_schemes;
    for (int i = 0; i < CS; ++i) {
        string scheme;
        cin >> scheme;
        clip_schemes.push_back(scheme);
    }
    return clip_schemes;
}

pair<int, int> left_neigh(pair<int, int> src) {
    return {src.first+1, src.second-1};
}
pair<int, int> right_neigh(pair<int, int> src) {
    return {src.first+1, src.second+1};
}
pair<int, int> lower_neigh(pair<int, int> src) {
    return {src.first+1, src.second};
}

vector<pair<int,int>> neighbors_of(pair<int, int> src, vector<vector<char>>& mtx) {
    /// pair<row, col>
    // 1, 2, 3, 0 sousedy
    int last_col = (int) mtx[0].size()-1;
    int first_col = 0;
    int last_row = (int) mtx.size()-1;
    int src_row = src.first;
    int src_col = src.second;

    if (src_row == last_row) {
        // 0 sousedu
        return {};
    }

    /// PRI RETURN JE NUTNE ABY SE PRVNI VRACEL UZEL NA PRAVO! Takove maji ve stacku prednost
    if (src_col == first_col) {
        int cols_to_the_right = last_col;
        if (cols_to_the_right == 0)
            // vracim spodniho souseda
            return { lower_neigh(src) };
        else
            /* vracim spodniho a souseda napravo */ /// NAPRAVO PRVNI!
            return {right_neigh(src), lower_neigh(src)};
    }
    else if (src_col == last_col) {
        int cols_to_the_left = last_col;
        if (cols_to_the_left == 0)
            // vracim spodniho
            return { lower_neigh(src) };
        else
            /* vracim spodniho a souseda nalevo */ /// SPODNI PRVNI! (je napravo)
            return {lower_neigh(src), left_neigh(src)};
    }
    else {
        return {right_neigh(src), lower_neigh(src), left_neigh(src)};
    }
}

string unwind_mtx(vector<vector<char>>& mtx) {
    string res;
    int col_num = (int) mtx[0].size();

    vector<string> m(col_num, "");
    vector<vector<string>> parent_mtx(mtx.size(),m);

    for (int i = 0; i < col_num; ++i) {
        /// pair<row, col>
        pair<int,int> src = {0,i}; // prvni radek, i-ty sloupec
        stack<pair<int,int>> S;
        S.push(src);
        while(!S.empty()) {
            pair<int,int> par = S.top(); S.pop();
            char par_char = mtx[par.first][par.second];

            auto nbrs = neighbors_of(par, mtx);
            if(nbrs.empty()) {
//                cout << " " << parent_mtx[par.first][par.second] + par_char; /// print sam sebe
                res += parent_mtx[par.first][par.second] + par_char;
            }
            for(auto nei_coor : nbrs) {
                // initovat sousedy
                int r = nei_coor.first;
                int c = nei_coor.second;
                string par_str = parent_mtx[par.first][par.second];
                parent_mtx[r][c] = par_str + par_char;

                // pushnout (precedent praveho je uz vyresen)
                S.push(nei_coor);
            }
        }
    }
    return res;
}

int get_clip_cost(const string &scheme, int CF) {
    char base = 'a' - 1;
    if (scheme.length() == 1)
        return (scheme[0] - base) * CF;

    int size_of_first = scheme[0] - base;
    int size_of_last = scheme[scheme.length()-1] - base;

    return (size_of_first + size_of_last) * CF;
}

vector<C_Scheme> init_C_Schemes(const string& demand, const string& chain, const vector<string>& schemes, int CF) {
    vector<C_Scheme> res;
    for (const string& sch : schemes) {
        C_Scheme curr;
        curr.pos = boyerMoore(sch, chain);
        curr.value = sch;
        curr.len = sch.length();
        curr.clip_cost = get_clip_cost(sch,CF);
        curr.dont_clip = (!boyerMoore(sch, demand).empty());
    }
    return res;
}

int main() {

    /// Read input
    int R,C,LD,CS,CF,RF;
    cin >> R >> C >> LD >> CS >> CF >> RF;
    vector<vector<char>> mtx = read_mtx(R, C);
    string demand;
    cin >> demand;
    vector<string> clip_schemes_plain = read_clip_schemes(CS);

    string chain = unwind_mtx(mtx);
    vector<C_Scheme> C_Schemes = init_C_Schemes(demand, chain, clip_schemes_plain, CF);
//    cout << chain;


    Result best = {INT32_MAX, INT32_MAX, INT32_MAX};
    vector<Result> r;
    for(const C_Scheme& cs : C_Schemes) {
        for (auto pos : cs.pos) {

            // 1) CUT-OUT
            string trimmed = chain.substr(0,pos) + chain.substr(pos+cs.len, chain.length());

            int start, end;
            if(demand.length() == pos || demand.length() < pos) // demand přečnívá na začátku (nebo je akorát)
                start = 0;
            else if (demand.length() > pos) // demand nepřečnívá
                start = (int)demand.length() - pos;

            end = start + (int)demand.length();

            /// dokud jsme nedojeli na konec textu nebo jsme neprohledali cele okoli
            while (end <= trimmed.length() && start <= pos) {

                string clip = trimmed.substr(start, demand.length());

                int replace_cost = get_replace_cost(clip, demand, RF);
                int total_cost = cs.clip_cost + replace_cost;
//                cout << clip << "  <-" << demand << endl;
                if(total_cost == 32) {
                    cout << "bug";
                }
                Result best_candidate = Result(start+1, demand.length()+cs.len, total_cost);
                r.push_back(best_candidate);
                best = min(best, best_candidate);

                start++;  end++;
            }
        }
    }

    cout << best.pos << " " << best.len << " " <<  best.cost << endl;
    return 0;
}

