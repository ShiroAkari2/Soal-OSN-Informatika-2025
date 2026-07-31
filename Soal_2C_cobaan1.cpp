#include <bits/stdc++.h>
using namespace std;

// Struktur Node Segment Tree
struct Node {
    long long sum;
    long long weighted_sum; // Sum(A[i] * (i - offset + 1))
};

int N, Q, K;
vector<long long> A;
vector<Node> tree;

// Membangun Segment Tree secara bottom-up / recursive
void build(int node, int l, int r, int depth) {
    if (l == r) {
        tree[node].sum = A[l];
        tree[node].weighted_sum = A[l]; // (A[l] * 1)
        return;
    }
    int mid = (l + r) / 2;
    int left_child = 2 * node;
    int right_child = 2 * node + 1;

    build(left_child, l, mid, depth - 1);
    build(right_child, mid + 1, r, depth - 1);

    long long len_left = mid - l + 1;
    tree[node].sum = tree[left_child].sum + tree[right_child].sum;
    
    // weighted_sum node gabungan = weighted_sum kiri + weighted_sum kanan + (sum kanan * len_left)
    tree[node].weighted_sum = tree[left_child].weighted_sum + 
                              tree[right_child].weighted_sum + 
                              tree[right_child].sum * len_left;
}

// Rekursi untuk menjawab query Tipe 2 & Tipe 3
// Meneruskan 'accumulated_count' untuk menghitung indeks relatif bagi Tipe 3
struct QueryResult {
    long long sum;
    long long weighted_sum;
};

QueryResult query(int node, int l, int r, int ql, int qr, int mask, int depth) {
    // Jika interval node sepenuhnya berada di luar [ql, qr]
    if (r < ql || l > qr) {
        return {0, 0};
    }
    
    // Jika interval node sepenuhnya di dalam [ql, qr]
    if (ql <= l && r <= qr) {
        return {tree[node].sum, tree[node].weighted_sum};
    }

    // Tentukan anak kiri dan kanan berdasarkan bit XOR saat ini
    int bit = (mask >> (depth - 1)) & 1;
    int left_child = 2 * node + bit;
    int right_child = 2 * node + (1 - bit);

    int mid = (l + r) / 2;
    
    // Subtree kiri asli memegang rentang [l, mid], subtree kanan asli [mid+1, r]
    QueryResult res_left = query(left_child, l, mid, ql, qr, mask, depth - 1);
    QueryResult res_right = query(right_child, mid + 1, r, ql, qr, mask, depth - 1);

    // Hitung berapa banyak elemen yang valid di interval kiri query
    int cnt_left = max(0, min(qr, mid) - max(ql, l) + 1);

    QueryResult ans;
    ans.sum = res_left.sum + res_right.sum;
    ans.weighted_sum = res_left.weighted_sum + res_right.weighted_sum + res_right.sum * cnt_left;

    return ans;
}

int main() {
    // Optimasi I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> Q)) return 0;

    A.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    // Hitung K di mana 2^K = N
    K = 0;
    while ((1 << K) < N) {
        K++;
    }

    tree.resize(4 * N);
    build(1, 0, N - 1, K);

    int xor_mask = 0;

    while (Q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x;
            cin >> x;
            xor_mask ^= x;
        } else if (type == 2) {
            int l, r;
            cin >> l >> r;
            QueryResult res = query(1, 0, N - 1, l, r, xor_mask, K);
            cout << res.sum << "\n";
        } else if (type == 3) {
            int l, r;
            cin >> l >> r;
            QueryResult res = query(1, 0, N - 1, l, r, xor_mask, K);
            cout << res.weighted_sum << "\n";
        }
    }

    return 0;
}
