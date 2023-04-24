//
// Created by caleb on 4/20/2023.
//

#include <iostream>
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "ConsoleAppsNodes.cpp"
#include <stdexcept>
#include <chrono>
#include <fstream>
#include <sstream>

static void InitializeMapConsole(unordered_map<string, ConsoleNode*>& catalogue) {
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
static void InitializeMapApp(unordered_map<string, AppNode*>& catalogue) {
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
                if (catalogue.size() == 3938)
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

static void makeText(sf::Text& text, sf::Font& font, string s, int size, int width, int height){
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setString(s);
    text.setPosition(width, height);
}
static void makeInputBox(sf::RenderWindow& window, int sizeX, int sizeY, int width, int height){ // initial 300, 40 ; 525, 200
    sf::RectangleShape inputBox(sf::Vector2f(sizeX, sizeY));
    inputBox.setPosition(width, height);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    window.draw(inputBox);
}

static void makeGUI() {
    unordered_map < string, ConsoleNode * > ConsoleGames;
    unordered_map < string, AppNode * > AppGames;

    InitializeMapConsole(ConsoleGames);

    int width = 1500;
    int height = 850;


    sf::RenderWindow window(sf::VideoMode(width, height), "WaterVapor Gaming");
    sf::Font font;
    //font.loadFromFile("files/Anuphan-VariableFont_wght.ttf");
    font.loadFromFile("files/CourierPrime-Regular.ttf");
    sf::Text cursor;
    makeText(cursor, font, "|", 20, (height / 2) + 100, 200);
    sf::Text cursor2;
    makeText(cursor2, font, "|", 20, 0, 0);
    sf::String input = "";
    sf::Text waterVaporText;
    makeText(waterVaporText, font, "WaterVapor Gaming", 60, height / 2, 100);
    sf::Text initialQ;
    makeText(initialQ, font, "How many parameters would you like to search by?", 35, 325, 200);
    sf::Text sByTitle;
    makeText(sByTitle, font, "Enter Title here: ", 35, 50, 220);

    int numHeight = 240;
    vector <sf::Text> initialNumbers;
    for (int i = 1; i <= 4; i++) {
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
    sf::Text valueToSearchDisplay;
    string valueToSearch;

    sf::RectangleShape titleBox(sf::Vector2f(300, 60));
    titleBox.setPosition(50, 273);
    titleBox.setFillColor(sf::Color::White);
    titleBox.setOutlineThickness(2);
    titleBox.setOutlineColor(sf::Color::Black);
    bool isEditing = false;
    string titleSearch;
    sf::Sprite star;
    star = sf::Sprite(TextureManager::GetTexture("star"));
    star.setPosition(370, 276);
    star.setScale(0.25, 0.25);
    bool displayStar = false;

    sf::Sprite chief;
    chief = sf::Sprite(TextureManager::GetTexture("chief"));
    chief.setPosition((height / 2), height / 2 + 100);
    chief.setScale(0.5f, 0.5f);
    int maxNumBoxes = 0;
    int valBoxes = 1;
    int numBoxes = 1;
    bool allowTextInput = true;
    vector <string> searchParams;
    vector <string> givenVals;
    vector <sf::Text> searchTexts;
    vector <sf::Text> searchVals;
    bool parameterGiven = false;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);
                cout << coordinates.x << ", " << coordinates.y << endl;
                if (respawn.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                    //TODO: clear out text boxes, equivalent to start over
                    parametersQ = false;
                    searchTexts.clear();
                    vector <float> value = {
                            24.95,
                            2004
                    };
                    for (int j = 0; j < searchParams.size(); j++) {
                        auto iter = ConsoleGames.begin();
                        string param = searchParams.at(j);
                        transform(param.begin(), param.end(), param.begin(),
                                  [](unsigned char c) { return std::tolower(c); });
                        //TODO: Have input handling for all possible search parameters
                        if (param == "price" || param == "release") {
                            while (iter != ConsoleGames.end()) {
                                float gameTraitValue = any_cast<float>(consoleTraits[param](*iter->second));
                                float queueVal = abs(value.at(j) - gameTraitValue);
                                if (j == 0)
                                    q1.insert(to_string(queueVal), iter->second->Title);
                                else if (j == 1)
                                    q2.insert(to_string(queueVal), iter->second->Title);
                                else if (j == 2)
                                    q3.insert(to_string(queueVal), iter->second->Title);
                                else
                                    q4.insert(to_string(queueVal), iter->second->Title);
                                iter++;
                            }
//                            for (int i = 0; i < 100; i++) {
//                                string key = q.extractedVal();
//                                transform(key.begin(), key.end(), key.begin(),
//                                          [](unsigned char c) { return std::tolower(c); });
//                                cout << ConsoleGames[key]->Title << "\t\t" << ConsoleGames[key]->rating << "\n";
//                            }
                        }
                        cout << "Here is 100 games that fit your preferences of " << searchParams.at(j) << "\n";
                    }
//                    for (int i = 0; i < searchParams.size(); i++) {
//                        string key = searchParams.at(i);
//                        if (key.find('\r') == 0)
//                            key = key.substr(1);
//                        transform(key.begin(), key.end(), key.begin(),
//                                  [](unsigned char c) { return std::tolower(c); });
//                        try {
//                            auto finder = ConsoleGames.find(key);
//                            if (finder == ConsoleGames.end()) {
//                                throw invalid_argument(
//                                        "We're sorry, \"" + searchParams.at(i) + "\" is not in our library of games.");
//                            }
//                            cout << ConsoleGames[key]->price << endl;
//                        } catch (invalid_argument &e) {
//                            cerr << "WaterVapor Gaming has encountered an error, please standby." <<
//                                 endl << e.what() << endl;
//                        }
//                    }
                    givenVals.clear();
                    searchParams.clear();
                    numBoxes = 1;
                    valBoxes = 1;
                    input.clear();
                    cursor.setString("");
                    cursor.setPosition(525, 200);
                    cursor2.setString("");
                    cursor2.setPosition(0,0);
                    allowTextInput = true;
                    displayStar = false;
                }
                if (titleBox.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    isEditing = true;
                }
                if (coal.getGlobalBounds().contains(coordinates.x, coordinates.y)) { //option 1
                    maxNumBoxes = 1;
                    parametersQ = true;
                    isEditing = false;
                }
                if (iron.getGlobalBounds().contains(coordinates.x, coordinates.y)) { // option 2
                    maxNumBoxes = 2;
                    parametersQ = true;
                    isEditing = false;
                }
                if (gold.getGlobalBounds().contains(coordinates.x, coordinates.y)) { //option 3
                    maxNumBoxes = 3;
                    parametersQ = true;
                    isEditing = false;
                }
                if (diamond.getGlobalBounds().contains(coordinates.x, coordinates.y)) { //option 4
                    maxNumBoxes = 4;
                    parametersQ = true;
                    isEditing = false;
                }
            }
            if (parametersQ || isEditing) {
                if (event.type == sf::Event::TextEntered && allowTextInput && !isEditing) {
                    if ((isalpha(event.text.unicode) || isdigit(event.text.unicode) || ispunct(event.text.unicode)
                         || isspace(event.text.unicode)) && input.getSize() < 25) {
                        if (!parameterGiven) {
                            input += event.text.unicode;
                            cursor.setString(input + "|");
                            int increment = (numBoxes - 1) * 50;
                            int x = 200 + increment;
                            cursor.setPosition(525, x);
                        } else {
                            input += event.text.unicode;
                            cursor.setString(input + "|");
                            int increment = (numBoxes - 2) * 50;
                            int x = 200 + increment;
                            cursor.setPosition(845, x);
                        }
                    }
                    if (valBoxes > maxNumBoxes) {
                        allowTextInput = false;
                    }

                } if (isEditing && event.type == sf::Event::TextEntered) {
                    if ((isalpha(event.text.unicode) || isdigit(event.text.unicode) || ispunct(event.text.unicode)
                         || isspace(event.text.unicode)) && input.getSize() < 50) {// Check if ASCII and not backspace
                        input += event.text.unicode;
                        if (input.getSize() <= 25) {
                            cursor.setString(input + "|");
                            cursor.setPosition(50, 273);
                            if (input.getSize() == 25){
                                cursor.setString(input);
                            }
                        } else if (input.getSize() < 50){
                            cursor2.setString(input.substring(25) + "|");
                            cursor2.setPosition(50, 293);
                        }
                    }
                } if (event.type == sf::Event::KeyPressed) {
                    if (!isEditing && event.key.code == sf::Keyboard::BackSpace) {
                        input = input.substring(0, input.getSize() - 1);
                        cursor.setString(input + "|");
                    }
                    if (isEditing && event.key.code == sf::Keyboard::BackSpace){
                        input = input.substring(0, input.getSize() - 1);
                        if (input.getSize() >= 25) {
                            cursor2.setString(input.substring(25) + "|");
                            if (input.getSize() == 25){
                                cursor2.setString(input.substring(25));
                            }
                        }
                        if (input.getSize() < 25){
                            cursor.setString(input + "|");
                        }
                    }
                    if (!isEditing && event.key.code == sf::Keyboard::Enter && numBoxes <= maxNumBoxes + 1 && allowTextInput) {
                        if (!parameterGiven) {
                            searchParameter1 = input.toAnsiString();
                            searchParams.push_back(searchParameter1);
                            if (numBoxes > maxNumBoxes) {
                                allowTextInput = false;
                            }
                            numBoxes++;
                            parameterGiven = true;
                            int increment = (numBoxes - 2) * 50;
                            int x = 200 + increment;
                            makeText(searchParameterDisplay, font, searchParameter1, 30, 525, x);
                            //searchParameterDisplay.setStyle(sf::Text::Bold);
                            input.clear();
                            searchTexts.push_back(searchParameterDisplay);
                            if (!searchParameter1.empty()) {
                                transform(searchParameter1.begin(), searchParameter1.end(), searchParameter1.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << searchParameter1 << endl;
                            }
                        } else {
                            valueToSearch = input.toAnsiString();
                            givenVals.push_back(valueToSearch);
                            if (valBoxes > maxNumBoxes) {
                                allowTextInput = false;
                            }
                            valBoxes++;
                            parameterGiven = false;
                            input.clear();
                            int increment = (numBoxes - 2) * 50;
                            int x = 200 + increment;
                            makeText(valueToSearchDisplay, font, valueToSearch, 25, 845, x);
                            searchVals.push_back(valueToSearchDisplay);
                            if (!valueToSearch.empty()) {
                                transform(valueToSearch.begin(), valueToSearch.end(), valueToSearch.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << valueToSearch << endl;
                            }
                        }
                    }
                    if (isEditing && event.key.code == sf::Keyboard::Enter && allowTextInput) {
                        titleSearch = input.toAnsiString();
                        if (!titleSearch.empty()){
                            transform(titleSearch.begin(), titleSearch.end(), titleSearch.begin(),
                                      [](unsigned char c) { return std::tolower(c); });
                            displayStar = true;
                            isEditing = false;
                        }
                        input.clear();
                        cursor.setString("");
                        cursor2.setString("");
                    }
                }
            }
        }
        // order should be clear, draw display.
        window.clear(sf::Color::Blue);
        window.draw(waterVaporText);

        //search by title
        window.draw(sByTitle);
        window.draw(titleBox);
        if(isEditing && !parametersQ){
            window.draw(cursor);
            window.draw(cursor2);
        }
        if (displayStar){
            window.draw(star);
        }

        int x = 525;
        int y = 200;
        if (parametersQ) {
           // if (parameterGiven){
                for (int i = 1; i <= valBoxes; i++) {
                    makeInputBox(window, 200, 40, 845, y); // search boxes pop up
                    if (valBoxes <= maxNumBoxes) {
                        window.draw(cursor); //cursor tracts
                    }
                    y += 50;
                    if (i == maxNumBoxes) {
                        break;
                    }
                }
                for (auto &iter: searchVals) { // search text display
                    window.draw(iter);
                }
            //} else {
                y = 200;
                for (int i = 1; i <= numBoxes; i++) {
                    makeInputBox(window, 300, 40, 525, y); // search boxes pop up
                    if (numBoxes <= maxNumBoxes) {
                        window.draw(cursor); //cursor tracts
                    }
                    y += 50;
                    if (i == maxNumBoxes) {
                        break;
                    }
                }
                for (auto &iter: searchTexts) { // search text display
                    window.draw(iter);
                }
            //}
        } else {
            searchTexts.clear();
            window.draw(initialQ);
            for (const auto &number: initialNumbers) {
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
}