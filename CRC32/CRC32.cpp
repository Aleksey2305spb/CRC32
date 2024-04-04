//CRC 32.cpp :
//Данная программа написана "в лист" без разбивки на функции
//При запуске в командной строки - <имя файла>, чей CRC32 требуется опреоделить

#include <iostream>
#include <conio.h>
#include <stdbool.h>
#include <fstream>
#include <bitset> 
#include <cstring>
using std::cout;
using std::endl;
void press_any_key();
int main(int argc, char* argv[])
{
    ///////////////////////////////////////////
    // Преобразование ключа в битовый массив //
    ///////////////////////////////////////////
    long int KEY = 0x04C11DB7;
    const short int KEY_size = sizeof(KEY) * CHAR_BIT;
    short int offset = 0;
    for (short int i = (KEY_size - 1); i > 0; i--) {
        if (KEY & (1 << i)) {
            offset = i;
            break;
        }
    }
    bool* KEY_bit;
    try {
        KEY_bit = new bool[offset + 1];
    }
    catch (std::bad_alloc xa)
    {
        cout << "Alloc ERROR" << endl;
        return 1;
    }
    for (short int i = 0; i < offset + 1; i++) {
        if (KEY & (1 << (offset - i))) KEY_bit[i] = 1;
        else KEY_bit[i] = 0;
    } short int KEY_bit_size = (offset + 1);
    /////////////////////////////////////////////////////////
    //Открытие файла и преобразование его в битовый массив //
    /////////////////////////////////////////////////////////
    std::string file_name;
    if (argc < 2)
    {
        cout << "There is no arguments\n";
        press_any_key();
        return 1;
    }
    else
    {
        file_name = argv[1];
    }
    std::fstream file(file_name, std::ios::in | std::ios::binary);
    if (file) cout << "File \"" << file_name << "\" is opened seccessfully" << endl;
    else { cout << "Error"; return 1; }
    char byte;
    const short length_byte = sizeof(byte) * CHAR_BIT; // размер читаемого байта в битах = 8 бит
    long int counter = 0;
    while (file) {
        file.get(byte);
        counter++;
    } counter--;
    cout << "File size is  " << counter << "  byte  ( " << counter * CHAR_BIT << " bit )" << endl;
    bool* bit_file;
    long int bit_file_size = counter * CHAR_BIT + 32;
    long int bit_file_counter = 0;
    try {
        bit_file = new bool[bit_file_size];
    }
    catch (std::bad_alloc xa)
    {
        cout << "Alloc ERROR" << endl;
        return 1;
    }
    file.clear();
    file.seekp(0, std::ios::beg);
    bit_file_counter = 0;
    file.get(byte);
    while (file) {
        for (long int i = 0; i < length_byte; i++) { // i: 0 -> 7
            if (byte & (1 << i))  bit_file[bit_file_counter] = 1;
            else bit_file[bit_file_counter] = 0;
            bit_file_counter++;
        }
        file.get(byte);
    }
    for (long int i = bit_file_counter; i < bit_file_size; i++) {
        bit_file[i] = 0;
    }
    // получен массив  KEY_bit[0] - KEY_bit[KEY_bit_size-1]
    // получен массив  bit_file [0] - bit_file [bit_file_size-1]
    //////////////////////////////////////
    // Наложение кллюча на битовый файл //
    //////////////////////////////////////
    long int carriage = 0;
    while (carriage < bit_file_size - KEY_bit_size) {
        for (short int i = 0; i < KEY_bit_size; i++) {
            bit_file[carriage + i] = bit_file[carriage + i] ^ KEY_bit[i];
        }
        for (short int i = 0; i < bit_file_size; i++) {
            if (bit_file[carriage + i]) {
                carriage = carriage + i;
                break;
            }
        }
    }
    ////////////////////////////////////////////
    // Запись конторльной суммы CRC32 в число //
    ////////////////////////////////////////////
    long int CRC32 = 0;
    short int CRC32_offset = 0;
    for (long int i = carriage; i < bit_file_size; i++) {
        CRC32_offset = bit_file_size - 1 - i;
        if (bit_file[i]) {
            CRC32 |= (1 << CRC32_offset);
        }
    }
    cout << "CRC32-key :  " << KEY << "  " << std::bitset <KEY_size>(KEY) << endl;
    cout << "CRC32 check sum :  " << CRC32 << "  " << std::bitset <KEY_size>(CRC32) << endl;
    delete[] bit_file;
    delete[] KEY_bit;
    file.close();
    press_any_key();
    return 0;
}
void press_any_key()
{
    cout << "Press any key to continue ";
    _getch();
}
