#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ll N;cin >> N;
    vector<ll> tinggi(N);
    
    for (int i = 0; i < N; i++) cin >> tinggi[i];
    for (int i = 1; i < N+1; i++) cout << tinggi[N-i] << "\n";
}
