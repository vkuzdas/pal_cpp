//
// Created by kuzdavoj@fel.cvut.cz on 12/17/2022.
//

#include <vector>
#include <iostream>
#include <string>
#include <stack>
#include <set>
#include <map>


using namespace std;


int NA = -1;
int REPLACE = 0;







void ld_dbg(string pattern, string text, vector<vector<int>> dp) {
    // print dlouheho stringu nahore
    cout << "    ";
    for(char c : text) cout << c << " ";
    cout << endl;

    for (int i = 0; i < dp.size(); ++i) {
        if(i != 0) cout << pattern[i - 1] << " ";
        if(i == 0) cout << "  ";
        for (int j = 0; j < dp[0].size(); ++j) {
            if(dp[i][j] == -1) {
                cout << "." << " ";
            } else {
                cout << dp[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void op_dbg(string pattern, string text, vector<vector<int>> op) {
    // print dlouheho stringu nahore
    cout << "    ";
    for(char c : text) cout << c << " ";
    cout << endl;

    for (int i = 0; i < op.size(); ++i) {
        if(i != 0) cout << pattern[i - 1] << " ";
        if(i == 0) cout << "  ";
        for (int j = 0; j < op[0].size(); ++j) {
            if(op[i][j] == -1) {
                cout << "-1" << " ";
            } else if(op[i][j] == REPLACE) {
                cout << "-1" << " ";
            } else {
                cout << op[i][j] << " ";
            }
        }
        cout << endl;
    }
}



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
            positions.push_back(shift+pattern.length()-1);
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


// pozice stringu -> indexy clip schemes
vector<vector<int>> clips_in_chain(const string& chain, vector<string> clip_schemes) {
    vector<int> e;
    vector<vector<int>> clips_on_pos(chain.length(), e);

    for (int sch_idx = 0; sch_idx < clip_schemes.size(); ++sch_idx) {

        /// pro kazde schema zjistime jeho pozice
        string scheme = clip_schemes[sch_idx];
        vector<int> positions_in_chain = boyerMoore(scheme, chain);

        /// kazdou pozici zaneseme do mapy v paru {pozice_v_chainu, pozice_schemu_ve_vectoru}
        for (int pos : positions_in_chain) {
            auto other_sch_positions = clips_on_pos[pos];
            if (other_sch_positions.empty()) {
                clips_on_pos[pos].push_back(sch_idx);
            }
            else {
                other_sch_positions.push_back(sch_idx);
                clips_on_pos[pos] = other_sch_positions;
            }
        }
    }
    return clips_on_pos;
}

int calc_repl_cost(vector<vector<int>> &dp, const string &pattern, const string &text, int r, int c, int RF) {
    char pattern_char = pattern[r-1];
    char text_char = text[c-1];
    int diff = abs(pattern_char - text_char);

    int prev_repl_cost = dp[r-1][c-1];
    int total_repl_cost = prev_repl_cost + (diff * RF);

    return total_repl_cost;
}


// vraci cenu nejnizsiho clip schemu na danem miste textu
pair<int, int> calc_min_clip_len(
        vector<vector<int>> &dp, const string &pattern, const string &text,
        int r, int c, int CF,
        vector<vector<int>> &clips_on_pos, const vector<string> &clip_schemes) {


    int current_text_pos = c-1;
    vector<int> clips_on_current = clips_on_pos[current_text_pos];

    if(clips_on_current.empty()) {
        return {INT32_MAX, 0};
    }
    else {
        int best_cost = INT32_MAX;
        int best_len = INT32_MAX;
        for (int sch_idx : clips_on_current) {
            string scheme = clip_schemes[sch_idx];
            if(dp[r][c - scheme.length()] == NA) {
                // clip zasahuje do trojuhelniku
                continue;
            }
            else {
                /// ke cost se musi pricist cena co je o delku clipu dozadu
                char first = scheme[0] - 'a' + 1;
                char last = scheme.back() - 'a' + 1;
                int cost = (first + last) * CF;
                int cost_of_scheme_len_back = dp[r][c-scheme.length()];
                cost += cost_of_scheme_len_back;
                if(scheme.length() == 1) cost = (first * CF) + cost_of_scheme_len_back;
                if(cost < best_cost) {
                    best_cost = cost;
                    best_len = scheme.length();
                } else {
                    if (cost == best_cost) {
                        if(scheme.length() < best_len) {
                            best_cost = cost;
                            best_len = scheme.length();
                        }
                        // kdyz jsou stejne dlouhe i stejne drahe, je to jedno
                    }
                    // kdyz je best cost mensi, nic neaktualizuju
                }
            }
        }
        return {best_cost, best_len};
    }
}

pair<vector<vector<int>>, vector<vector<int>>> leven_clip_replace(
        const string& pattern, const string& text,
        int CF, int RF,
        vector<vector<int>>& clips_on_pos, const vector<string>& clip_schemes, bool print_tables) {

    int p_len = (int) pattern.length();
    int t_len = (int) text.length();

    vector<vector<int>> dp(p_len + 1, vector<int>(t_len + 1));
    vector<vector<int>> op(p_len + 1, vector<int>(t_len + 1));

    /// init leveho trojuhelniku
    for (int r = 1; r <= p_len; r++) {
        for (int c = 0; c < r; ++c) {
            dp[r][c] = NA;
            op[r][c] = NA;
        }
    }
//    ld_dbg(pattern, text, dp);


    for (int r = 1; r <= p_len; r++) {
        for (int c = r; c <= t_len; c++) {
            // zjisti replace
            // zjisti min(clip) a jeho delku
            // na pole zapis minimum obou, do OP zda jsi delal replace nebo clip
            int replace = calc_repl_cost(dp, pattern, text, r, c, RF);

            pair<int, int> clip_len = calc_min_clip_len(dp, pattern, text, r, c, CF, clips_on_pos, clip_schemes);

            int min_clip = clip_len.first;
            int len = clip_len.second;
            int m = min(replace, min_clip);
            dp[r][c] = m;

            /// bud jsme udelali replace a nebo clip
            if(m == replace) {
                op[r][c] = REPLACE;
            }
            else if(m == min_clip) {
                op[r][c] = len;
            }

        }
    }

    if(print_tables) {
        ld_dbg(pattern, text, dp);
        op_dbg(pattern, text, op);
    }
//    cout << dp[p_len][t_len]
    return {dp, op};
}

pair<int, int> find_min(vector<vector<int>> dp, string demand) {
    vector<int> last_row = dp[demand.length()];
    int min = INT32_MAX;
    int leftest = last_row.size();
    // do velikost demand.length() je v dp trojuhelnik
    for (int i = demand.length(); i < last_row.size(); ++i) {
        int candidate = last_row[i];
        if(candidate < min) {
            min = candidate;
            leftest = i;
        } else if (candidate == min) {
            if (i < leftest) {
                min = candidate; // asi neni treba
                leftest = i;
            }
        }
    }

    return {min, leftest};
}

int main() {
    /// Read input
    int R,C,LD,CS,CF,RF;
    cin >> R >> C >> LD >> CS >> CF >> RF;
    vector<vector<char>> mtx = read_mtx(R, C);
    string demand;
    cin >> demand;
    vector<string> clip_schemes = read_clip_schemes(CS);

    /// unwind original chain into single long string
    string chain = unwind_mtx(mtx);

    /// where are clips in the string
    vector<vector<int>> clips_on_pos = clips_in_chain(chain, clip_schemes);

    pair<vector<vector<int>>, vector<vector<int>>> dp_op =
            leven_clip_replace(demand, chain, CF, RF, clips_on_pos, clip_schemes, false);

    vector<vector<int>> dp = dp_op.first;
    vector<vector<int>> op = dp_op.second;

    // najdeme ve spodnim radku minimum nejvice vlevo
    pair<int, int> min_pos = find_min(dp, demand);
    int cost = min_pos.first;

    // od minima jdeme doleva nahoru "traceback" abychom zjistili kde zacal cut
    int c = min_pos.second;
    int r = demand.length();




    for(;;) {
        if(c == 0 || r == 0) {
            break;
        }
        if(op[r][c] == REPLACE) {
            c--;
            r--;
        }
        if(op[r][c] != REPLACE) {
            c = c - op[r][c];
        }
    }

    int len = min_pos.second - c;

    cout << c+1 << " " << len << " " << cost << endl;


    return 0;
}

