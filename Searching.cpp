
#include <iostream>
#include <iomanip>
using namespace std;
struct Buku {
    string judul;
    int harga;
};
int main() {
    // Data Buku
    Buku daftar[10] = { {"Conan", 95000}, {"Your Name", 110000},{"Suzume", 115000},{"Fairy Tail", 120000},
        {"Naruto", 120000},{"Attack on Titan", 125000},{"One Piece", 130000},{"Demon Slayer", 135000},
        {"No Game No Life", 138000},{"Re:Zero", 145000}};
    int n = 10;
    int menuUtama, menuCari, menuMetode, subMenu;
    string cariNama;
    int cariHarga;
    string kembali;
    do {
		//menu utama 
        cout << "\n=====================================================" << endl;
        cout << "              PROGRAM TOKO WIBU               " << endl;
        cout << "=====================================================" << endl;
        cout << "1. Daftar Buku" << endl;
        cout << "2. Cari Buku" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilihan: "; cin >> menuUtama;

        if (menuUtama == 1) {
            //  fitur lihat daftar buku
            cout << "\nDAFTAR BUKU TERSEDIA:" << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << left << setw(5) << "No" << "| " << setw(25) << "Judul Buku" << "| " << "Harga" << endl;
            cout << "-----------------------------------------------------" << endl;
            for(int i = 0; i < n; i++) {
                cout << left << setw(5) << i + 1 << "| " << setw(25) << daftar[i].judul << "| Rp " << daftar[i].harga << endl;
            }
            cout << "-----------------------------------------------------" << endl;

        } else if (menuUtama == 2) {
            // Fitur pencarian
            bool found = false;
            int index = -1;

            cout << "\nIngin mencari berdasarkan apa?" << endl;
            cout << "1. Nama Buku" << endl;
            cout << "2. Harga Buku" << endl;
            cout << "Pilihan: "; cin >> menuCari;

            if(menuCari == 1) {
                cout << "Masukkan Judul Buku: "; cin.ignore(); getline(cin, cariNama);
            } else {
                cout << "Masukkan Harga Buku: "; cin >> cariHarga;
            }

            cout << "\nPilih Metode Utama:" << endl;
            cout << "1. Sequential Search" << endl;
            cout << "2. Binary Search" << endl;
            cout << "Pilihan: "; cin >> menuMetode;

            if (menuMetode == 1) {
                // Sub Menu senquential 
                cout << "\n--- Opsi Sequential Search ---" << endl;
                cout << "1. Tanpa Sentinel" << endl;
                cout << "2. Dengan Sentinel" << endl;
                cout << "Pilihan: "; cin >> subMenu;

                if (subMenu == 1) {
                    //  Tanpa Sentinel
                    int i = 0;
                    while(i < n && found == false) {
                        bool kondisi = (menuCari == 1) ? (daftar[i].judul == cariNama) : (daftar[i].harga == cariHarga);
                        if (kondisi) {
                            found = true;
                            index = i;
                        } else {
                            i++;
                        }
                    }
                } else {
                    // Dengan Sentinel
                    Buku temp[11];
                    for(int i=0; i<n; i++) temp[i] = daftar[i];
                    
                    int i = 0;
                    if(menuCari == 1) temp[n].judul = cariNama; else temp[n].harga = cariHarga;

                    if(menuCari == 1) {
                        while(temp[i].judul != cariNama) i++;
                    } else {
                        while(temp[i].harga != cariHarga) i++;
                    }

                    if (i < n) {
                        found = true;
                        index = i;
                    }
                }
            } else if (menuMetode == 2) {
                // Binary Search
                int i = 0, j = n - 1, k;
                while ((!found) && (i <= j)) {
                    k = (i + j)/2;
                    bool sama, lebihKecil;
                    if(menuCari == 1)  {
                        sama = (cariNama == daftar[k].judul);
                        lebihKecil = (cariNama < daftar[k].judul);
                    } else{
                        sama = (cariHarga == daftar[k].harga);
                        lebihKecil = (cariHarga < daftar[k].harga);
                    }

                    if (sama){
                        found = true;
                        index = k;
                    } else {
                        if (lebihKecil) j = k - 1; else i = k + 1;
                    }
                }
            }

            // Hasil Pencarian
            cout << "\n-----------------------------------------------------" << endl;
            if (found) {
                cout << "DATA DITEMUKAN!" << endl;
                cout << "Judul : " << daftar[index].judul << " | Harga : Rp " << daftar[index].harga << endl;
                cout << "Posisi: Index ke-" << index << endl;
            } else {
                cout << "DATA TIDAK DITEMUKAN." << endl;
            }
            cout << "-----------------------------------------------------" << endl;
        }
        if (menuUtama != 3) {
            cout << "\nKembali ke Menu Utama? (y/n): "; cin >> kembali;
        } else {
            kembali = "n";
        }
    } while (kembali == "y" || kembali == "Y");
\
    cout << "\nTerima kasih! Program selesai." << endl;
    return 0;
}
