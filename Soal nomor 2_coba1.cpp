#include <bits/stdc++.h>
using namespace std;

int main() {
    // Mempercepat I/O C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<long long> freq(100001, 0);
    for (int i = 0; i < N; i++) {
        int val;
        cin >> val;
        freq[val]++;
    }

    long long total_pasangan_malang = 0;

    // Untuk setiap angka kelipatan a (yaitu b = 2*a, 3*a, ...),
    // tambahkan (jumlah munculnya a) * (jumlah munculnya b)
    for (int a = 1; a <= 100000; a++) {
        if (freq[a] == 0) continue;

        for (int b = 2 * a; b <= 100000; b += a) {
            if (freq[b] > 0) {
                total_pasangan_malang += freq[a] * freq[b];
            }
        }
    }

    cout << total_pasangan_malang << "\n";

    return 0;
}
