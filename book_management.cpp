#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Buku {
    string judul;
    string penulis;
    int tahunTerbit;
};

void tambahBuku(vector<Buku>& inventaris) {
    Buku bukuBaru;
    cout << "Masukkan judul buku: ";
    getline(cin, bukuBaru.judul);

    // Validasi judul tidak boleh kosong
    while (bukuBaru.judul.empty()) {
        cout << "Judul tidak boleh kosong. Masukkan judul buku: ";
        getline(cin, bukuBaru.judul);
    }

    cout << "Masukkan penulis buku: ";
    getline(cin, bukuBaru.penulis);

    while (bukuBaru.penulis.empty()) {
        cout << "Penulis tidak boleh kosong. Masukkan penulis buku: ";
        getline(cin, bukuBaru.penulis);
    }

    cout << "Masukkan tahun terbit buku: ";
    cin >> bukuBaru.tahunTerbit;
    cin.ignore();

    inventaris.push_back(bukuBaru);
    cout << "Buku berhasil ditambahkan ke Inventaris buku." << endl;
}

bool cariBuku(const Buku& buku, const string& kueriCari) {
    string judulKecil = buku.judul;
    string penulisKecil = buku.penulis;
    transform(judulKecil.begin(), judulKecil.end(), judulKecil.begin(), ::tolower);
    transform(penulisKecil.begin(), penulisKecil.end(), penulisKecil.begin(), ::tolower);

    string kueriKecil = kueriCari;
    transform(kueriKecil.begin(), kueriKecil.end(), kueriKecil.begin(), ::tolower);

    return (judulKecil.find(kueriKecil) != string::npos || penulisKecil.find(kueriKecil) != string::npos);
}

bool cariBukuDiInventaris(const vector<Buku>& inventaris, const string& kueriCari) {
    bool ditemukan = false;

    cout << left << setw(30) << "Judul" << setw(30) << "Penulis" << setw(15) << "Tahun Terbit" << endl;
    cout << setfill('-') << setw(75) << "-" << setfill(' ') << endl;

    for (const Buku& buku : inventaris) {
        if (cariBuku(buku, kueriCari)) {
            cout << setw(30) << buku.judul << setw(30) << buku.penulis << setw(15) << buku.tahunTerbit << endl;
            ditemukan = true;
        }
    }

    return ditemukan;
}

void editBuku(vector<Buku>& inventaris) {
    string kueriCari;
    cout << "Masukkan judul atau penulis buku yang ingin diedit: ";
    getline(cin, kueriCari);

    for (Buku& buku : inventaris) {
        if (cariBuku(buku, kueriCari)) {
            cout << "Mengedit buku: " << buku.judul << " oleh " << buku.penulis << " (Tahun Terbit: " << buku.tahunTerbit << ")\n";

            cout << "Masukkan judul baru: ";
            getline(cin, buku.judul);

            while (buku.judul.empty()) {
                cout << "Judul tidak boleh kosong. Masukkan judul buku: ";
                getline(cin, buku.judul);
            }

            cout << "Masukkan penulis baru: ";
            getline(cin, buku.penulis);

            while (buku.penulis.empty()) {
                cout << "Penulis tidak boleh kosong. Masukkan penulis buku: ";
                getline(cin, buku.penulis);
            }

            cout << "Masukkan tahun terbit baru: ";
            cin >> buku.tahunTerbit;
            cin.ignore();

            cout << "Informasi buku berhasil diupdate." << endl;
            return;
        }
    }

    cout << "Buku tidak ditemukan dalam Inventaris buku." << endl;
}

void hapusBuku(vector<Buku>& inventaris) {
    string kueriHapus;
    cout << "Masukkan judul atau penulis buku yang ingin dihapus: ";
    getline(cin, kueriHapus);

    auto it = remove_if(inventaris.begin(), inventaris.end(), [kueriHapus](const Buku& buku) {
        return cariBuku(buku, kueriHapus);
    });

    if (it != inventaris.end()) {
        inventaris.erase(it, inventaris.end());
        cout << "Buku berhasil dihapus dari Inventaris buku." << endl;
    } else {
        cout << "Buku tidak ditemukan dalam Inventaris buku." << endl;
    }
}

void tampilkanInventaris(const vector<Buku>& inventaris) {
    if (inventaris.empty()) {
        cout << "Inventaris kosong." << endl;
        return;
    }

    cout << left << setw(30) << "Judul" << setw(30) << "Penulis" << setw(15) << "Tahun Terbit" << endl;
    cout << setfill('-') << setw(75) << "-" << setfill(' ') << endl;

    for (const Buku& buku : inventaris) {
        cout << setw(30) << buku.judul << setw(30) << buku.penulis << setw(15) << buku.tahunTerbit << endl;
    }
}

void simpanKeFile(const vector<Buku>& inventaris, const string& namaFile) {
    ofstream file(namaFile);
    if (file.is_open()) {
        for (const Buku& buku : inventaris) {
            file << buku.judul << ";" << buku.penulis << ";" << buku.tahunTerbit << "\n";
        }
        file.close();
        cout << "Inventaris berhasil disimpan ke file: " << namaFile << endl;
    } else {
        cerr << "Gagal membuka file untuk penyimpanan." << endl;
    }
}

void muatDariFile(vector<Buku>& inventaris, const string& namaFile) {
    ifstream file(namaFile);
    if (file.is_open()) {
        Buku buku;
        while (getline(file, buku.judul, ';') && getline(file, buku.penulis, ';') && (file >> buku.tahunTerbit)) {
            inventaris.push_back(buku);
            file.ignore();
        }
        file.close();
        cout << "Inventaris berhasil dimuat dari file: " << namaFile << endl;
    } else {
        cerr << "Gagal membuka file untuk pemuatan." << endl;
    }
}

int main() {
    cout << "Selamat datang di Inventaris Buku" << endl;
    vector<Buku> inventaris; 
    const string namaFile = "inventaris_buku.txt"; 

    muatDariFile(inventaris, namaFile);

    while (true) {
        cout << "1. Tambahkan Buku\n";
        cout << "2. Cari Buku\n";
        cout << "3. Edit Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Tampilkan Inventaris\n";
        cout << "6. Simpan dan Keluar\n";
        cout << "Masukkan pilihan Anda (1-6): ";

        int pilihan;
        cin >> pilihan;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Pilihan anda tidak valid. Masukkan angka antara 1 dan 6.\n";
            continue;
        }

        cin.ignore();

        switch (pilihan) {
            case 1:
                tambahBuku(inventaris);
                break;
            case 2: {
                string kueriCari;
                cout << "Masukkan judul atau penulis untuk dicari: ";
                getline(cin, kueriCari);
                if (!cariBukuDiInventaris(inventaris, kueriCari)) {
                    cout << "Maaf, buku yang anda cari tidak ditemukan." << endl;
                }
                break;
            }
            case 3:
                editBuku(inventaris);
                break;
            case 4:
                hapusBuku(inventaris);
                break;
            case 5:
                tampilkanInventaris(inventaris);
                break;
            case 6:
                simpanKeFile(inventaris, namaFile);
                cout << "Terima kasih anda sudah mengunjungi inventaris buku. Data berhasil disimpan.\n";
                return 0; // Keluar dari program
            default:
                cout << "Pilihan anda tidak valid. Masukkan angka antara 1 dan 6.\n";
        }
    }

    return 0;
}
