#include <iostream>
using namespace std;
// BUBBLE SORT ASCENDING
void bubblesort(string arr[], int n) {
    for(int i = 0; i < n - 1; i++) {// Loop pertama untuk jumlah putaran
        for(int j = 0; j < n - 1; j++) { // Loop kedua untuk membandingkan tetangga
            // Jika nama di kiri lebih besar secara alfabet dari kanan, tukar
            if(arr[j] > arr[j + 1]) {
                string temp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// QUICK SORT DESCENDING
void quick_sort(string array[], int awal, int akhir) {
    int low = awal, high = akhir;
    // Mengambil pivot dari tengah array
    string pivot = array[(awal + akhir) / 2];

    do {
        // DESCENDING, balik logikanya:
        // Cari yang lebih besar dari pivot di sisi kiri
        while (array[low] > pivot) low++;
        // Cari yang lebih kecil dari pivot di sisi kanan
        while (array[high] < pivot) high--;

        if (low <= high) {
            // Tukar posisi elemen
            string temp = array[low];
            array[low] = array[high];
            array[high] = temp;
            
            low++;
            high--;
        }
    } while (low <= high);

    // Rekursif untuk bagian kiri dan kanan
    if (awal < high) quick_sort(array, awal, high);
    if (low < akhir) quick_sort(array, low, akhir);
}

int main() {
    int n;
    cout << "Masukkan jumlah mahasiswa: ";
    cin >> n;
    cin.ignore(); // supaya nama bisa keluar pas pakek spasi
    string daftarMahasiswa[100]; // Array untuk menyimpan nama maksimal 100)
    // Input data dari user
    for (int i = 0; i < n; i++) {
        cout << "Nama mahasiswa ke-" << i + 1 << ": ";
        getline(cin, daftarMahasiswa[i]);
    }
    cout << endl;
    // Jalankan Bubble Sort Ascending
    bubblesort(daftarMahasiswa, n);
    cout <<"Hasil Ascending Bubble Sort "<< endl;
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". "<< daftarMahasiswa[i] << endl;
    }
    //  Jalankan Quick Sort Descending
    // panggil quick_sort dari indeks 0 sampai n-1
    quick_sort(daftarMahasiswa, 0, n - 1);
    cout << "Hasil Descending Quick Sort " << endl;
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". "<< daftarMahasiswa[i]<< endl;
    }
    return 0;
}
