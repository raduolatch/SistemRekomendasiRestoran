#include <bits/stdc++.h>
using namespace std;

struct Restoran {
    string nama;
    string kategori;
    string alamat;
    double rating;
    double jarak;
};

void tampilkanRestoran(const vector<Restoran>& restoran) {
    if (restoran.empty()) {
        cout << "\nTidak ada restoran yang dapat ditampilkan.\n";
        return;
    }

    cout << "\nDaftar restoran:\n";
    cout << setw(4) << "No"
         << setw(25) << "Nama"
         << setw(18) << "Kategori"
         << setw(24) << "Alamat"
         << setw(10) << "Rating"
         << setw(12) << "Jarak (km)" << '\n';
    cout << string(93, '-') << '\n';

    cout << fixed << setprecision(1);
    for (size_t i = 0; i < restoran.size(); ++i) {
        cout << setw(4) << i + 1
             << setw(25) << restoran[i].nama
             << setw(18) << restoran[i].kategori
             << setw(24) << restoran[i].alamat
             << setw(7) << restoran[i].rating << "/5"
             << setw(12) << restoran[i].jarak << '\n';
    }
}

vector<Restoran> urutkanBerdasarkanRating(const vector<Restoran>& restoran) {
    vector<Restoran> hasil = restoran;
    sort(hasil.begin(), hasil.end(),
         [](const Restoran& a, const Restoran& b) {
             if (a.rating != b.rating) {
                 return a.rating > b.rating;
             }
             return a.jarak < b.jarak;
         });
    return hasil;
}

vector<Restoran> urutkanBerdasarkanJarak(const vector<Restoran>& restoran) {
    vector<Restoran> hasil = restoran;
    sort(hasil.begin(), hasil.end(),
         [](const Restoran& a, const Restoran& b) {
             if (a.jarak != b.jarak) {
                 return a.jarak < b.jarak;
             }
             return a.rating > b.rating;
         });
    return hasil;
}

void tampilkanRekomendasi(const vector<Restoran>& restoran) {
    vector<Restoran> rekomendasi = restoran;
    sort(rekomendasi.begin(), rekomendasi.end(),
         [](const Restoran& a, const Restoran& b) {
             const double skorA = a.rating / (a.jarak + 1.0);
             const double skorB = b.rating / (b.jarak + 1.0);
             return skorA > skorB;
         });

    if (rekomendasi.size() > 5) {
        rekomendasi.resize(5);
    }

    cout << "\nRekomendasi terbaik berdasarkan rating dan jarak:\n";
    cout << "(Skor rekomendasi = rating / (jarak + 1))\n";
    tampilkanRestoran(rekomendasi);
}
void tampilPencarianKat(const vector<Restoran>& restoran, string jenis){
    int urutan=0;
    vector<Restoran> pencarianKat=restoran;
    for(int i=0;i<pencarianKat.size();i++){
        if(pencarianKat[i].kategori==jenis){
            cout << setw(4) << ++urutan
             << setw(25) << restoran[i].nama
             << setw(18) << restoran[i].kategori
             << setw(24) << restoran[i].alamat
             << setw(7) << restoran[i].rating << "/5"
             << setw(12) << restoran[i].jarak << '\n';
        }
    }
}
void tampilJarak(const vector<Restoran>& restoran, int distance){
    vector<Restoran> pencarianJarak=restoran;
    int a=0;

    for(int i=0;i<pencarianJarak.size();i++){
        if(pencarianJarak[i].jarak>=distance && pencarianJarak[i].jarak<distance+1){
            cout << setw(4) << ++a
             << setw(25) << restoran[i].nama
             << setw(18) << restoran[i].kategori
             << setw(24) << restoran[i].alamat
             << setw(7) << restoran[i].rating << "/5"
             << setw(12) << restoran[i].jarak << '\n';
        }
    }
}
int bacaPilihan() {
    int pilihan;
    while (!(cin >> pilihan)) {
        cout << "Input harus berupa angka. Coba lagi: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return pilihan;
}

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

    cout << "===============================================\n";
    cout << "   SISTEM REKOMENDASI RESTORAN / KULINER KOTA MEDAN\n";
    cout << "===============================================\n";
    cout << "Data nama dan lokasi kuliner berada di Kota Medan.\n";
    cout << "Rating dan jarak merupakan data simulasi untuk tugas.\n";

    int pilihan;
    do {
        cout << "\nMenu:\n";
        cout << "1. Tampilkan rekomendasi terbaik\n";
        cout << "2. Urutkan berdasarkan rating tertinggi\n";
        cout << "3. Urutkan berdasarkan jarak terdekat\n";
        cout << "4. Tampilkan semua restoran\n";
        cout << "5. Cari berdasarkan kategori jenis (makanan/minuman)\n";
        cout << "6. Cari berdasarkan jarak\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";
        pilihan = bacaPilihan();

        switch (pilihan) {
        case 1:
            tampilkanRekomendasi(restoran);
            break;
        case 2: {
            vector<Restoran> hasil = urutkanBerdasarkanRating(restoran);
            tampilkanRestoran(hasil);
            break;
        }
        case 3: {
            vector<Restoran> hasil = urutkanBerdasarkanJarak(restoran);
            tampilkanRestoran(hasil);
            break;
        }
        case 4:
            tampilkanRestoran(restoran);
            break;
        case 5:{
            vector<Restoran> hasil=restoran;
            string jenis;

            cout<<"Silakan masukkan kategori (Makanan/Minuman): ";
            cin>>jenis;
            
            tampilPencarianKat(hasil, jenis);
            break;
        }
        case 6:{
            int distance;
            vector<Restoran> hasil=restoran;
            cout<<"Cari berdasarkan jarak: ";
            cin>>distance;
            tampilJarak(hasil, distance);
        }
        case 0:
            cout << "\nTerima kasih telah menggunakan sistem rekomendasi.\n";
            break;
        default:
            cout << "\nPilihan tidak tersedia. Silakan pilih menu 0 sampai 4.\n";
        }
    } while (pilihan != 0);

    return 0;
}