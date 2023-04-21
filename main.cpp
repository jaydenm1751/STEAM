#include <iostream>
#include <SFML/Graphics.hpp>
#include "HashTable.h"
#include "StorePage.h"
#include "PriorityQ.h"
#include <chrono>

using namespace std;

void InitializeMap(string fileName, HashTable& catalogue);

int main() {
    HashTable map;

//    map.insert("Caleb");
//    map.insert("Jayden");
//    map.insert("Maxwell");
//    map.search("Caleb");
//    map.search("aCelb");

    InitializeMap("AppStoreGames", map);

    PriorityQ q;

    q.insert("Caleb");
    q.insert("Jayden");
    q.insert("Maxwell");
    q.insert("Anthony");
    q.extractedVal();
    q.extractedVal();

    StorePage Steam;
    sf::RenderWindow window(sf::VideoMode(Steam.width, Steam.height), "WaterVapor");

    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}

//TODO: Add Nodes to the list from .CSV
void InitializeMap(string fileName, HashTable& catalogue) {
    auto start_time = chrono::steady_clock::now();

    string path = "files/" + fileName + ".csv";
    ifstream storeFile(path);

    while (true) {
        if (!storeFile.is_open()) {
            cerr << "Error opening the file:" << path << endl;
        } else {
            string headerLine;
            getline(storeFile, headerLine);

            string line;
            while(getline(storeFile, line)) {
                istringstream line_stream(line);
                string cell;

                while (getline(line_stream, cell, ',')) {
                    if(cell.empty()) {
                        cout << "N/A\t";
                    }else {
                        cout << cell << "\t";
                    }
                }
                cout << endl;
            }
            storeFile.close();
            break;
        }
    }

    cout << endl;
    auto end_time = chrono::steady_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::seconds>(end_time - start_time);
    cout << "Elapsed time: " << elapsed_time.count() << " sec.\n";
}
