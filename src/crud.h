#include <iostream>
#include <string>
#include <fstream>
#include <limits>



namespace crud {
    struct Mahasiswa {
        int id;
        std::string NPM;
        std::string name;
        std::string jurusan;
    }; 
    
    Mahasiswa readData(std::fstream &dataBase, int posisi){
        Mahasiswa readMahasiswa;
        dataBase.seekg((posisi - 1)*sizeof(Mahasiswa), std::ios::beg);
        dataBase.read(reinterpret_cast<char*>(&readMahasiswa), sizeof(Mahasiswa));
        return readMahasiswa;
    }
    
    void writeData(std::fstream &dataBase, int posisi, Mahasiswa &inputMahasiswa){
        dataBase.seekp((posisi - 1)*sizeof(Mahasiswa), std::ios::beg);
        dataBase.write(reinterpret_cast<char*>(&inputMahasiswa), sizeof(Mahasiswa));
    }

    int getDataSize(std::fstream &dataBase){
        int start;
        int end;
        dataBase.seekg(0, std::ios::beg);
        start = dataBase.tellg();
        dataBase.seekg(0, std::ios::end);
        end = dataBase.tellg();

        return (end - start)/sizeof(Mahasiswa);
    }

    void checkDatabase(std::fstream &dataBase){
        dataBase.open("Database.txt", std::ios::out | std::ios::in | std::ios::binary);

        if (dataBase.is_open()){
            std::cout << "Database ditemukan" << std::endl;
        } else {
            std::cout << "Database tidak ditemukan" << std::endl;
            std::cout << "Membuat database baru" << std::endl;
            dataBase.open("Database.txt", std::ios::trunc | std::ios::out | std::ios::in | std::ios::binary);
        }
    }

    void addDataMahasiswa(std::fstream &dataBase){
        Mahasiswa inputMahasiswa, lastMahasiswa;

        int ukuranData = getDataSize(dataBase);
        
        if(ukuranData == 0){
            inputMahasiswa.id = 1;
        } else {
            lastMahasiswa = readData(dataBase, ukuranData);
            inputMahasiswa.id = lastMahasiswa.id + 1;
        }

        std::cout << "Nama: ";
        std::getline(std::cin, inputMahasiswa.name);
        std::cout << "Jurusan: ";
        std::getline(std::cin, inputMahasiswa.jurusan);
        std::cout << "NPM: ";
        std::getline(std::cin, inputMahasiswa.NPM);

        writeData(dataBase, ukuranData + 1, inputMahasiswa);
    }

    void displayDataMahasiswa(std::fstream &dataBase){
        int size = getDataSize(dataBase);
        Mahasiswa showMahasiswa;
        std::cout << "No\tID\tNPM\tNama\tJurusan" << std::endl;
        for (int i = 1; i <= size; i++){
            showMahasiswa = readData (dataBase, i);
            std::cout << i << "\t";
            std::cout << showMahasiswa.id << "\t";
            std::cout << showMahasiswa.NPM << "\t";
            std::cout << showMahasiswa.name << "\t";
            std::cout << showMahasiswa.jurusan << std::endl;
        }
    }

    void updateRecordMahasiswa(std::fstream &dataBase){
        int id;
        Mahasiswa updateMahasiswa;
        std::cout << "Pilih no: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits < std::streamsize > ::max(), '\n');

        updateMahasiswa = readData(dataBase, id);
        std::cout << "\nMerubah Data Mahasiswa" << std::endl;
        std::cout << "NPM: ";
        std::getline(std::cin, updateMahasiswa.NPM);
        std::cout << "Nama: ";
        std::getline(std::cin, updateMahasiswa.name);
        std::cout << "Jurusan: ";
        std::getline(std::cin, updateMahasiswa.jurusan);

        writeData(dataBase, id, updateMahasiswa);
    }

    void deleteRecordMahasiswa(std::fstream &dataBase){
        int no;
        int offset;
        char isYakin;
        Mahasiswa blankMahasiswa, tempMahasiswa;
        int size = getDataSize(dataBase);
        std::fstream dataSementara;

        std::cout << "Hapus no: ";
        std::cin >> no;

        yakin:
        std::cout << "Apakah anda yakin?(y/n)" << no << std::endl;
        std::cin >> isYakin;
        if (isYakin == 'y'|| isYakin == 'Y'){
            writeData(dataBase, no, blankMahasiswa);
            dataSementara.open("temp.dat", std::ios::trunc | std::ios::out | std::ios::in | std::ios::binary);

            offset = 0;
            
            for(int i = 1; i<=size; i++){
                tempMahasiswa = readData(dataBase, i);
                if (!tempMahasiswa.name.empty()){
                    writeData(dataSementara, i - offset, tempMahasiswa);
                } else {
                    offset++;
                    std::cout << "Data terhapus" << std::endl;
                }
            }

            size = getDataSize(dataSementara);
            dataBase.close();
            dataBase.open("Database.txt", std::ios::trunc | std::ios::out | std::ios::binary);
            dataBase.close();
            dataBase.open("Database.txt", std::ios::out | std::ios::in | std::ios::binary);

            for(int i = 1; i <= size; i++){
                tempMahasiswa = readData(dataSementara, i);
                writeData(dataBase, i, tempMahasiswa);
            }
        } 
        else if (isYakin != 'n' && isYakin != 'N'){
            std::cout << "Input salah harap masukkan nilai yang sesuai" << std::endl;
            goto yakin;

        } else {
            std::cout << "lanjut" << std::endl;
        }
        
    }

    

}

