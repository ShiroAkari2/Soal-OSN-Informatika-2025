#include <bits/stdc++.h>
using namespace std;

int main() {
    // Mempercepat I/O C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    const int MAXA = 100000;
    vector<long long> freq(MAXA + 1, 0);

    for (int i = 0; i < N; i++) {
        int val;
        cin >> val;
        freq[val]++;
    }

    long long total_pasangan_malang = 0;

    // Untuk setiap x dari 1..MAXA
    for (int x = 1; x <= MAXA; x++) {
        if (freq[x] == 0) continue;

        // Iterasi semua kelipatan y = 2*x, 3*x, dst.
        for (int y = 2 * x; y <= MAXA; y += x) {
            if (freq[y] > 0) {
                // Syarat: KPK(x, y) < XOR(x, y)
                // Karena x membagi y, KPK(x, y) = y.
                // Jadi cukup cek apakah (x ^ y) > y.
                if ((x ^ y) > y) {
                    total_pasangan_malang += freq[x] * freq[y];
                }
            }
        }
    }

    cout << total_pasangan_malang << "\n";

    return 0;
}
