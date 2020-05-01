#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Mahasiswa {
    int id;
    string NPM;
    string name;
    string jurusan;
}; 

char getOption();
void exitScreen();
void checkDatabase(fstream &dataBase);
void addDataMahasiswa(fstream &dataBase);
void displayDataMahasiswa(fstream &dataBase);
void updateRecordMahasiswa(fstream &dataBase);
void deleteRecordMahasiswa(fstream &database);
void writeData(fstream &dataBase, int posisi, Mahasiswa &inputMahasiswa);
int getDataSize(fstream &dataBase);
Mahasiswa readData(fstream &dataBase, int posisi);


int main(){
    fstream dataBase;
    checkDatabase(dataBase);
    char userInput = getOption();
    char isContinue;
    enum userOption{CREATE = '1', READ, UPDATE, DELETE, EXIT};

    while(userInput != EXIT){
        switch (userInput){
        case CREATE :
            cout << "Menambahkan data mahasiswa:" << endl;
            addDataMahasiswa(dataBase);
            break;
        case READ :
            cout << "Menampilkan data mahasiswa:" << endl;
            displayDataMahasiswa(dataBase);
            break;
        case UPDATE :
            cout << "Mengubah data mahasiswa:" << endl;
            displayDataMahasiswa(dataBase);
            updateRecordMahasiswa(dataBase);
            displayDataMahasiswa(dataBase);
            break;
        case DELETE :
            cout << "Menghapus data mahasiswa:" << endl;
            displayDataMahasiswa(dataBase);
            deleteRecordMahasiswa(dataBase);
            displayDataMahasiswa(dataBase);
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
}

void checkDatabase(fstream &dataBase){
    dataBase.open("Database.txt", ios::out | ios::in );

    if (dataBase.is_open()){
        cout << "Database ditemukan" << endl;
    } else {
        cout << "Database tidak ditemukan" << endl;
        cout << "Membuat database baru" << endl;
        dataBase.open("Database.txt", ios::trunc | ios::out | ios::in );
    }
}

void addDataMahasiswa(fstream &dataBase){
    Mahasiswa inputMahasiswa, lastMahasiswa;

    int ukuranData = getDataSize(dataBase);
    
    if(ukuranData == 0){
        inputMahasiswa.id = 1;
    } else {
        lastMahasiswa = readData(dataBase, ukuranData);
        inputMahasiswa.id = lastMahasiswa.id + 1;
    }

    cout << "Nama: ";
    getline(cin, inputMahasiswa.name);
    cout << "Jurusan: ";
    getline(cin, inputMahasiswa.jurusan);
    cout << "NPM: ";
    getline(cin, inputMahasiswa.NPM);

    writeData(dataBase, ukuranData + 1, inputMahasiswa);
}

void displayDataMahasiswa(fstream &dataBase){
    int size = getDataSize(dataBase);
    Mahasiswa showMahasiswa;
    cout << "No\tID\tNPM\tNama\tJurusan" << endl;
    for (int i = 1; i <= size; i++){
        showMahasiswa = readData (dataBase, i);
        cout << i << "\t";
        cout << showMahasiswa.id << "\t";
        cout << showMahasiswa.NPM << "\t";
        cout << showMahasiswa.name << "\t";
        cout << showMahasiswa.jurusan << endl;
    }
}

void updateRecordMahasiswa(fstream &dataBase){
    int id;
    Mahasiswa updateMahasiswa;
    cout << "Pilih no: ";
    cin >> id;
    cin.ignore(numeric_limits < streamsize > ::max(), '\n');

    updateMahasiswa = readData(dataBase, id);
    cout << "\nMerubah Data Mahasiswa" << endl;
    cout << "NPM: ";
    getline(cin, updateMahasiswa.NPM);
    cout << "Nama: ";
    getline(cin, updateMahasiswa.name);
    cout << "Jurusan: ";
    getline(cin, updateMahasiswa.jurusan);

    writeData(dataBase, id, updateMahasiswa);
}

void deleteRecordMahasiswa(fstream &database){
    int id;
    int offset;
    char isYakin;
    Mahasiswa blankMahasiswa, tempMahasiswa;
    int size = getDataSize(database);
    fstream dataSementara;

    cout << "Hapus no: ";
    cin >> id;

    yakin:
    cout << "Apakah anda yakin?(y/n)";
    cin >> isYakin;
    if (isYakin == 'y'|| isYakin == 'Y'){
        writeData(database, id, blankMahasiswa);

        dataSementara.open("temp.dat", ios::trunc | ios::in);
        offset = 0;
        for(int i = 1; i<=size; i++){
            tempMahasiswa = readData(database, i);

            if (!tempMahasiswa.name.empty()){
                writeData(dataSementara, i - offset, tempMahasiswa);
            } else {
                offset++;
                cout << "Data terhapus" << endl;
            }
        }

        size = getDataSize(dataSementara);
        database.close();
        database.open("Database.txt", ios::trunc | ios::out);
        database.close();
        database.open("Database.txt", ios::out | ios::in);

        for(int i = 1; i <= size; i++){
            tempMahasiswa = readData(dataSementara, i);
            writeData(database, i, tempMahasiswa);
        }

    } else if (isYakin == 'n'|| isYakin == 'N'){
        goto lanjut;

    } else {
        cout << "Input salah harap masukkan nilai yang sesuai" << endl;
        goto yakin;    
    }

    lanjut:
    getOption();
}

void writeData(fstream &dataBase, int posisi, Mahasiswa &inputMahasiswa){
    dataBase.seekp((posisi - 1)*sizeof(Mahasiswa), ios::beg);
    dataBase.write(reinterpret_cast<char*>(&inputMahasiswa), sizeof(Mahasiswa));
}

int getDataSize(fstream &dataBase){
    int start;
    int end;
    dataBase.seekg(0, ios::beg);
    start = dataBase.tellg();
    dataBase.seekg(0, ios::end);
    end = dataBase.tellg();

    return (end - start)/sizeof(Mahasiswa);
}

Mahasiswa readData(fstream &dataBase, int posisi){
    Mahasiswa readMahasiswa;
    dataBase.seekg((posisi - 1)*sizeof(Mahasiswa), ios::beg);
    dataBase.read(reinterpret_cast<char*>(&readMahasiswa), sizeof(Mahasiswa));
    return readMahasiswa;
}

