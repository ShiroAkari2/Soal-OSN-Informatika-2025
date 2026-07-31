#include <bits/stdc++.h>
using namespace std;

// Struktur hasil kueri
struct Node {
    long long sum;    
    long long sum_idx;
};

int N, Q, K;
vector<long long> A;

// Dynamic Segment Tree / Trie berbasis bit
// node: ID node saat ini
// l, r: rentang indeks posisi [l, r]
// depth: kedalaman bit saat ini (K - 1 down to 0)
// mask: XOR mask global saat ini
Node query(int node, int l, int r, int ql, int qr, int mask, int depth) {
    if (r < ql || l > qr) {
        return {0, 0};
    }
    
    // Basis rekursi: Daun (Leaf Node)
    if (l == r) {
        // Indeks asli elemen yang menempati posisi 'l' saat ini adalah (l ^ mask)
        long long val = A[l ^ mask];
        return {val, val * l};
    }

    int mid = (l + r) / 2;
    int left_child = 2 * node;
    int right_child = 2 * node + 1;

    // Jika seluruh interval [l, r] masuk dalam query [ql, qr], 
    // kita bisa mempercepatnya, namun rekursi penuh hingga leaf atau pemangkasan bit
    // tetap sangat cepat karena K <= 19 (kedalaman maksimal 19).
    
    Node res_left = query(left_child, l, mid, ql, qr, mask, depth - 1);
    Node res_right = query(right_child, mid + 1, r, ql, qr, mask, depth - 1);

    return {
        res_left.sum + res_right.sum,
        res_left.sum_idx + res_right.sum_idx
    };
}

// Rekursi yang dioptimasi dengan Segment Tree / Pre-calculated Trie
struct SegmentTree {
    int n;
    int k;
    // tree_sum[node] menyimpan total sum elemen di subtree tersebut (independen dari XOR)
    vector<long long> tree_sum; 
    
    // tree_sum_idx[node][b] menyimpan sum(A[i] * bit_b(i_original)) di subtree
    // Ini membantu menghitung sum(A[i] * i_current) secara instan!
    vector<vector<long long>> tree_bit_sum;

    SegmentTree(int n, int k) : n(n), k(k) {
        tree_sum.resize(4 * n, 0);
        tree_bit_sum.assign(4 * n, vector<long long>(k, 0));
    }

    void build(int node, int l, int r) {
        if (l == r) {
            tree_sum[node] = A[l];
            for (int b = 0; b < k; b++) {
                if ((l >> b) & 1) {
                    tree_bit_sum[node][b] = A[l];
                }
            }
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid);
        build(2 * node + 1, mid + 1, r);

        tree_sum[node] = tree_sum[2 * node] + tree_sum[2 * node + 1];
        for (int b = 0; b < k; b++) {
            tree_bit_sum[node][b] = tree_bit_sum[2 * node][b] + tree_bit_sum[2 * node + 1][b];
        }
    }

    // Mengembalikan {sum(A[i]), sum(A[i] * i_current)} untuk rentang [ql, qr] dengan XOR mask
    Node query_tree(int node, int l, int r, int ql, int qr, int mask) {
        if (r < ql || l > qr) {
            return {0, 0};
        }

        // Jika interval node sepenuhnya di dalam [ql, qr]
        if (ql <= l && r <= qr) {
            long long s = tree_sum[node];
            long long s_idx = 0;

            // Hitung sum(A[i] * i_current) menggunakan bit-sum precalculated
            for (int b = 0; b < k; b++) {
                long long bit_val = (1LL << b);
                int mask_bit = (mask >> b) & 1;
                
                if (mask_bit == 0) {
                    // Jika bit mask 0, kontribusi bit b tetap dari elemen yang bit b-nya 1
                    s_idx += tree_bit_sum[node][b] * bit_val;
                } else {
                    // Jika bit mask 1, bit b dari i_current terbalik (0 jadi 1, 1 jadi 0)
                    // Jumlah elemen dengan bit b = 1 pada i_current adalah (total_elemen - elemen dengan bit b asli = 1)
                    s_idx += (tree_sum[node] - tree_bit_sum[node][b]) * bit_val;
                }
            }
            return {s, s_idx};
        }

        int mid = (l + r) / 2;

        // Penting: Karena XOR membalik indeks pada level bit tertentu,
        // rentang [l, mid] dan [mid+1, r] pada posisi *saat ini* ditukar jika bit terkait pada mask bernilai 1.
        int depth = 31 - __builtin_clz(r - l + 1) - 1; // bit level untuk split ini
        
        int left_child = 2 * node;
        int right_child = 2 * node + 1;

        // Jika bit pada level ini di XOR mask bernilai 1, anak kiri & kanan tertukar posisi fisiknya
        if ((mask >> depth) & 1) {
            swap(left_child, right_child);
        }

        Node res_left = query_tree(left_child, l, mid, ql, qr, mask);
        Node res_right = query_tree(right_child, mid + 1, r, ql, qr, mask);

        return {
            res_left.sum + res_right.sum,
            res_left.sum_idx + res_right.sum_idx
        };
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> Q)) return 0;

    A.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    K = 0;
    while ((1 << K) < N) {
        K++;
    }

    SegmentTree st(N, K);
    st.build(1, 0, N - 1);

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
            Node res = st.query_tree(1, 0, N - 1, l, r, xor_mask);
            cout << res.sum << "\n";
        } else if (type == 3) {
            int l, r;
            cin >> l >> r;
            Node res = st.query_tree(1, 0, N - 1, l, r, xor_mask);
            // Formula Tipe 3: Sum(A[i] * (i - L + 1)) = Sum(A[i] * i) - (L - 1) * Sum(A[i])
            long long ans = res.sum_idx - (long long)(l - 1) * res.sum;
            cout << ans << "\n";
        }
    }

    return 0;
}
