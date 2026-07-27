#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Optimasi agar proses input/output berjalan cepat
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    // Vector menggunakan long long karena nilai A_i bisa mencapai 10^18
    vector<long long> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    // Cetak secara terbalik dari indeks N-1 ke 0 (kanan ke kiri)
    for (int i = N - 1; i >= 0; i--) {
        cout << A[i] << "\n";
    }

    return 0;
}
