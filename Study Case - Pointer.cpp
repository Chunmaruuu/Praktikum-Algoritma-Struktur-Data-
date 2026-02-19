#include <iostream>  // Library untuk input dan output
using namespace std; //tidak perlu menulis std:: 

int main() { 
    int angka[] = {1,2,3,5,10,15,20,21,25,27};   // Membuat array integer
    int n = sizeof(angka) / sizeof(angka[0]);   //jumlah elemen dalam array
    int *ptr = angka;     // Pointer menunjuk ke alamat awal array
    cout << "Isi array:\n";     //tampilkan isi array 
    for(int i = 0; i < n; i++) {  //Perulangan untuk menampilkan elemen
        cout << *(ptr + i) << " ";  }  //akses elemen menggunakan pointer

    cout << endl;   // Pindah baris biar gak dempet tampilannya
    int max = *ptr;  //menganggap elemen pertama sebagai nilai max awal
   
    int min = *ptr; //Menganggap elemen pertama sebagai nilai min awal  
   
    int jumlah = 0;    //Variabel untuk menyimpan total jumlah 

    for(int i = 0; i < n; i++) {    // Perulangan untuk mencari max, min, dan jumlah
        if(*(ptr + i) > max) {  //jika elemen lebih besar dari max
            max = *(ptr + i);  //update nilai max
    }

        if(*(ptr + i) < min) {  //jika elemen lebih kecil dari min
            min = *(ptr + i);  //update nilai min
    }

        jumlah += *(ptr + i);  //nambah elemen ke total jumlah
    }

    double rata = (double)jumlah / n;  //hitung rata-rata
    cout << "\n\n--- Hasil ---" << endl; // biar estetik
    cout << "\nNilai maksimum  : " << max << endl;   // menampilkan nilai max
    cout << "Nilai minimum   : " << min << endl;   // menampilkan nilai min
    cout << "Jumlah          : " << jumlah << endl;  // menampilkan jumlah
    cout << "Rata-rata       : " << rata << endl;  // menampilkan rata-rata
 
    return 0;  // Program selesai,
    
}

