#include <iostream>
#include "HashTable.h"
#include "HashTableApp.h"
#include "PriorityQ.h"
#include <SFML/Graphics.hpp>
#include <chrono>

using namespace std;
void InitializeMapConsole(HashTableConsole& catalogue) {
    auto start_time = chrono::steady_clock::now();

    string path = "files/ConsoleStoreGames.csv";
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
                vector<string> nodeProperties;

                while (getline(line_stream, cell, ',')) {
                    if(cell.empty()) {
                        nodeProperties.push_back(("N/A"));
                    }else {
                        nodeProperties.push_back(cell);
                    }
                }
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

//TODO: Implement the App hash table
void InitializeMapApp(HashTableApp& catalogue) {
    auto start_time = chrono::steady_clock::now();

    string path = "files/AppStoreGames.csv";
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
                vector<string> nodeProperties;

                while (getline(line_stream, cell, ',')) {
                    if(cell.empty()) {
                        nodeProperties.push_back("N/A");
                    }else {
                        nodeProperties.push_back(cell);
                    }
                }
            }
            storeFile.close();
            break;
        }
    }

    cout << endl;
    auto end_time = chrono::steady_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Elapsed time: " << elapsed_time.count() << " ms.\n";
}

int main() {
    HashTableApp map;

//    map.insert("Caleb");
//    map.insert("Jayden");
//    map.insert("Maxwell");
//    map.search("Caleb");
//    map.search("aCelb");

    PriorityQ q;

    q.insert("Caleb");
    q.insert("Jayden");
    q.insert("Maxwell");
    q.insert("Anthony");
    q.extractedVal();
    q.extractedVal();

    sf::RenderWindow window(sf::VideoMode(1500, 850), "WaterVapor Gaming");
    InitializeMapApp(map);


    sf::Font font;
    font.loadFromFile("files/Anuphan-VariableFont_wght.ttf");
    sf::Text cursor;
    sf::String input = "";
    cursor.setFont(font);
    cursor.setCharacterSize(20);
    cursor.setFillColor(sf::Color::Black);

    while (window.isOpen()){
        sf::Event event{};
        while(window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed){
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);
                cout << coordinates.x << ", " << coordinates.y << endl;
            }
            //TODO: put text in box in the center
            if (event.type == sf::Event::TextEntered){
                if (isalpha(event.text.unicode) || isdigit(event.text.unicode) || ispunct(event.text.unicode)
                || isspace(event.text.unicode)){
                    input += event.text.unicode;
                    cursor.setString(input + "|");
                }
            }
            else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::BackSpace) {
                    input = input.substring(0, input.getSize() - 1);
                    cursor.setString(input + "|");
                }
            }
        }



        // order should be clear, draw display.
        window.clear(sf::Color::Blue);
        window.draw(cursor);
        window.display();
    }


    return 0;
}
