#include <iostream>
#include "HashTable.h"
#include "HashTableApp.h"
#include "PriorityQ.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "ConsoleAppsNodes.cpp"
#include <chrono>

using namespace std;
void InitializeMapConsole(unordered_map<string, ConsoleNode*>& catalogue) {
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
                insertConsoleData(nodeProperties, catalogue);
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
void InitializeMapApp(unordered_map<string, AppNode*>& catalogue) {
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
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Elapsed time: " << elapsed_time.count() << " ms.\n";
}

void makeText(sf::Text& text, sf::Font& font, string s, int size, int width, int height){
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setString(s);
    text.setPosition(width, height);
}

int main() {
    unordered_map<string, ConsoleNode*> ConsoleGames;

//    map.insert("Caleb");
//    map.insert("Jayden");
//    map.insert("Maxwell");
//    map.search("Caleb");
//    map.search("aCelb");

    PriorityQ q;
    InitializeMapConsole(ConsoleGames);

    q.insert("A");
    q.insert("B");
    q.insert("D");
    q.insert("H");
    q.insert("C");
    q.insert("Anthony");
    q.extractedVal();
    q.extractedVal();
    q.extractedVal();

    int width = 1500;
    int height = 850;


    sf::RenderWindow window(sf::VideoMode(width, height), "WaterVapor Gaming");
    sf::Font font;
    font.loadFromFile("files/CourierPrime-Regular.ttf");
    sf::Text cursor;
    cursor.setFont(font);
    cursor.setCharacterSize(20);
    cursor.setFillColor(sf::Color::Black);
    sf::String input = "";
    sf::Text waterVaporText;
    makeText(waterVaporText, font, "WaterVapor Gaming", 60, height / 2, 100);
    sf::RectangleShape inputBox(sf::Vector2f(300, 40));
    inputBox.setPosition((height / 2) + 100, 200);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);

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
            if (event.type == sf::Event::TextEntered){
                if ((isalpha(event.text.unicode) || isdigit(event.text.unicode) || ispunct(event.text.unicode)
                     || isspace(event.text.unicode)) && input.getSize() < 25) {
                    input += event.text.unicode;
                    cursor.setString(input + "|");
                    cursor.setPosition((height / 2) + 100, 200);
                }
            }
            else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::BackSpace) {
                    input = input.substring(0, input.getSize() - 1);
                    cursor.setString(input + "|");
                }
                if(event.key.code == sf::Keyboard::Enter){
                    string searchParameter1 = input;
                    continue;
                }
            }
        }



        // order should be clear, draw display.
        window.clear(sf::Color::Blue);
        window.draw(waterVaporText);
        window.draw(inputBox);
        window.draw(cursor);
        window.display();
    }


    return 0;
}
