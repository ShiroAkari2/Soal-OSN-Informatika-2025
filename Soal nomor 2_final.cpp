
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    int N; if (!(cin >> N)) return 0;

    const int MAXA = 100000;
    vector<ll> freq(MAXA + 1, 0);

    for (int i = 0; i < N; i++) {
        int val; cin >> val;
        freq[val]++;
    }
    ll r = 0;

    for (int x = 1; x <= MAXA; x++) {
        if (freq[x] == 0) continue;
        for (int y = 2 * x; y <= MAXA; y += x) {
            if (freq[y] > 0) {
                if ((x ^ y) > y) r += freq[x] * freq[y];
            }
        }
    }
    cout << r << "\n";
}
