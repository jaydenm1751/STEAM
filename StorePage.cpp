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
#include "PriorityQ.h"
#include <functional>
#include <any>

static void InitializeMapConsole(unordered_map<string, ConsoleNode*>& catalogue) {

    string path = "files/ConsoleStoreGames.csv";
    ifstream storeFile(path);

    while (true) {
        if (!storeFile.is_open()) {
            cerr << "Error opening the file:" << path << endl;
        } else {
            string headerLine;
            getline(storeFile, headerLine);

            string line;
            while (getline(storeFile, line)) {
                istringstream line_stream(line);
                string cell;
                vector<string> nodeProperties;

                while (getline(line_stream, cell, ',')) {
                    if (cell.empty()) {
                        nodeProperties.emplace_back(("N/A"));
                    } else {
                        nodeProperties.push_back(cell);
                    }
                }
                insertConsoleData(nodeProperties, catalogue);
            }
            storeFile.close();
            break;
        }
    }
}

//TODO: Implement the App hash table
static void InitializeMapApp(unordered_map<string, AppNode*>& catalogue) {

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
                        nodeProperties.emplace_back(("N/A"));
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
}

static void makeText(sf::Text& text, sf::Font& font, const string& s, int size, int width, int height){
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
    unordered_map <string, ConsoleNode*> ConsoleGames;
    unordered_map <string, AppNode*> AppGames;
    unordered_map <string, function<any(const ConsoleNode&)>> consoleTraits = {
            {"title", [](const ConsoleNode& node) -> string { return node.Title; }},
            {"players", [](const ConsoleNode& node) -> bool { return node.players > 1; }},
            {"online", [](const ConsoleNode& node) -> bool { return node.online; }},
            {"genre", [](const ConsoleNode& node) -> vector<string> { return node.genres; }},
            {"price", [](const ConsoleNode& node) -> float { return node.price; }},
            {"console", [](const ConsoleNode& node) -> vector<string> { return node.console; }},
            {"rating", [](const ConsoleNode& node) -> string { return node.rating; }},
            {"release", [](const ConsoleNode& node) -> float { return node.release; }},
    };
    unordered_map<std::string, std::function<std::any(const AppNode&)>> appTraits = {
            {"title", [](const AppNode& node) -> string { return node.Title; }},
            {"rating", [](const AppNode& node) -> float { return node.rating; }},
            {"ratingCount", [](const AppNode& node) -> int { return node.ratingCount; }},
            {"price", [](const AppNode& node) -> float { return node.price; }},
            {"inAppPurchases", [](const AppNode& node) -> vector<float> { return node.inAppPurchases; }},
            {"developer", [](const AppNode& node) -> string { return node.developer; }},
            {"age", [](const AppNode& node) -> string { return node.age; }},
            {"languages", [](const AppNode& node) -> vector<string> { return node.languages; }},
            {"size", [](const AppNode& node) -> long long unsigned { return node.size; }},
            {"genres", [](const AppNode& node) -> vector<string> { return node.genres; }},
            {"releaseDate", [](const AppNode& node) -> string { return node.releaseDate; }},
            {"updateData", [](const AppNode& node) -> string { return node.updateData; }},
    };

    auto start_time = chrono::steady_clock::now();
    InitializeMapApp(AppGames);
    InitializeMapConsole(ConsoleGames);
    auto end_time = chrono::steady_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Elapsed time: " << elapsed_time.count() << " ms.\n";

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
    makeText(sByTitle, font, "Enter Title here: ", 35, 0, 0);

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
    respawn.setPosition(width - 270, 0);
    bool parametersQ = false;

    sf::Sprite console = sf::Sprite(TextureManager::GetTexture("console"));
    console.setScale(0.5f, 0.5f);
    console.setPosition(400, 310);
    sf::Sprite nokia = sf::Sprite(TextureManager::GetTexture("phone"));
    nokia.setScale(0.5f, 0.5f);
    nokia.setPosition(850, 310);
    bool typeConsole = false;
    bool typeChosen = false;
    sf::Text initialQ2;
    makeText(initialQ2, font, "What type of game are you searching?", 35, 325, 200);

    sf::Text searchParameterDisplay;
    string searchParameter1;
    sf::Text valueToSearchDisplay;
    string valueToSearch;

    sf::View view(sf::FloatRect(0,0,width,height));

    sf::RectangleShape titleBox(sf::Vector2f(300, 60));
    titleBox.setPosition(10, 53);
    titleBox.setFillColor(sf::Color::White);
    titleBox.setOutlineThickness(2);
    titleBox.setOutlineColor(sf::Color::Black);
    bool isEditing = false;
    sf::Text titleSearchDisplay;
    sf::Text titleSearchDisplay2;
    string titleSearch;
    string titleSearch2;
    sf::Sprite star;
    star = sf::Sprite(TextureManager::GetTexture("star"));
    star.setPosition(323, 53);
    star.setScale(0.25, 0.25);
    bool displayStar = false;
    bool selecting = true;

    sf::Sprite pacman;
    pacman = sf::Sprite(TextureManager::GetTexture("pacman"));
    pacman.setPosition(650, 0);
    pacman.setScale(0.45f, 0.45f);
    sf::Sprite shield;
    shield = sf::Sprite(TextureManager::GetTexture("shield"));
    shield.setPosition(550, 0);
    shield.setScale(0.45f, 0.45f);
    sf::Sprite pokeball;
    pokeball = sf::Sprite(TextureManager::GetTexture("pokeball"));
    pokeball.setPosition(750, 0);
    pokeball.setScale(0.45f, 0.45f);
    bool pokeballClicked = false;
    bool shieldClicked = false;
    bool pacmanClicked = false;

    sf::Sprite bowser;
    bowser = sf::Sprite(TextureManager::GetTexture("bowser"));
    bowser.setPosition(50, 75);
    bowser.setScale(3.5f, 4.0f);

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

    const auto topPage = (float) height;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Check for mouse wheel movement
            if (event.type == sf::Event::MouseWheelMoved) {
                // Check the movement direction
                if (event.mouseWheel.delta > 0) {
                    sf::Vector2f viewPos = view.getCenter();
                    view.move(0, -50);
                    if (view.getCenter().y - view.getSize().y / 2 < 0){
                        view.setCenter(viewPos.x, view.getSize().y / 2);
                    }
                }
                else if (event.mouseWheel.delta < 0) {
                    view.move(0, 50);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);
                cout << coordinates.x << ", " << coordinates.y << endl;
                if (respawn.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                    //TODO: clear out text boxes, equivalent to start over
                    parametersQ = false;
                    searchTexts.clear();
                    searchVals.clear();
                    PriorityQ q1;
                    PriorityQ q2;
                    PriorityQ q3;
                    PriorityQ q4;
                    for (int j = 0; j < searchParams.size(); j++) {
                        int q1Size = 0;
                        auto iter = ConsoleGames.begin();
                        string param = searchParams.at(j);
                        string val = givenVals.at(j);
                        if(param.find('\r') == 0)
                            param = param.substr(1);
                        if(param.find(' ') == param.size() - 1)
                            param = param.substr(0,param.size() - 1);
                        if(val.find('\r') == 0)
                            val = val.substr(1);
                        transform(param.begin(), param.end(), param.begin(),
                                  [](unsigned char c) { return std::tolower(c); });
                        //TODO: Have input handling for all possible search parameters
                        if (param == "price" || param == "release") {
                            while (iter != ConsoleGames.end()) {
                                float gameTraitValue = any_cast<float>(consoleTraits[param](*iter->second));
                                float queueVal = abs(stof(val) - gameTraitValue);
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
                        }
                        if (param == "rating" || param == "genre" || param == "console") {
                            int base = 0;
                            for (char k : val) {
                                base += k;
                            }
                            if (param != "rating") {

                            } else {
                                while (iter != ConsoleGames.end()) {
                                    int comp = 0;
                                    for (char k : iter->second->rating) {
                                        comp += k;
                                    }
                                    int queueVal = abs(base - comp);
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
                            }
                        }
                        if (j == 0) {
                            q1Size = q1.getSize();
                            for (int i = 0; i < q1Size; i++) {
                                string key = q1.extractedVal();
                                transform(key.begin(), key.end(), key.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << ConsoleGames[key]->Title << "\t\t" << ConsoleGames[key]->rating << "\n";
                            }
                        } else if (j == 1) {
                            for (int i = 0; i < q2.getSize(); i++) {
                                string key = q2.extractedVal();
                                transform(key.begin(), key.end(), key.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << ConsoleGames[key]->Title << "\t\t" << ConsoleGames[key]->price << "\n";
                            }
                        } else if (j == 2) {
                            for (int i = 0; i < q3.getSize(); i++) {
                                string key = q3.extractedVal();
                                transform(key.begin(), key.end(), key.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << ConsoleGames[key]->Title << "\t\t" << ConsoleGames[key]->release << "\n";
                            }
                        } else {
                            for (int i = 0; i < q3.getSize(); i++) {
                                string key = q3.extractedVal();
                                transform(key.begin(), key.end(), key.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << ConsoleGames[key]->Title << "\t\t" << ConsoleGames[key]->rating << "\n";
                            }
                        }
                        cout << "Here is " << q1Size << " games that fit your preferences of " << param << "\n";
                    }
                    givenVals.clear();
                    searchParams.clear();
                    numBoxes = 1;
                    valBoxes = 1;
                    input.clear();
                    cursor.setString("");
                    cursor.setPosition(0, 0);
                    cursor2.setString("");
                    cursor2.setPosition(0,0);
                    allowTextInput = true;
                    displayStar = false;
                    selecting = true;
                    typeChosen = false;
                    typeConsole = false;
                    pacmanClicked = false;
                    shieldClicked = false;
                    pokeballClicked = false;
                }
                //TODO: implement button functionality
                if (pacman.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    pacmanClicked = true;
                }
                if (pokeball.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    pokeballClicked = true;
                }
                if (shield.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    shieldClicked = true;
                }
                if (console.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    typeConsole = true;
                    typeChosen = true;
                }
                if (nokia.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    typeChosen = true;
                    typeConsole = false;
                }
                if (displayStar && (titleBox.getGlobalBounds().contains(coordinates.x, coordinates.y) ||
                    star.getGlobalBounds().contains(coordinates.x, coordinates.y))){
                    displayStar = false;
                }
                if (titleBox.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    isEditing = true;
                    cursor.setString("");
                    cursor2.setString("");
                }
                if (coal.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { //option 1
                    maxNumBoxes = 1;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
                }
                if (iron.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { // option 2
                    maxNumBoxes = 2;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
                }
                if (gold.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { //option 3
                    maxNumBoxes = 3;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
                }
                if (diamond.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { //option 4
                    maxNumBoxes = 4;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
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
                            cursor.setPosition(10, 53);
                            if (input.getSize() == 25){
                                cursor.setString(input);
                            }
                        } else if (input.getSize() < 50){
                            cursor2.setString(input.substring(25) + "|");
                            cursor2.setPosition(10, 73);
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
                        titleSearch = input.substring(0, 20).toAnsiString();
                        makeText(titleSearchDisplay, font, titleSearch, 23, 10, 53);
                        if (input.toAnsiString().length() > 20) {
                            titleSearch2 = input.substring(20).toAnsiString();
                            makeText(titleSearchDisplay2, font, titleSearch2, 23, 10, 83);
                        }
                        input.clear();
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
        window.setView(view);
        window.draw(waterVaporText);
        window.draw(pacman);
        window.draw(shield);
        window.draw(pokeball);

        //search by title
        window.draw(sByTitle);
        window.draw(titleBox);
        if(isEditing && !parametersQ && !pacmanClicked && !shieldClicked && !pokeballClicked){
            window.draw(cursor);
            window.draw(cursor2);
        }
        if (displayStar){
            window.draw(titleSearchDisplay);
            window.draw(titleSearchDisplay2);
            window.draw(star);
        }

        if (typeChosen && !pacmanClicked && !shieldClicked && !pokeballClicked) {
            if (parametersQ) {
                int y = 200;
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
        } else if (!pacmanClicked && !shieldClicked && !pokeballClicked){
            window.draw(initialQ2);
            window.draw(nokia);
            window.draw(console);
        }
        window.draw(chief);
        window.draw(respawn);

        //new window display
        if (pacmanClicked || pokeballClicked || shieldClicked){
            window.draw(bowser);
        }

        window.display();
    }
}