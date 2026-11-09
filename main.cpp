
#include <bits/stdc++.h>
using namespace std;

struct Restoran {
    string nama;
    string kategori;
    string alamat;
    double rating;
    double jarak;
};

// Struktur edge pada graph
struct Edge {
    int tujuan;
    double bobot;
};

// Graph menggunakan adjacency list
using Graph = vector<vector<Edge>>;

void tampilkanRestoran(const vector<Restoran>& restoran) {
    if (restoran.empty()) {
        cout << "\nTidak ada restoran.\n";
        return;
    }

    cout << "\nDaftar restoran:\n";
    cout << left << setw(4) << "No"
         << setw(25) << "Nama"
         << setw(12) << "Kategori"
         << setw(28) << "Alamat"
         << setw(10) << "Rating"
         << setw(12) << "Jarak (km)" << '\n';

    cout << string(91, '-') << '\n';

    cout << fixed << setprecision(1);

    for (int i = 0; i < (int)restoran.size(); i++) {
        cout << left << setw(4) << i + 1
             << setw(25) << restoran[i].nama
             << setw(12) << restoran[i].kategori
             << setw(28) << restoran[i].alamat
             << setw(10) << restoran[i].rating
             << setw(12) << restoran[i].jarak << '\n';
    }
}

// Sorting berdasarkan rating
vector<Restoran> urutkanBerdasarkanRating(
    const vector<Restoran>& restoran) {

    vector<Restoran> hasil = restoran;

    sort(hasil.begin(), hasil.end(),
        [](const Restoran& a, const Restoran& b) {
            if (a.rating != b.rating)
                return a.rating > b.rating;
            return a.jarak < b.jarak;
        });

    return hasil;
}

// Sorting berdasarkan jarak
vector<Restoran> urutkanBerdasarkanJarak(
    const vector<Restoran>& restoran) {

    vector<Restoran> hasil = restoran;

    sort(hasil.begin(), hasil.end(),
        [](const Restoran& a, const Restoran& b) {
            if (a.jarak != b.jarak)
                return a.jarak < b.jarak;
            return a.rating > b.rating;
        });

    return hasil;
}

// Rekomendasi terbaik
void tampilkanRekomendasi(const vector<Restoran>& restoran) {
    vector<Restoran> rekomendasi = restoran;

    sort(rekomendasi.begin(), rekomendasi.end(),
        [](const Restoran& a, const Restoran& b) {
            double skorA = a.rating / (a.jarak + 1.0);
            double skorB = b.rating / (b.jarak + 1.0);
            return skorA > skorB;
        });

    if (rekomendasi.size() > 5)
        rekomendasi.resize(5);

    cout << "\nRekomendasi terbaik:\n";
    cout << "Skor = rating / (jarak + 1)\n";

    tampilkanRestoran(rekomendasi);
}

// Pencarian berdasarkan kategori
void tampilPencarianKat(
    const vector<Restoran>& restoran, string jenis) {

    vector<Restoran> hasil;

    transform(jenis.begin(), jenis.end(), jenis.begin(),
              [](unsigned char c) { return tolower(c); });

    for (const auto& r : restoran) {
        string kategori = r.kategori;

        transform(kategori.begin(), kategori.end(),
                  kategori.begin(),
                  [](unsigned char c) { return tolower(c); });

        if (kategori == jenis)
            hasil.push_back(r);
    }

    tampilkanRestoran(hasil);
}

// Pencarian berdasarkan interval jarak
void tampilJarak(
    const vector<Restoran>& restoran, int distance) {

    vector<Restoran> hasil;

    for (const auto& r : restoran) {
        if (r.jarak >= distance &&
            r.jarak < distance + 1) {
            hasil.push_back(r);
        }
    }

    tampilkanRestoran(hasil);
}

// ==========================================
// BAGIAN GRAPH
// ==========================================

// Menambahkan edge dua arah
void tambahEdge(Graph& graph, int asal,
                int tujuan, double jarak) {

    graph[asal].push_back({tujuan, jarak});
    graph[tujuan].push_back({asal, jarak});
}

// Membuat graph antarrestoran sejenis
Graph buatGraph(const vector<Restoran>& restoran) {
    Graph graph(restoran.size());

    // Edge kategori MAKANAN
    tambahEdge(graph, 1, 2, 0.9);
    tambahEdge(graph, 1, 8, 0.5);
    tambahEdge(graph, 2, 8, 0.8);
    tambahEdge(graph, 2, 5, 1.3);
    tambahEdge(graph, 5, 11, 1.2);
    tambahEdge(graph, 11, 8, 0.7);
    tambahEdge(graph, 5, 4, 2.0);
    tambahEdge(graph, 4, 6, 1.6);
    tambahEdge(graph, 6, 9, 2.3);
    tambahEdge(graph, 9, 4, 2.0);
    tambahEdge(graph, 9, 5, 2.8);

    // Edge kategori MINUMAN
    tambahEdge(graph, 0, 3, 2.0);
    tambahEdge(graph, 3, 7, 1.1);
    tambahEdge(graph, 7, 10, 2.1);
    tambahEdge(graph, 0, 10, 1.8);
    tambahEdge(graph, 3, 10, 2.4);

    return graph;
}

// ==========================================
// ALGORITMA DIJKSTRA
// ==========================================

void dijkstra(const Graph& graph,
              const vector<Restoran>& restoran,
              int asal, int tujuan) {

    int n = graph.size();

    const double INF = numeric_limits<double>::infinity();

    vector<double> dist(n, INF);
    vector<int> parent(n, -1);

    // Priority queue untuk memilih jarak terkecil
    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    dist[asal] = 0;
    pq.push({0, asal});

    while (!pq.empty()) {
        double jarakSekarang = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (jarakSekarang > dist[u])
            continue;

        for (const Edge& edge : graph[u]) {
            int v = edge.tujuan;
            double bobot = edge.bobot;

            // Hanya boleh melewati restoran sejenis
            if (restoran[v].kategori !=
                restoran[asal].kategori) {
                continue;
            }

            if (dist[u] + bobot < dist[v]) {
                dist[v] = dist[u] + bobot;
                parent[v] = u;

                pq.push({dist[v], v});
            }
        }
    }

    if (dist[tujuan] == INF) {
        cout << "\nRute tidak ditemukan.\n";
        return;
    }

    // Rekonstruksi rute menggunakan parent
    vector<int> rute;

    for (int v = tujuan; v != -1; v = parent[v]) {
        rute.push_back(v);
    }

    reverse(rute.begin(), rute.end());

    cout << "\n====================================\n";
    cout << "        HASIL DIJKSTRA\n";
    cout << "====================================\n";

    cout << "Kategori : " << restoran[asal].kategori << '\n';
    cout << "Asal     : " << restoran[asal].nama << '\n';
    cout << "Tujuan   : " << restoran[tujuan].nama << '\n';

    cout << "\nRute terpendek:\n";

    for (int i = 0; i < (int)rute.size(); i++) {
        cout << restoran[rute[i]].nama;

        if (i < (int)rute.size() - 1)
            cout << " -> ";
    }

    cout << fixed << setprecision(2);
    cout << "\n\nTotal jarak terpendek: "
         << dist[tujuan] << " km\n";
}

// ==========================================
// MENU PENCARIAN RUTE
// ==========================================

        // Deklarasi fungsi
int bacaPilihan();

void cariRute(const Graph& graph,
              const vector<Restoran>& restoran) {

    cout << "\nDaftar restoran:\n";

    for (int i = 0; i < (int)restoran.size(); i++) {
        cout << i + 1 << ". " << restoran[i].nama
             << " (" << restoran[i].kategori << ")\n";
    }

    int asal, tujuan;

    cout << "\nMasukkan nomor restoran asal: ";
    asal = bacaPilihan();

    cout << "Masukkan nomor restoran tujuan: ";
    tujuan = bacaPilihan();

    int n = restoran.size();

    if (asal < 1 || asal > n ||
        tujuan < 1 || tujuan > n) {
        cout << "\nNomor restoran tidak valid.\n";
        return;
    }

    // Mengubah nomor pilihan menjadi indeks array
    asal--;
    tujuan--;

    if (restoran[asal].kategori !=
        restoran[tujuan].kategori) {

        cout << "\nGagal mencari rute!\n";
        cout << "Restoran harus memiliki kategori sama.\n";
        return;
    }

    dijkstra(graph, restoran, asal, tujuan);
}

// Input angka yang aman
int bacaPilihan() {
    int pilihan;

    while (!(cin >> pilihan)) {
        cout << "Input harus angka. Coba lagi: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return pilihan;
}

// ==========================================
// PROGRAM UTAMA
// ==========================================

int main() {

    vector<Restoran> restoran = {
        {"Tianlala", "Minuman", "Jl.Karya Wisata", 4.7, 2.1},
        {"Soto Kesawan", "Makanan", "Jl. Ahmad Yani No. 116", 4.5, 1.2},
        {"Bihun Bebek Asie", "Makanan", "Jl. Kumango No. 15", 4.6, 1.0},
        {"Waiting List Coffee", "Minuman", "Jl. Berdikari", 4.7, 0.7},
        {"Mie Aceh Titi Bobrok", "Makanan", "Jl. Setia Budi No. 17-D", 4.4, 3.5},
        {"Rumah Makan Tabona", "Makanan", "Jl. Mangkubumi No. 17", 4.6, 1.8},
        {"Rumah Makan Sipirok", "Makanan", "Jl. Sunggal No. 14", 4.5, 4.2},
        {"Cold 'N Brew", "Minuman", "Jl. Dr. Mansyur", 4.1, 0.9},
        {"Tip Top Restaurant", "Makanan", "Jl. Ahmad Yani No. 92", 4.4, 1.0},
        {"Ucok Durian", "Makanan", "Jl. Wahid Hasyim No. 30-32", 4.3, 2.7},
        {"Moo Coffee", "Minuman", "Jl. Perdana", 4.9, 2.3},
        {"Merdeka Walk", "Makanan", "Jl. Balai Kota", 4.5, 1.4}
    };

    // Membuat graph satu kali
    Graph graph = buatGraph(restoran);

    cout << "=============================================\n";
    cout << " SISTEM REKOMENDASI RESTORAN KOTA MEDAN\n";
    cout << "=============================================\n";
    cout << "Rating dan jarak adalah data simulasi.\n";

    int pilihan;

    do {
        cout << "\nMenu:\n";
        cout << "1. Tampilkan rekomendasi terbaik\n";
        cout << "2. Urutkan berdasarkan rating tertinggi\n";
        cout << "3. Urutkan berdasarkan jarak terdekat\n";
        cout << "4. Tampilkan semua restoran\n";
        cout << "5. Cari berdasarkan kategori\n";
        cout << "6. Cari berdasarkan jarak\n";
        cout << "7. Cari rute terpendek (Dijkstra)\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";

        pilihan = bacaPilihan();

        switch (pilihan) {
            case 1:
                tampilkanRekomendasi(restoran);
                break;

            case 2:
                tampilkanRestoran(
                    urutkanBerdasarkanRating(restoran));
                break;

            case 3:
                tampilkanRestoran(
                    urutkanBerdasarkanJarak(restoran));
                break;

            case 4:
                tampilkanRestoran(restoran);
                break;

            case 5: {
                string jenis;
                cout << "Masukkan kategori (Makanan/Minuman): ";
                cin >> jenis;
                tampilPencarianKat(restoran, jenis);
                break;
            }

            case 6: {
                int distance;
                cout << "Cari berdasarkan jarak (km): ";
                distance = bacaPilihan();
                tampilJarak(restoran, distance);
                break;
            }

            case 7:
                cariRute(graph, restoran);
                break;

            case 0:
                cout << "\nTerima kasih telah menggunakan sistem.\n";
                break;

            default:
                cout << "\nPilihan tidak tersedia (0-7).\n";
        }

    } while (pilihan != 0);

    return 0;
}
