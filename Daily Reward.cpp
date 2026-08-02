#include <bits/stdc++.h>
using namespace std;

// Tabel hadiah bertingkat berdasarkan streak (Hari 1 sampai Hari 7)
const vector<int> REWARDS = {100, 200, 350, 500, 750, 1000, 2500};

struct Player {
    string name;
    int coins = 0;
    int streak = 0;
    int lastClaimedDay = 0;
};

void showMenu() {
    cout << "\n=================================\n";
    cout << "     SYSTEM DAILY REWARD C++     \n";
    cout << "=================================\n";
    cout << "1. Lihat Status Player\n";
    cout << "2. Klaim Daily Reward\n";
    cout << "3. Lewati 1 Hari (Simulasi Waktu)\n";
    cout << "4. Keluar\n";
    cout << "Pilih menu (1-4): ";
}

void showStatus(const Player& player, int currentDay) {
    cout << "\n--- STATISTIK PEMAIN ---\n";
    cout << "Nama Player    : " << player.name << "\n";
    cout << "Total Koin     : " << player.coins << " Gold\n";
    cout << "Streak Saat Ini: " << player.streak << " Hari\n";
    cout << "Hari Sekarang  : Hari ke-" << currentDay << "\n";
    cout << "Status Klaim   : " << (player.lastClaimedDay == currentDay ? "Sudah Di-klaim" : "Belum Di-klaim") << "\n";
}

void claimReward(Player& player, int currentDay) {
    // 1. Cek apakah hari ini sudah pernah klaim
    if (player.lastClaimedDay == currentDay) {
        cout << "\n[!] Anda sudah mengambil reward hari ini! Kembalilah besok.\n";
        return;
    }

    // 2. Cek apakah streak terputus (melewatkan lebih dari 1 hari)
    if (currentDay > player.lastClaimedDay + 1 && player.lastClaimedDay != 0) {
        cout << "\n[!] Anda melewatkan klaim kemarin! Streak Anda di-reset ke 0.\n";
        player.streak = 0;
    }

    // 3. Tambah streak dan hitung reward
    player.streak++;
    
    // Indeks berulang dari 0-6 jika streak melebihi 7 hari
    int dayIndex = (player.streak - 1) % REWARDS.size(); 
    int rewardAmount = REWARDS[dayIndex];

    player.coins += rewardAmount;
    player.lastClaimedDay = currentDay;

    cout << "\n[+] SELAMAT! Klaim Berhasil (Streak Hari ke-" << player.streak << ")\n";
    cout << "[+] Anda Mendapatkan : " << rewardAmount << " Gold!\n";
    
    if (player.streak % 7 == 0) {
        cout << "[★] BONUS JACKPOT! Anda menyelesaikan streak 7 hari penuh!\n";
    }
}

int main() {
    Player player;
    cout << "Masukkan Nama Pemain: ";
    getline(cin, player.name);

    int currentDay = 1;
    int choice;

    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid!\n";
            continue;
        }

        switch (choice) {
            case 1:
                showStatus(player, currentDay);
                break;
            case 2:
                claimReward(player, currentDay);
                break;
            case 3:
                currentDay++;
                cout << "\n[>] Waktu berlalu... Hari berganti ke Hari ke-" << currentDay << ".\n";
                break;
            case 4:
                cout << "\nTerima kasih telah bermain! Sampai jumpa lagi.\n";
                return 0;
            default:
                cout << "\nPilihan menu tidak tersedia.\n";
        }
    }

    return 0;
}
