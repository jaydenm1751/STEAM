#include <iostream>
#include "HashTable.h"
#include "PriorityQ.h"
#include <SFML/Graphics.hpp>
#include <chrono>

using namespace std;
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

int main() {
    HashTable map;

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

    sf::Font font;
    font.loadFromFile("files/Anuphan-VariableFont_wght.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    string input;
    sf::Text cursor;
    cursor.setFont(font);
    cursor.setCharacterSize(20);
    cursor.setFillColor(sf::Color::Black);
    sf::Clock backspaceCLK;

//    bool backspaceHeld = false;

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
                if (event.text.unicode < 128){
                    input += static_cast<char>(event.text.unicode);
                    cursor.setString(input + "|");
                    text.setString(input);
                }
            }
            // TODO: fix back space key
            else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (backspaceCLK.getElapsedTime().asMilliseconds() > 200 && !input.empty()) {
                        input.pop_back();
                        cursor.setString(input +"|");
                        sf::FloatRect  textRect = cursor.getLocalBounds();
                        cursor.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    }
                }
            }
            if (event.type == sf::Event::KeyReleased){
                if (event.key.code == sf::Keyboard::BackSpace){
//                    backspaceHeld = false;
                }
            }
        }



        // order should be clear, draw display.
        window.clear(sf::Color::Blue);
        window.draw(text);
        window.display();
    }


    return 0;
}
