#include <iostream>
#include <string.h>
#include <iomanip>
using namespace std;
#define MAX 100
struct Produk {
    char name[60];
    char category[30];
    double price;
    int stock;
    float rating;
};

Produk katalog[MAX];
int jumlah = 0;

// ====================== OPERASI FILE ======================

void simpanKeFile() {
    FILE *pf = fopen("shopease_db.txt", "w");
    if (pf == NULL) {
        cout << "Gagal membuka file untuk disimpan!" << endl;
        return;
    }
    for (int i = 0; i < jumlah; i++) {
        fprintf(pf, "%s|%s|%.0lf|%d|%.1f\n",katalog[i].name, katalog[i].category,
            katalog[i].price, katalog[i].stock, katalog[i].rating);
    }
    fclose(pf);
}

void muatDariFile() {
    FILE *pf = fopen("shopease_db.txt", "r");
    if (pf == NULL) return;
    int i = 0;
    while (fscanf(pf, " %[^|]|%[^|]|%lf|%d|%f\n",katalog[i].name, katalog[i].category,&katalog[i].price, &katalog[i].stock, &katalog[i].rating) != EOF) {
        i++;
    }
    jumlah = i;
    fclose(pf);
}

// ====================== TAMPILAN ======================

void tampilHeader() {
    cout << "\n";
    cout << "============================================================" << endl;
    cout << "           SHOPEASE - SISTEM MANAJEMEN KATALOG              " << endl;
    cout << "============================================================" << endl;
}

void cetakTabel(Produk arr[], int n) {
    cout << "\n" << left << setw(3) << "No"<< " "<< left<< setw(22) << "Nama Produk"  << " "<< left << setw(14) << "Kategori" << " "
         << right << setw(14) << "Harga (Rp)"   << " "<< right << setw(6)  << "Stok" << " "<< right << setw(7)  << "Rating"<< endl;
    cout << string(3,'-')<< " "<< string(22,'-')<<" "<< string(14,'-') << " " << string(14,'-')<< " " << string(6,'-')<< " "<< string(7,'-')<< endl;
    for (int i = 0; i < n; i++) {
        cout << left  << setw(3)  << i + 1 << " "<< left  << setw(22) << arr[i].name<< " "<< left  << setw(14) << arr[i].category << " "
             << right << setw(14) << fixed << setprecision(0) << arr[i].price  << " "<< right << setw(6)  << arr[i].stock    << " "
             << right << setw(7)  << fixed << setprecision(1) << arr[i].rating<< endl;
    }
    cout << "\nTotal produk: " << n << endl;
}

// ====================== TAMBAH PRODUK ======================

void tambahProduk() {
    if (jumlah >= MAX) {
        cout << "Katalog penuh! Tidak bisa menambah produk baru." << endl;
        return;
    }
    Produk p;
    cout << "\n--- Tambah Produk Baru ---" << endl;

    cout << "Nama Produk    : ";
    cin.ignore();
    cin.getline(p.name, 60);

    cout << "Kategori       : ";
    cin.getline(p.category, 30);

    cout << "Harga (Rp)     : ";
    cin >> p.price;

    cout << "Stok           : ";
    cin >> p.stock;

    cout << "Rating (0-5)   : ";
    cin >> p.rating;

    if (p.rating < 0.0f) p.rating = 0.0f;
    if (p.rating > 5.0f) p.rating = 5.0f;

    katalog[jumlah] = p;
    jumlah++;
    simpanKeFile();
    cout << "\nProduk \"" << p.name << "\" berhasil ditambahkan dan disimpan!" << endl;
}

// ====================== SORTING ======================

// Bubble Sort ascending by name
void bubbleSortNama(Produk arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j].name, arr[j + 1].name) > 0) {
                Produk temp = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Quick Sort descending by rating
int partisi(Produk arr[], int low, int high) {
    float pivot = arr[high].rating;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].rating > pivot) {
            i++;
            Produk temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Produk temp    = arr[i + 1];
    arr[i + 1]     = arr[high];
    arr[high]      = temp;
    return i + 1;
}

void quickSortRating(Produk arr[], int low, int high) {
    if (low < high) {
        int pi = partisi(arr, low, high);
        quickSortRating(arr, low, pi - 1);
        quickSortRating(arr, pi + 1, high);
    }
}

void tampilDaftarProduk() {
    if (jumlah == 0) {
        cout << "Katalog masih kosong." << endl;
        return;
    }
    cout << "\n--- Tampilkan Daftar Produk ---" << endl;
    cout << "1. Urutkan berdasarkan Nama (A-Z)      - Bubble Sort" << endl;
    cout << "2. Urutkan berdasarkan Rating (Tertinggi) - Quick Sort" << endl;
    cout << "Pilihan: ";
    int pilihan;
    cin >> pilihan;

    Produk salinan[MAX];
    for (int i = 0; i < jumlah; i++) salinan[i] = katalog[i];

    if (pilihan == 1) {
        bubbleSortNama(salinan, jumlah);
        cout << "\n[Diurutkan berdasarkan Nama Produk - Ascending]" << endl;
    } else if (pilihan == 2) {
        quickSortRating(salinan, 0, jumlah - 1);
        cout << "\n[Diurutkan berdasarkan Rating - Descending]" << endl;
    } else {
        cout << "Pilihan tidak valid." << endl;
        return;
    }

    cetakTabel(salinan, jumlah);
}

// ====================== SEARCHING (Sequential Search) ======================

void cariProduk() {
    char keyword[60];
    cout << "\n--- Cari Produk ---" << endl;
    cout << "Masukkan nama produk: ";
    cin.ignore();
    cin.getline(keyword, 60);

    int ditemukan = -1;
    for (int i = 0; i < jumlah; i++) {
        char nameLower[60], keyLower[60];
        strcpy(nameLower, katalog[i].name);
        strcpy(keyLower, keyword);
        for (int j = 0; nameLower[j]; j++) nameLower[j] = tolower(nameLower[j]);
        for (int j = 0; keyLower[j]; j++) keyLower[j] = tolower(keyLower[j]);
        if (strstr(nameLower, keyLower) != NULL) {
            ditemukan = i;
            break;
        }
    }

    if (ditemukan == -1) {
        cout << "Produk tidak ditemukan!" << endl;
    } else {
        cout << "\n=== Produk Ditemukan ===" << endl;
        cout << "Nama Produk : " << katalog[ditemukan].name<< endl;
        cout << "Kategori    : " << katalog[ditemukan].category << endl;
        cout << "Harga       : Rp " << fixed << setprecision(0)<< katalog[ditemukan].price<< endl;
        cout << "Stok        : " << katalog[ditemukan].stock << endl;
        cout << "Rating      : " << fixed << setprecision(1)
             << katalog[ditemukan].rating << " / 5.0" << endl;
    }
}

// ====================== MAIN ======================

int main() {
    muatDariFile();
    tampilHeader();
    cout << "Data berhasil dimuat. Total produk: " << jumlah << endl;

    int pilihan;
    do {
        cout << "\n============================================================" << endl;
        cout << "  MENU UTAMA" << endl;
        cout << "  1. Tambah Produk Baru"<< endl;
        cout << "  2. Tampilkan Daftar Produk"<< endl;
        cout << "  3. Cari Produk"<< endl;
        cout << "  4. Keluar dan Simpan"<< endl;
        cout << "============================================================"<< endl;
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: 
            tambahProduk();      
            break;
            case 2:
             tampilDaftarProduk();
            break;
            case 3: 
            cariProduk();         
            break;
            case 4:
                simpanKeFile();
                cout << "\nData telah disimpan. Terima kasih telah menggunakan ShopEase!"<< endl;
                cout << "Sampai jumpa! :)" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    } while (pilihan != 4);

    return 0;
}
