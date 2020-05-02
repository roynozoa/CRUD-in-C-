#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include "crud.h"

using namespace std;


void mainMenu();
char getOption();
void exitScreen();


int main(){
    mainMenu();
    cin.get();
    return 0;
}

char getOption(){
    char input;
    system("cls");
    cout << "========== CRUD in C++ ==========" << endl;
    cout << "=================================" << endl;
    cout << "1. Tambah data mahasiswa" << endl;
    cout << "2. Tampilkan data mahasiswa" << endl;
    cout << "3. Ubah data mahasiswa" << endl;
    cout << "4. Hapus data mahasiswa" << endl;
    cout << "5. Keluar" << endl;
    cout << "=================================" << endl;
    cout << "Masukkan pilihan anda:";
    cin >> input;
    cin.ignore(numeric_limits < streamsize > ::max(), '\n');
    return input;
}

void exitScreen(){
    system("cls");
    cout << "==========================================" << endl;
    cout << " Terima kasih telah memakai program ini!! " << endl;
    cout << "==========================================" << endl;
    cin.get();   
}

void mainMenu(){
    fstream dataBase;
    crud::checkDatabase(dataBase);
    char userInput = getOption();
    char isContinue;
    enum userOption{CREATE = '1', READ, UPDATE, DELETE, EXIT};

    while(userInput != EXIT){
        switch (userInput){
        case CREATE :
            cout << "Menambahkan data mahasiswa:" << endl;
            crud::addDataMahasiswa(dataBase);
            break;
        case READ :
            cout << "Menampilkan data mahasiswa:" << endl;
            crud::displayDataMahasiswa(dataBase);
            break;
        case UPDATE :
            cout << "Mengubah data mahasiswa:" << endl;
            crud::displayDataMahasiswa(dataBase);
            crud::updateRecordMahasiswa(dataBase);
            crud::displayDataMahasiswa(dataBase);
            break;
        case DELETE :
            cout << "Menghapus data mahasiswa:" << endl;
            crud::displayDataMahasiswa(dataBase);
            crud::deleteRecordMahasiswa(dataBase);
            crud::displayDataMahasiswa(dataBase);
            break;
        default:
            cout << "Pilihan salah, harap masukan input yang sesuai" << endl;
            break;
        }

        Continue:
        cout << "Anda ingin melanjutkan program?(y/n)";
        cin >> isContinue;
        if (isContinue == 'y'|| isContinue == 'Y'){
            userInput = getOption();
        } else if (isContinue == 'n'|| isContinue == 'N'){
            break;
        } else {
            cout << "\nInput salah, harap masukkan input yang sesuai";
            goto Continue;
        }
    }
    exitScreen();
}