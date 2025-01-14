#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib> // Для rand() и srand()
#include <ctime>   // Для time()

using namespace std;
using namespace chrono;

vector<int> generate_random_packet(int size) {
    vector<int> packet;
    for (int i = 0; i < size; i++) {
        packet.push_back(rand() % 2);
    }
    return packet;
}

vector<int> computeCRC(const vector<int>& packet, const vector<int>& polynomial) {
    // Найдем индекс первого ненулевого бита в полиноме
    int first_non_zero_index = 0;
    while (first_non_zero_index < polynomial.size() && polynomial[first_non_zero_index] == 0) {
        first_non_zero_index++;
    }

    // Если весь полином состоит из нулей, возвращаем пустой CRC
    if (first_non_zero_index == polynomial.size()) {
        return {};
    }

    // Создаем пакет с добавленными нулями
    vector<int> packet_zeros(packet.begin(), packet.end());
    packet_zeros.resize(packet.size() + polynomial.size() - 1, 0);

    // Выполняем деление, начиная с первого ненулевого бита полинома
    for (int i = 0; i <= packet.size(); ++i) {
        if (packet_zeros[i] == 1) {
            for (int j = first_non_zero_index; j < polynomial.size(); ++j) {
                packet_zeros[i + j] ^= polynomial[j];
            }
        }
    }

    // Извлекаем CRC из конца пакета
    vector<int> CRC(packet_zeros.end() - (polynomial.size() + first_non_zero_index - 1), packet_zeros.end());
    return CRC;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел
    int size = 1000;
    vector<float> times;
    vector<int> packet = generate_random_packet(size);
    
    for (int poly = 0; poly <= 255; ++poly) {
        vector<int> polynomial;
        for (int i = 7; i >= 0; --i) {
            polynomial.push_back((poly >> i) & 1);
        }



        auto start = high_resolution_clock::now();
        vector<int> CRC = computeCRC(packet, polynomial);
        auto end = high_resolution_clock::now();
        
        auto duration = duration_cast<microseconds>(end - start);
        float time_in_us = duration.count();
        times.push_back(time_in_us);
    }

    ofstream outputFile("crc_time.txt");
    for (int i = 0; i <= 255; ++i) {
        outputFile << i << ", " << times[i] << endl;
    }
    outputFile.close();
    
    return 0;
}
