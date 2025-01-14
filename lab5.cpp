#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

vector<int> generate_random_packet(int size){
    vector<int> packet;
    for (int i = 0; i < size; i++){
        packet.push_back(rand() % 2);
    }

    return packet;
}

vector<int> computeCRC(const vector<int>& packet, const vector<int>& polynomial){
    vector<int> packet_zeros(packet.begin(), packet.end());
    packet_zeros.resize(packet.size() + polynomial.size() - 1, 0);
    for (int i = 0; i <= packet.size(); ++i){
        if (packet_zeros[i] == 1) {
            for (int j = 0; j < polynomial.size(); ++j) {
                packet_zeros[i + j] ^= polynomial[j];
            }
        }
    }

    vector<int> CRC(packet_zeros.end() - (polynomial.size() - 1), packet_zeros.end());
    return CRC;
}

bool checkPacket(const vector<int>& receivedPacket, const vector<int>& polynomial){
    vector<int> result = computeCRC(receivedPacket, polynomial);

    for (int bit : result) {
        if (bit != 0){
            return false;
        }
    }
    return true;
}

int main(){
    int size = 500;
    vector<int> packet = generate_random_packet(size);
    cout << "Пакет длиной " << size << " бит: ";
    for (int i : packet){
        cout << i;
    }

    cout << endl;

    cout << "CRC: ";
    vector<int> G = {1, 1, 1, 1, 0, 0, 0, 1};
    vector<int> CRC = computeCRC(packet, G);

    for (int i : CRC){
        cout << i;
    }

    cout << endl;

    vector<int> transmittedPacket(packet);
    transmittedPacket.insert(transmittedPacket.end(), CRC.begin(), CRC.end());
    cout << "Пакет для передачи: ";
    for (int i : transmittedPacket){
        cout << i;
    }

    cout << endl;

    if (checkPacket(transmittedPacket, G)) {
        cout << "Пакет передан без ошибок." << endl;
    } else {
        cout << "Обнаружена ошибка в пакете." << endl;
    }

    cout << endl;

    size = 250;
    packet = generate_random_packet(size);
    cout << "Пакет длиной " << size << " бит: ";
    for (int i : packet){
        cout << i;
    }

    cout << endl;

    cout << "CRC: ";
    CRC = computeCRC(packet, G);

    for (int i : CRC){
        cout << i;
    }

    cout << endl;

    vector<int> transmittedPacket_250(packet);
    transmittedPacket_250.insert(transmittedPacket_250.end(), CRC.begin(), CRC.end());
    cout << "Пакет для передачи: ";
    for (int i : transmittedPacket_250){
        cout << i;
    }

    cout << endl;

    if (checkPacket(transmittedPacket_250, G)) {
        cout << "Пакет передан без ошибок." << endl;
    } else {
        cout << "Обнаружена ошибка в пакете." << endl;
    }

    cout << endl;

    float errorsDetected = 0, errorsMissed = 0;
    vector<int> distortedPacket(transmittedPacket_250);
    for (int i = 0; i < transmittedPacket_250.size(); ++i){
        distortedPacket[i] ^= 1;

        if (!checkPacket(distortedPacket, G)) {
            errorsDetected++;
        } else {
            errorsMissed++;
        }
    }

    cout << "Обнаружено ошибок: " << errorsDetected << endl;
    cout << "Пропущено ошибок: " << errorsMissed << endl;

    // при полиноме 7 бит
    vector<float> probability_7bits;
    ofstream outputFile("probability_7bits.txt");
    for(int i = 100; i <= 2500; i += 100){
        packet = generate_random_packet(i);
        CRC = computeCRC(packet, G);
        vector<int> transmittedPacket_i(packet);
        transmittedPacket_i.insert(transmittedPacket_i.end(), CRC.begin(), CRC.end());
        errorsDetected = 0;
        errorsMissed = 0;
        vector<int> distortedPacket_j(transmittedPacket_i);
        for (int j = 0; j < transmittedPacket_i.size() - CRC.size(); ++j){
            
            distortedPacket_j[j] ^= 1;

            if (!checkPacket(distortedPacket_j, G)) {
                errorsDetected++;
            } else {
                errorsMissed++;
            }
        }


        float probability = static_cast<float>(errorsMissed) / static_cast<float>(errorsDetected + errorsMissed);
        probability_7bits.push_back(probability);
        if (outputFile.is_open()) {
            outputFile << probability << ", " << errorsMissed << endl;
        }

    }

    outputFile.close();

    cout << "------------------------------------" << endl;

    // при полиноме 3 бита
    vector<int> G_3bits = {1, 0, 1, 1};
    vector<float> probability_3bits;
    ofstream outputFile2("probability_3bits.txt");
    for(int i = 100; i <= 2500; i += 100){
        packet = generate_random_packet(i);
        CRC = computeCRC(packet, G_3bits);
        vector<int> transmittedPacket_i3(packet);
        transmittedPacket_i3.insert(transmittedPacket_i3.end(), CRC.begin(), CRC.end());
        errorsDetected = 0;
        errorsMissed = 0;
        vector<int> distortedPacket_j3(transmittedPacket_i3);
        for (int j = 0; j < transmittedPacket_i3.size() - CRC.size(); ++j){
            
            distortedPacket_j3[j] ^= 1;

            if (!checkPacket(distortedPacket_j3, G_3bits)) {
                errorsDetected++;
            } else {
                errorsMissed++;
            }
        }


        float probability = static_cast<float>(errorsMissed) / static_cast<float>(errorsDetected + errorsMissed);
        probability_3bits.push_back(probability);
        if (outputFile2.is_open()) {
            outputFile2 << probability << ", " << errorsMissed << endl;
        }
    }

    outputFile2.close();

    return 0;

}
