#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N; if (!(cin >> N)) return 0;
    vector<ll> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    for (int i = N - 1; i >= 0; i--) {
        cout << A[i] << "\n";
    }
}
