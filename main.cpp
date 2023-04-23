#include <iostream>
#include "HashTable.h"
#include "HashTableApp.h"
#include "PriorityQ.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "ConsoleAppsNodes.cpp"
#include <chrono>
#include <stdexcept>

using namespace std;

unordered_map<string, sf::Texture> TextureManager::textures;

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
                insertAppData(nodeProperties, catalogue);
                if (catalogue.size() == 7964)
                    continue;
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
void makeInputBox(sf::RenderWindow& window, int x, int y){ // initial 525, 200
    sf::RectangleShape inputBox(sf::Vector2f(300, 40));
    inputBox.setPosition(x, y);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    window.draw(inputBox);
}

int main() {
    unordered_map<string, ConsoleNode*> ConsoleGames;
    unordered_map<string, AppNode*> AppGames;

//    map.insert("Caleb");
//    map.insert("Jayden");
//    map.insert("Maxwell");
//    map.search("Caleb");
//    map.search("aCelb");

    PriorityQ q;
    InitializeMapConsole(ConsoleGames);
    //InitializeMapApp(AppGames);

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
    //font.loadFromFile("files/Anuphan-VariableFont_wght.ttf");
    font.loadFromFile("files/CourierPrime-Regular.ttf");
    sf::Text cursor;
    makeText(cursor, font, "|", 20, (height / 2) + 100, 200);
    sf::String input = "";
    sf::Text waterVaporText;
    makeText(waterVaporText, font, "WaterVapor Gaming", 60, height / 2, 100);
    sf::Text initialQ;
    makeText(initialQ, font, "How many parameters would you like to search by?", 35, 325, 200);
    int numHeight = 240;
    vector<sf::Text> initialNumbers;
    for (int i = 1; i <= 4; i++){
        sf::Text number;
        makeText(number, font, to_string(i), 32, 525, numHeight);
        numHeight += 60;
        initialNumbers.push_back(number);
    }
    sf::Sprite coal = sf::Sprite(TextureManager::GetTexture("coal"));
    coal.setPosition(560, 240);
    coal.setScale(0.25f, 0.25f);
    sf::Sprite iron = sf::Sprite(TextureManager::GetTexture("iron"));
    iron.setPosition(560, 300);
    iron.setScale(0.20f, 0.20f);
    sf::Sprite gold = sf::Sprite(TextureManager::GetTexture("gold"));
    gold.setPosition(560, 360);
    gold.setScale(0.20f, 0.20f);
    sf::Sprite diamond = sf::Sprite(TextureManager::GetTexture("diamond"));
    diamond.setPosition(560, 420);
    diamond.setScale(0.20f, 0.20f);
    sf::Sprite respawn = sf::Sprite(TextureManager::GetTexture("respawn"));
    respawn.setScale(0.4f, 0.5f);
    bool parametersQ = false;

    sf::Text searchParameterDisplay;
    string searchParameter1;


    sf::Sprite chief;
    chief = sf::Sprite(TextureManager::GetTexture("chief"));
    chief.setPosition((height / 2), height / 2 + 100);
    chief.setScale(0.5f, 0.5f);
    bool entered = false;
    int maxNumBoxes = 0;
    int numBoxes = 1;
    bool allowTextInput = true;
    vector<string> searchParams;
    vector<sf::Text> searchTexts;

    while (window.isOpen()){
        sf::Event event{};
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);
                cout << coordinates.x << ", " << coordinates.y << endl;
                if (respawn.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    //TODO: clear out text boxes, equivalent to start over
                    parametersQ = false;
                    searchTexts.clear();
                    for (int i = 0; i < searchParams.size(); i++) {
                        cout << searchParams.at(i) << endl;
                        string key = searchParams.at(i);
                        if (key.find('\r') == 0)
                            key = key.substr(1);
                        transform(key.begin(),key.end(), key.begin(),
                                  [](unsigned char c) { return std::tolower(c); });
                        try {
                            auto finder = ConsoleGames.find(key);
                            if (finder == ConsoleGames.end()) {
                                throw invalid_argument("We're sorry, \"" + searchParams.at(i) + "\" is not in our library of games.");
                            }
                            cout << ConsoleGames[key]->price << endl;
                        } catch (invalid_argument &e) {
                            cerr << "WaterVapor Gaming has encountered an error, please standby." <<
                            endl << e.what() << endl;
                        }
                    }
                    searchParams.clear();
                    numBoxes = 1;
                    input.clear();
                    cursor.setString("|");
                    cursor.setPosition(525, 200);
                    allowTextInput = true;
                }
                if (coal.getGlobalBounds().contains(coordinates.x, coordinates.y)){ //option 1
                    maxNumBoxes = 1;
                    parametersQ = true;
                }
                if (iron.getGlobalBounds().contains(coordinates.x, coordinates.y)){ // option 2
                    maxNumBoxes = 2;
                    parametersQ = true;
                }
                if (gold.getGlobalBounds().contains(coordinates.x, coordinates.y)){ //option 3
                    maxNumBoxes = 3;
                    parametersQ = true;
                }
                if (diamond.getGlobalBounds().contains(coordinates.x, coordinates.y)){ //option 4
                    maxNumBoxes = 4;
                    parametersQ = true;
                }
            }
            if (parametersQ) {
                if (event.type == sf::Event::TextEntered && !entered && allowTextInput) {
                    if ((isalpha(event.text.unicode) || isdigit(event.text.unicode) || ispunct(event.text.unicode)
                         || isspace(event.text.unicode)) && input.getSize() < 25) {
                        input += event.text.unicode;
                        cursor.setString(input + "|");
                        int increment = (numBoxes - 1) * 50;
                        int x = 200 + increment;
                        cursor.setPosition(525, x);
                    }

                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::BackSpace) {
                        input = input.substring(0, input.getSize() - 1);
                        cursor.setString(input + "|");
                    }
                    if (event.key.code == sf::Keyboard::Enter && numBoxes <= maxNumBoxes && allowTextInput) {
                        searchParameter1 = input.toAnsiString();
                        searchParams.push_back(input);
                        numBoxes++;
                        if (numBoxes > maxNumBoxes)
                            allowTextInput = false;
                        int increment = (numBoxes - 2) * 50;
                        int x = 200 + increment;
                        searchParameterDisplay.setPosition(525, x);
                        makeText(searchParameterDisplay, font, searchParameter1, 30, 525, x);
                        //searchParameterDisplay.setStyle(sf::Text::Bold);
                        input.clear();
                        searchTexts.push_back(searchParameterDisplay);
                        if (!searchParameter1.empty()) {
                            transform(searchParameter1.begin(), searchParameter1.end(), searchParameter1.begin(),
                                      [](unsigned char c) { return std::tolower(c); });
                            cout << searchParameter1 << endl;

                        }
                    }
                }
            }
        }
        // order should be clear, draw display.
        window.clear(sf::Color::Blue);
        window.draw(waterVaporText);
        int x = 200;
        if(parametersQ) {
            for (int i = 1; i <= numBoxes; i++) {
                makeInputBox(window, 525, x); // search boxes pop up
                if (numBoxes <= maxNumBoxes) {
                    window.draw(cursor); //cursor tracts
                }
                x += 50;
                if (i == maxNumBoxes) {
                    break;
                }
            }
            for (auto &iter: searchTexts) { // search text display
                window.draw(iter);
            }
        } else {
            searchTexts.clear();
            window.draw(initialQ);
            for (const auto& number : initialNumbers){
                window.draw(number);
            }
            window.draw(coal);
            window.draw(gold);
            window.draw(iron);
            window.draw(diamond);
        }
        window.draw(chief);
        window.draw(respawn);

        window.display();
    }
    return 0;
}
