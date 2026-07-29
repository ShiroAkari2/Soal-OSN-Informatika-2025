#include <bits/stdc++.h>
using namespace std;

const vector<string> grid = {
    "EEBE",
    "BBEB",
    "BBEB",
    "EEBE"
};

int main() {
    int Q = 3; cout << Q << endl; 

    vector<string> moves = {"R", "D", "R"};
    vector<string> history;

    string initial_type;
    cin >> initial_type;
    history.push_back(initial_type);

    for (int i = 0; i < Q; i++) {
        cout << moves[i] << endl;
        string res; cin >> res;
        history.push_back(res);
    }

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int cur_r = r;
            int cur_c = c;
            bool match = true;

            string type = (grid[cur_r][cur_c] == 'E') ? "ES" : "BATU";
            if (type != history[0]) continue;

            for (int k = 0; k < Q; k++) {
                if (moves[k] == "R") {
                    cur_c = (cur_c + 1) % 4;
                } else if (moves[k] == "L") {
                    cur_c = (cur_c + 3) % 4;
                } else if (moves[k] == "D") {
                    cur_r = (cur_r + 1) % 4;
                } else if (moves[k] == "U") {
                    cur_r = (cur_r + 3) % 4;
                }

                string move_type = (grid[cur_r][cur_c] == 'E') ? "ES" : "BATU";
                if (move_type != history[k + 1]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                cout << r + 1 << " " << c + 1 << endl;
                return 0;
            }
        }
    }
}
