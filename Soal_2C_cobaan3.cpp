#include <bits/stdc++.h>
using namespace std;

int N, Q, K;
vector<long long> A;

// tree_sum[node] menyimpan total sum elemen di node tersebut
vector<long long> tree_sum;

// tree_bit_sum[node][b] menyimpan sum(A[i]) di mana bit b dari indeks asli i adalah 1
vector<vector<long long>> tree_bit_sum;

// Membangun Segment Tree berbasis bit
void build(int node, int l, int r, int depth) {
    if (l == r) {
        tree_sum[node] = A[l];
        for (int b = 0; b < K; b++) {
            if ((l >> b) & 1) {
                tree_bit_sum[node][b] = A[l];
            }
        }
        return;
    }
    int mid = (l + r) / 2;
    build(2 * node, l, mid, depth - 1);
    build(2 * node + 1, mid + 1, r, depth - 1);

    tree_sum[node] = tree_sum[2 * node] + tree_sum[2 * node + 1];
    for (int b = 0; b < K; b++) {
        tree_bit_sum[node][b] = tree_bit_sum[2 * node][b] + tree_bit_sum[2 * node + 1][b];
    }
}

struct PrefixResult {
    long long sum;
    long long sum_idx;
};

// Menghitung sum dan sum_idx untuk rentang [0, P] dalam waktu O(K)
PrefixResult query_prefix(int P, int mask) {
    if (P < 0) return {0, 0};

    long long total_sum = 0;
    long long total_sum_idx = 0;

    int curr_prefix = 0; // Prefiks indeks saat ini
    int node = 1;

    for (int b = K - 1; b >= 0; b--) {
        int bit_p = (P >> b) & 1;
        int bit_m = (mask >> b) & 1;

        if (bit_p == 1) {
            // Anak kiri (di bawah urutan XOR) mencakup blok penuh berukuran 2^b
            int full_block_child = 2 * node + bit_m;

            // 1. Hitung total sum dari blok penuh ini
            long long s = tree_sum[full_block_child];
            total_sum += s;

            // 2. Hitung sum_idx dari blok penuh ini
            // Posisi bit >= b ditentukan oleh (curr_prefix ^ (bit_m << b))
            long long base_idx = (curr_prefix ^ (bit_m << b));
            total_sum_idx += s * base_idx;

            // Posisi bit < b dihitung menggunakan tree_bit_sum
            for (int bit_i = 0; bit_i < b; bit_i++) {
                long long bit_val = (1LL << bit_i);
                int m_bit = (mask >> bit_i) & 1;

                if (m_bit == 0) {
                    total_sum_idx += tree_bit_sum[full_block_child][bit_i] * bit_val;
                } else {
                    total_sum_idx += (s - tree_bit_sum[full_block_child][bit_i]) * bit_val;
                }
            }

            // Lanjut ke anak kanan untuk sisa P
            node = 2 * node + (1 - bit_m);
            curr_prefix |= (1 << b);
        } else {
            // Lanjut ke anak kiri
            node = 2 * node + bit_m;
        }
    }

    // Tambahkan elemen terakhir pada P (leaf node)
    long long s = tree_sum[node];
    total_sum += s;
    total_sum_idx += s * P;

    return {total_sum, total_sum_idx};
}

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

    tree_sum.assign(4 * N, 0);
    tree_bit_sum.assign(4 * N, vector<long long>(K, 0));

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
            PrefixResult res_r = query_prefix(r, xor_mask);
            PrefixResult res_l = query_prefix(l - 1, xor_mask);
            
            long long ans = res_r.sum - res_l.sum;
            cout << ans << "\n";
        } else if (type == 3) {
            int l, r;
            cin >> l >> r;
            PrefixResult res_r = query_prefix(r, xor_mask);
            PrefixResult res_l = query_prefix(l - 1, xor_mask);

            long long sum_lr = res_r.sum - res_l.sum;
            long long sum_idx_lr = res_r.sum_idx - res_l.sum_idx;

            long long ans = sum_idx_lr - (long long)(l - 1) * sum_lr;
            cout << ans << "\n";
        }
    }

    return 0;
}
