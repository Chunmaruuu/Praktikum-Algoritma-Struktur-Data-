#include <iostream>
using namespace std;
// VARIABEL KAPASITAS 
int KAPASITAS_PARKIR = 0;  // Kapasitas maksimal area parkir inap
// STRUKTUR DATA KENDARAAN (NODE untuk PTB / Binary Search Tree)
struct Kendaraan {
    string nomorPolisi;
    string namaPemilik;
    string status;  // "Di Parkiran" atau "Di Luar Parkiran"
    Kendaraan* kiri;
    Kendaraan* kanan;
};
// ============================================================
// STRUKTUR DATA STACK (untuk fitur Undo)
// ============================================================
struct NodeStack {
    string nomorPolisi;
    string aksi;  // "masuk" atau "keluar"
    NodeStack* bawah;
};
struct Stack {
    NodeStack* atas;
};

// ============================================================
// STRUKTUR DATA QUEUE (untuk antrian kendaraan)
// ============================================================
struct NodeQueue {
    string nomorPolisi;
    NodeQueue* selanjutnya;
};

struct Queue {
    NodeQueue* depan;
    NodeQueue* belakang;
};
// ============================================================
// VARIABEL GLOBAL
// ============================================================
Kendaraan* akarPTB = NULL;   // Akar pohon BST
Stack stackUndo;              // Stack untuk undo
Queue antrianParkir;          // Queue untuk antrian
int jumlahDiParkiran = 0;     // Penghitung kendaraan di dalam parkiran

// ============================================================
// FUNGSI-FUNGSI PTB (Binary Search Tree)
// ============================================================

// Fungsi membuat node kendaraan baru
Kendaraan* buatKendaraan(string nomPol, string nama) {
    Kendaraan* baru = new Kendaraan;
    baru->nomorPolisi = nomPol;
    baru->namaPemilik = nama;
    baru->status = "Di Luar Parkiran";
    baru->kiri = NULL;
    baru->kanan = NULL;
    return baru;
}

// Fungsi memasukkan kendaraan ke PTB (rekursif)
Kendaraan* masukkanPTB(Kendaraan* node, string nomPol, string nama) {
    if (node == NULL) {
        return buatKendaraan(nomPol, nama);
    }
    if (nomPol < node->nomorPolisi) {
        node->kiri = masukkanPTB(node->kiri, nomPol, nama);
    } else if (nomPol > node->nomorPolisi) {
        node->kanan = masukkanPTB(node->kanan, nomPol, nama);
    } else {
        cout << "Nomor polisi sudah ada di sistem!" << endl;
    }
    return node;
}

// Fungsi mencari kendaraan berdasarkan nomor polisi
Kendaraan* cariKendaraan(Kendaraan* node, string nomPol) {
    if (node == NULL) {
        return NULL;
    }
    if (nomPol == node->nomorPolisi) {
        return node;
    } else if (nomPol < node->nomorPolisi) {
        return cariKendaraan(node->kiri, nomPol);
    } else {
        return cariKendaraan(node->kanan, nomPol);
    }
}

// Fungsi mencari node terkecil (dipakai saat hapus node)
Kendaraan* cariTerkecil(Kendaraan* node) {
    while (node->kiri != NULL) {
        node = node->kiri;
    }
    return node;
}

// Fungsi menghapus kendaraan dari PTB
Kendaraan* hapusDariPTB(Kendaraan* node, string nomPol) {
    if (node == NULL) {
        return NULL;
    }
    if (nomPol < node->nomorPolisi) {
        node->kiri = hapusDariPTB(node->kiri, nomPol);
    } else if (nomPol > node->nomorPolisi) {
        node->kanan = hapusDariPTB(node->kanan, nomPol);
    } else {
        // Node ditemukan, proses hapus
        if (node->kiri == NULL && node->kanan == NULL) {
            // Kasus 1: node tidak punya anak
            delete node;
            return NULL;
        } else if (node->kiri == NULL) {
            // Kasus 2: hanya punya anak kanan
            Kendaraan* temp = node->kanan;
            delete node;
            return temp;
        } else if (node->kanan == NULL) {
            // Kasus 3: hanya punya anak kiri
            Kendaraan* temp = node->kiri;
            delete node;
            return temp;
        } else {
            // Kasus 4: punya dua anak, ganti dengan successor terkecil dari kanan
            Kendaraan* pengganti = cariTerkecil(node->kanan);
            node->nomorPolisi = pengganti->nomorPolisi;
            node->namaPemilik = pengganti->namaPemilik;
            node->status = pengganti->status;
            node->kanan = hapusDariPTB(node->kanan, pengganti->nomorPolisi);
        }
    }
    return node;
}

// Fungsi tampil semua kendaraan (in-order traversal: urut abjad)
void tampilSemuaKendaraan(Kendaraan* node) {
    if (node == NULL) return;
    tampilSemuaKendaraan(node->kiri);
    cout << "  Nomor Polisi : " << node->nomorPolisi << endl;
    cout << "  Nama Pemilik : " << node->namaPemilik << endl;
    cout << "  Status       : " << node->status << endl;
    cout << "  ----------------------------" << endl;
    tampilSemuaKendaraan(node->kanan);
}

// Fungsi cek apakah PTB kosong
bool ptbKosong(Kendaraan* node) {
    return node == NULL;
}

// ============================================================
// FUNGSI-FUNGSI STACK (untuk Undo)
// ============================================================

// Inisialisasi stack
void inisialisasiStack(Stack& s) {
    s.atas = NULL;
}

// Cek apakah stack kosong
bool stackKosong(Stack& s) {
    return s.atas == NULL;
}

// Push (masukkan data ke stack)
void push(Stack& s, string nomPol, string aksi) {
    NodeStack* baru = new NodeStack;
    baru->nomorPolisi = nomPol;
    baru->aksi = aksi;
    baru->bawah = s.atas;
    s.atas = baru;
}

// Pop (ambil data teratas dari stack)
NodeStack* pop(Stack& s) {
    if (stackKosong(s)) return NULL;
    NodeStack* temp = s.atas;
    s.atas = s.atas->bawah;
    return temp;
}

// ============================================================
// FUNGSI-FUNGSI QUEUE (untuk antrian parkir)
// ============================================================

// Inisialisasi queue
void inisialisasiQueue(Queue& q) {
    q.depan = NULL;
    q.belakang = NULL;
}

// Cek apakah queue kosong
bool queueKosong(Queue& q) {
    return q.depan == NULL;
}

// Enqueue (masukkan ke belakang antrian)
void enqueue(Queue& q, string nomPol) {
    NodeQueue* baru = new NodeQueue;
    baru->nomorPolisi = nomPol;
    baru->selanjutnya = NULL;
    if (queueKosong(q)) {
        q.depan = baru;
        q.belakang = baru;
    } else {
        q.belakang->selanjutnya = baru;
        q.belakang = baru;
    }
}

// Dequeue (ambil dari depan antrian)
string dequeue(Queue& q) {
    if (queueKosong(q)) return "";
    NodeQueue* temp = q.depan;
    string nomPol = temp->nomorPolisi;
    q.depan = q.depan->selanjutnya;
    if (q.depan == NULL) {
        q.belakang = NULL;
    }
    delete temp;
    return nomPol;
}

// ============================================================
// FUNGSI-FUNGSI MENU UTAMA
// ============================================================

// Menu 1: Tambah data kendaraan
void tambahKendaraan() {
    string nomPol, nama;
    cout << "\n== TAMBAH DATA KENDARAAN ==" << endl;
    cout << "Masukkan Nomor Polisi : ";
    cin.ignore();
    getline(cin, nomPol);
    cout << "Masukkan Nama Pemilik : ";
    getline(cin, nama);

    akarPTB = masukkanPTB(akarPTB, nomPol, nama);
    cout << "Data kendaraan berhasil ditambahkan." << endl;
}

// Menu 2: Tampil semua kendaraan
void tampilKendaraan() {
    cout << "\n== DAFTAR KENDARAAN ==" << endl;
    if (ptbKosong(akarPTB)) {
        cout << "Belum ada data kendaraan." << endl;
        return;
    }
    cout << "  ----------------------------" << endl;
    tampilSemuaKendaraan(akarPTB);
}

// Menu 3: Catat kendaraan masuk
void kendaraanMasuk() {
    string nomPol;
    cout << "\n== CATAT KENDARAAN MASUK ==" << endl;
    cout << "Masukkan Nomor Polisi : ";
    cin.ignore();
    getline(cin, nomPol);

    Kendaraan* k = cariKendaraan(akarPTB, nomPol);
    if (k == NULL) {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }
    if (k->status == "Di Parkiran") {
        cout << "Kendaraan sudah berada di dalam parkiran!" << endl;
        return;
    }

    if (jumlahDiParkiran < KAPASITAS_PARKIR) {
        k->status = "Di Parkiran";
        jumlahDiParkiran++;
        push(stackUndo, nomPol, "masuk");
        cout << "Kendaraan berhasil masuk." << endl;
    } else {
        enqueue(antrianParkir, nomPol);
        cout << "Parkiran penuh, kendaraan masuk ke dalam antrian." << endl;
    }
}

// Menu 4: Catat kendaraan keluar
void kendaraanKeluar() {
    string nomPol;
    cout << "\n== CATAT KENDARAAN KELUAR ==" << endl;
    cout << "Masukkan Nomor Polisi : ";
    cin.ignore();
    getline(cin, nomPol);

    Kendaraan* k = cariKendaraan(akarPTB, nomPol);
    if (k == NULL) {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }
    if (k->status == "Di Luar Parkiran") {
        cout << "Kendaraan tidak sedang di dalam parkiran!" << endl;
        return;
    }

    k->status = "Di Luar Parkiran";
    jumlahDiParkiran--;
    push(stackUndo, nomPol, "keluar");
    cout << "Kendaraan berhasil keluar." << endl;

    // Cek apakah ada antrian
    if (!queueKosong(antrianParkir)) {
        string nomPolAntrian = dequeue(antrianParkir);
        Kendaraan* kAntrian = cariKendaraan(akarPTB, nomPolAntrian);
        if (kAntrian != NULL) {
            kAntrian->status = "Di Parkiran";
            jumlahDiParkiran++;
            push(stackUndo, nomPolAntrian, "masuk");
            cout << "Kendaraan berikutnya langsung masuk ke area parkir dari antrian." << endl;
        }
    }
}

// Menu 5: Hapus data kendaraan
void hapusKendaraan() {
    string nomPol;
    cout << "\n== HAPUS DATA KENDARAAN ==" << endl;
    cout << "Masukkan Nomor Polisi : ";
    cin.ignore();
    getline(cin, nomPol);

    Kendaraan* k = cariKendaraan(akarPTB, nomPol);
    if (k == NULL) {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }

    // Kurangi jumlah di parkiran jika kendaraan sedang parkir
    if (k->status == "Di Parkiran") {
        jumlahDiParkiran--;
    }

    akarPTB = hapusDariPTB(akarPTB, nomPol);
    cout << "Data kendaraan berhasil dihapus." << endl;
}

// Menu 6: Undo aksi terakhir
void undoAksi() {
    cout << "\n== UNDO AKSI TERAKHIR ==" << endl;
    if (stackKosong(stackUndo)) {
        cout << "Tidak ada aksi untuk di-undo." << endl;
        return;
    }

    NodeStack* aksiTerakhir = pop(stackUndo);
    Kendaraan* k = cariKendaraan(akarPTB, aksiTerakhir->nomorPolisi);

    if (aksiTerakhir->aksi == "masuk") {
        // Undo masuk = kendaraan keluar dari parkiran
        if (k != NULL) {
            k->status = "Di Luar Parkiran";
            jumlahDiParkiran--;
        }
        cout << "Undo: Kendaraan keluar dari parkiran." << endl;
    } else if (aksiTerakhir->aksi == "keluar") {
        // Undo keluar = kendaraan masuk kembali ke parkiran
        if (k != NULL) {
            k->status = "Di Parkiran";
            jumlahDiParkiran++;
        }
        cout << "Undo: Kendaraan masuk kembali ke parkiran." << endl;
    }

    delete aksiTerakhir;
}

// Menu 7: Tampil antrian kendaraan
void tampilAntrian() {
    cout << "\n== ANTRIAN KENDARAAN ==" << endl;
    if (queueKosong(antrianParkir)) {
        cout << "Antrian kosong." << endl;
        return;
    }

    NodeQueue* temp = antrianParkir.depan;
    while (temp != NULL) {
        cout << temp->nomorPolisi << " menunggu giliran masuk parkiran." << endl;
        temp = temp->selanjutnya;
    }
}

// ============================================================
// FUNGSI MAIN
// ============================================================
int main() {
    // Inisialisasi stack dan queue
    inisialisasiStack(stackUndo);
    inisialisasiQueue(antrianParkir);

    int pilihan;

    cout << "========================================" << endl;
    cout << "  SISTEM PARKIR INAP - BANDARA GSA      " << endl;
    cout << "========================================" << endl;

    // Input kapasitas parkir dari user
    do {
        cout << "Masukkan kapasitas area parkir inap : ";
        cin >> KAPASITAS_PARKIR;
        if (KAPASITAS_PARKIR <= 0) {
            cout << "Kapasitas harus lebih dari 0, coba lagi!" << endl;
        }
    } while (KAPASITAS_PARKIR <= 0);
    cout << "Kapasitas parkir berhasil diatur: " << KAPASITAS_PARKIR << " kendaraan." << endl;

    do {
        cout << "\n--- MENU UTAMA ---" << endl;
        cout << "1. Tambah Data Kendaraan" << endl;
        cout << "2. Tampil Semua Kendaraan" << endl;
        cout << "3. Catat Kendaraan Masuk" << endl;
        cout << "4. Catat Kendaraan Keluar" << endl;
        cout << "5. Hapus Data Kendaraan" << endl;
        cout << "6. Undo Aksi Terakhir" << endl;
        cout << "7. Tampil Antrian Kendaraan" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan Anda : ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahKendaraan();  break;
            case 2: tampilKendaraan();  break;
            case 3: kendaraanMasuk();   break;
            case 4: kendaraanKeluar();  break;
            case 5: hapusKendaraan();   break;
            case 6: undoAksi();         break;
            case 7: tampilAntrian();    break;
            case 0: cout << "\nTerima kasih! Program selesai." << endl; break;
            default: cout << "\nPilihan tidak valid, coba lagi." << endl;
        }

    } while (pilihan != 0);

    return 0;
}
