//
// Created by caleb on 4/20/2023.
//

#include <iostream>
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <stdexcept>
#include <chrono>
#include <fstream>
#include "ConsoleAppsNodes.h"
#include "searchRanking.h"

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

    auto start_time = chrono::steady_clock::now();
    InitializeMapApp(AppGames);
    InitializeMapConsole(ConsoleGames);
    auto end_time = chrono::steady_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Elapsed time: " << elapsed_time.count() << " ms.\n";
    sf::Font font;
    //text that always stays on screen
    font.loadFromFile("files/CourierPrime-Regular.ttf");

    //where the front end and back end meet
    vector<sf::Text> titleSearchTraits; //be all sf::Text obj to be displayed.
    vector<string> traits = {
            "Title", "Game Type", "Player Count", "Connectivity", "Genre", "Platform", "Price", "Age Rating", "Publisher", "Release Year"
    };
    vector<string> traits1 = {
            "Title", "Price", "In-App Purchases", "Genre", "Review", "Age Rating", "Developer", "Recent Update", "Size", "Languages", "URL"
    };
    unordered_map<string, function<any(const AppNode&)>> appTraits = {
            {"Title", [](const AppNode& node) -> string { return node.Title; }},
            {"Review", [](const AppNode& node) -> string { return to_string(node.rating); }},
            {"Price", [](const AppNode& node) -> string { return to_string(node.price); }},
            {"In-App Purchases", [](const AppNode& node) -> vector<string> { return node.inAppPurchases; }},
            {"Age Rating", [](const AppNode& node) -> string { return node.age; }},
            {"Developer", [](const AppNode& node) -> string { return node.developer; }},
            {"Size", [](const AppNode& node) -> string { return node.size; }},
            {"Genre", [](const AppNode& node) -> vector<string> { return node.genres; }},
            {"Recent Update", [](const AppNode& node) -> string { return node.updateData; }},
            {"Languages", [](const AppNode& node) -> vector<string> { return node.languages; }},
            {"URL", [](const AppNode& node) -> string { return node.url; }}
    };
    //epic
    unordered_map <string, function<any(const ConsoleNode&)>> consoleTraits = {
            {"Title", [](const ConsoleNode& node) -> string { return node.Title; }},
            {"Player Count", [](const ConsoleNode& node) -> string { return to_string(node.players); }},
            {"Game Type", [](const ConsoleNode& node) -> string { return (node.players > 1) ? "Co-op" : "Single-Player"; }},
            {"Connectivity", [](const ConsoleNode& node) -> string { return (node.online == 1) ? "Online" : "Local"; }},
            {"Review", [](const ConsoleNode& node) -> string { return to_string(node.review); }},
            {"Publisher", [](const ConsoleNode& node) -> vector<string> { return node.publishers ; }},
            {"Genre", [](const ConsoleNode& node) -> vector<string> { return node.genres; }},
            {"Price", [](const ConsoleNode& node) -> string { return to_string(node.price); }},
            {"Platform", [](const ConsoleNode& node) -> vector<string> { return node.console; }},
            {"Age Rating", [](const ConsoleNode& node) -> string { return node.rating; }},
            {"Release Year", [](const ConsoleNode& node) -> string { return to_string(node.release).substr(0, 4); }},
    };
    //window dimensions
    int width = 1500;
    int height = 850;

    sf::RenderWindow window(sf::VideoMode(width, height), "WaterVapor Gaming");
    sf::Text cursor;
    makeText(cursor, font, "|", 20, 475, 250);
    sf::Text cursor2;
    makeText(cursor2, font, "|", 20, 0, 0);
    sf::String input = "";
    sf::Text waterVaporText;
    makeText(waterVaporText, font, "WaterVapor Gaming", 60, height / 2, 125);
    sf::Text initialQ;
    makeText(initialQ, font, "How many parameters would you like to search by?", 35, 325, 200); // paramQ
    //top left text box
    sf::Text sByTitle;
    makeText(sByTitle, font, "Enter Title here: ", 35, 0, 0);
    sf::Text parameterType;
    makeText(parameterType, font, "Parameter Type", 25, 525, 210);
    sf::Text valEntered;
    makeText(valEntered, font, "Value", 25, 855, 210);
    // the info on parameter searches
    sf::Sprite validParametersConsole = sf::Sprite(TextureManager::GetTexture("validConsoleParameters"));
    validParametersConsole.setPosition(40, 550);
    sf::Sprite validParametersIOS = sf::Sprite(TextureManager::GetTexture("validMobileParameters"));
    validParametersIOS.setPosition(100, 550);
    sf::Sprite moreInfoConsole = sf::Sprite(TextureManager::GetTexture("consoleMoreInfo"));
    moreInfoConsole.setPosition(600, 550);
    sf::Sprite moreInfoIOS = sf::Sprite(TextureManager::GetTexture("mobileMoreInfo"));
    moreInfoIOS.setPosition(700, 533);
    bool moreInfoClicked = false;

    // parameter numbers
    int numHeight = 240;
    vector <sf::Text> initialNumbers;
    for (int i = 1; i <= 4; i++) {
        sf::Text number;
        makeText(number, font, to_string(i), 32, 525, numHeight);
        numHeight += 60;
        initialNumbers.push_back(number);
    }
    //icons for nums
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
    //home page button
    sf::Sprite respawn = sf::Sprite(TextureManager::GetTexture("respawn"));
    respawn.setScale(0.5f, 0.5f);
    respawn.setPosition(width - 310, 0);
    bool parametersQ = false;

    // home page buttons show what to search
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

    //numbers next to the input boxes
    sf::Sprite priority;
    vector<sf::Sprite> priorities;
    int nY = 250;
    for(int i = 1; i <= 4; i++){
        priority = sf::Sprite(TextureManager::GetTexture(to_string(i)));
        priority.setPosition(350, nY);
        priority.setScale(0.2f, 0.2f);
        priorities.push_back(priority);
        nY += 50;
    }
    sf::Text searchParameterDisplay;
    string searchParameter1;
    sf::Text valueToSearchDisplay;
    string valueToSearch;

    // scroll wheel
    sf::View view(sf::FloatRect(0,0,width,height));

    //input to title search box
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
    sf::Text errorTitle;
    makeText(errorTitle, font, "This game is not in our Library.", 18, 0, 113);
    errorTitle.setStyle(sf::Text::Bold);

    sf::Sprite searchButton = sf::Sprite(TextureManager::GetTexture("search"));
    searchButton.setScale(0.25, 0.25);
    bool searchButtonPressed = false;

    sf::Sprite star;
    star = sf::Sprite(TextureManager::GetTexture("star"));
    star.setPosition(323, 53);
    star.setScale(0.25, 0.25);
    sf::Sprite goomba = sf::Sprite(TextureManager::GetTexture("goomba"));
    goomba.setPosition(323, 53);
    goomba.setScale(0.25f, 0.23f);
    bool displayStar = false;
    bool displayGoomba = false;
    bool selecting = true;

    //TODO: implement the different store page tabs
    sf::Text browseConsole;
    makeText(browseConsole, font, "Browse All Console Games", 15, 655, 100);
    sf::Sprite pacman;
    pacman = sf::Sprite(TextureManager::GetTexture("pacman"));
    pacman.setPosition(725, 0);
    pacman.setScale(0.45f, 0.45f);
    sf::Text browseIOS;
    makeText(browseIOS, font, "Browse All Mobile Games", 15, 400, 100);
    sf::Sprite shield;
    shield = sf::Sprite(TextureManager::GetTexture("shield"));
    shield.setPosition(450, 0);
    shield.setScale(0.45f, 0.45f);
    sf::Text aboutUS;
    makeText(aboutUS, font, "About the Developers", 15, 950, 100);
    sf::Sprite pokeball;
    pokeball = sf::Sprite(TextureManager::GetTexture("pokeball"));
    pokeball.setPosition(1000, 0);
    pokeball.setScale(0.45f, 0.45f);
    bool pokeballClicked = false;
    bool shieldClicked = false;
    bool pacmanClicked = false;

    //BOWSER
    sf::Sprite bowser;
    bowser = sf::Sprite(TextureManager::GetTexture("bowser"));
    bowser.setPosition(50, 75);
    bowser.setScale(2.0f, 2.0f);

    sf::Text backButton;
    makeText(backButton, font, "Back To Parameters", 20, width - 240, 80);
    sf::Sprite crown;
    crown = sf::Sprite(TextureManager::GetTexture("crown"));
    crown.setPosition(width - 180, 110);
    crown.setScale(0.5f, 0.5f);
    bool crownClicked = false;
    bool respawnClicked = false;

    //figure out where to put the chief
//    sf::Sprite chief;
//    chief = sf::Sprite(TextureManager::GetTexture("chief"));
//    chief.setPosition((height / 2), height / 2 + 100);
//    chief.setScale(0.5f, 0.5f);
    int maxNumBoxes = 0;
    int valBoxes = 1;
    int numBoxes = 1;
    bool allowTextInput = true;
    vector <string> searchParams;
    vector <string> givenVals;
    vector <sf::Text> searchTexts;
    vector <sf::Text> searchVals;
    vector <sf::Text> searchOfGames;
    bool parameterGiven = false;

    vector<sf::Text> allGames;

    // Store the initial view windows position to come back to later
    sf::Vector2f initialViewCenter = view.getCenter();

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
                    view.move(0, -100);
                    respawn.move(0, -100);
                    if (view.getCenter().y - view.getSize().y / 2 < 0){
                        view.setCenter(viewPos.x, view.getSize().y / 2);
                        respawn.setPosition(width - 310, 0);
                    }
                }
                else if (event.mouseWheel.delta < 0) {
                    view.move(0, 100);
                    respawn.move(0, 100);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                // Get the mouse position in the window's coordinates space
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);

                // Convert the mouse position to the view's coordinates space
                sf::Vector2f mousePosView = window.mapPixelToCoords(coordinates, view);

                //cout << coordinates.x << ", " << coordinates.y << endl;
                if (respawn.getGlobalBounds().contains(mousePosView)) {
                    //TODO: clear out text boxes, equivalent to start over
                    view.setCenter(initialViewCenter);
                    respawn.setPosition(width - 310, 0);
                    parametersQ = false;
                    searchTexts.clear();
                    searchVals.clear();
                    givenVals.clear();
                    searchParams.clear();
                    numBoxes = 1;
                    valBoxes = 1;
                    input.clear();
                    cursor.setString("");
                    cursor.setPosition(0, 0);
                    cursor2.setString("");
                    cursor2.setPosition(0,0);
                    allGames.clear();
                    allowTextInput = true;
                    displayStar = false;
                    displayGoomba = false;
                    selecting = true;
                    typeChosen = false;
                    typeConsole = false;
                    pacmanClicked = false;
                    shieldClicked = false;
                    pokeballClicked = false;
                    respawnClicked = true;
                    moreInfoClicked = false;
                    titleSearchTraits.clear();
                    searchButtonPressed = false;
                    searchOfGames.clear();
                }
                if (typeChosen && (validParametersConsole.getGlobalBounds().contains(coordinates.x, coordinates.y)
                                   || validParametersIOS.getGlobalBounds().contains(coordinates.x, coordinates.y))){
                    moreInfoClicked = true;
                }
                //TODO: Display all console games
                if (pacman.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    pacmanClicked = true;
                    parametersQ = false;
                    allGames.clear();
                    auto iter = ConsoleGames.begin();
                    int yPos = 250;
                    int count = 1;
                    while (iter != ConsoleGames.end()) {
                        sf::Text game;
                        int point = to_string(iter->second->price).find('.');
                        string price = to_string(iter->second->price).substr(0, point+3);
                        string review = to_string(iter->second->review).substr(0,2);

                        string textLine = to_string(count) + ".)\tTitle: " + iter->second->Title + "\tPrice: $" + price + "\tPlatform(s): ";
                        for (unsigned int i = 0; i < iter->second->console.size(); i++) {
                            if (i == iter->second->console.size() - 1) {
                                textLine += iter->second->console.at(i);
                            } else {
                                textLine += iter->second->console.at(i) + ", ";
                            }
                        }

                        textLine += "\tScore: " + review;

                        makeText(game, font, textLine, 20, 75, yPos);
                        allGames.push_back(game);
                        iter++;
                        count++;
                        yPos += 30;
                    }
                }
                //TODO: About developers (if time)
                if (pokeball.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    pokeballClicked = true;
                }
                //TODO: Display all 17k app games
                if (shield.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    shieldClicked = true;
                    parametersQ = false;
                    allGames.clear();
                    auto iter = AppGames.begin();
                    int yPos = 250;
                    int count = 1;
                    while (iter != AppGames.end()) {
                        sf::Text game;
                        int point = iter->second->size.find('.');
                        string size = iter->second->size.substr(0,point+3);
                        point = to_string(iter->second->price).find('.');
                        string price = to_string(iter->second->price).substr(0, point+3);
                        string rating = to_string(iter->second->rating).substr(0, 3);

//                        point = price.find('.');
//                        price = price.substr(0,point+3);
                        string textLine = to_string(count) + ".) Title: " + iter->second->Title + "\tPrice: $" + price + "\tScore: " + rating;
                        makeText(game, font, textLine, 20, 75, yPos);
                        allGames.push_back(game);
                        textLine = "      Developer(s): " + iter->second->developer + "\tSize: " + size + "MB\n";
                        makeText(game, font, textLine, 20, 75, yPos + 20);
                        allGames.push_back(game);
                        iter++;
                        count++;
                        yPos += 50;
                    }
                }
                if (crown.getGlobalBounds().contains(coordinates.x, coordinates.y) && typeChosen && parametersQ){
                    crownClicked = true;
                    typeChosen = true;
                    moreInfoClicked = false;
                    parametersQ = false;
                    selecting = true;
                    searchTexts.clear();
                    searchVals.clear();
                    givenVals.clear();
                    searchParams.clear();
                    titleSearchTraits.clear();
                    numBoxes = 1;
                    valBoxes = 1;
                    input.clear();
                    cursor.setString("");
                    cursor.setPosition(0, 0);
                    cursor2.setString("");
                    cursor2.setPosition(0,0);
                    allowTextInput = true;
                    displayStar = false;
                    displayGoomba = false;
                    searchButtonPressed = false;
                    searchOfGames.clear();
                }
                if (!displayStar && console.getGlobalBounds().contains(coordinates.x, coordinates.y) && !typeChosen){
                    typeConsole = true;
                    typeChosen = true;
                }
                if (!displayStar && nokia.getGlobalBounds().contains(coordinates.x, coordinates.y) && !typeChosen){
                    typeChosen = true;
                    typeConsole = false;
                }
                if (displayGoomba && (titleBox.getGlobalBounds().contains(coordinates.x, coordinates.y) ||
                                      star.getGlobalBounds().contains(coordinates.x, coordinates.y))){
                    displayStar = false;
                    cursor.setString("|");
                    cursor.setPosition(10,53);
                    displayGoomba = false;
                    titleSearchTraits.clear();
                }
                if (titleBox.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    isEditing = true;
                    cursor.setString("|");
                    cursor.setPosition(10, 53);
                    cursor2.setString("");
                }
                if (searchButton.getGlobalBounds().contains(coordinates.x, coordinates.y)){
                    searchButtonPressed = true;
                    if (typeConsole) {
                        auto game = any_cast<vector<ConsoleNode*>>(searchRanking(typeConsole, ConsoleGames, AppGames, searchParams, givenVals));
                        int y = 250;
                        for (int i = 0; i < game.size(); i++) {
                            sf::Text currentGame;
                            string temp;
                            string line = to_string(i + 1) + ".) Title: " + game[i]->Title;
                            makeText(currentGame, font, line, 25, 425, y);
                            searchOfGames.push_back(currentGame);

                            temp = to_string(game[i]->price);
                            int index = temp.find('.');
                            temp = temp.substr(0, index + 3);
                            line = "Price: $" + temp;
                            makeText(currentGame, font, line, 25, 490, y + 25);
                            searchOfGames.push_back(currentGame);

                            //second line
                            line = "Game Type: ";
                            line += (game[i]->singlePlayer) ? "Single-Player" : "Co-Op";
//                            line += " Connectivity: ";
//                            line += (game[i]->online) ? "Online" : "Local";
                            makeText(currentGame, font, line, 25, 490, y + 50);
                            searchOfGames.push_back(currentGame);

                            temp = to_string(game[i]->review);
                            temp = temp.substr(0, 2);
                            line = "Review: " + temp;
                            makeText(currentGame, font, line, 25, 490, y + 75);
                            searchOfGames.push_back(currentGame);

                            line = "Release Year: ";
                            temp = to_string(game[i]->release);
                            index = temp.find('.');
                            temp = temp.substr(0, index);
                            line += temp;
                            makeText(currentGame, font, line, 25, 490, y + 100);
                            searchOfGames.push_back(currentGame);

                            line.clear();
                            line += "Publisher: ";
                            for (int j = 0; j < game[i]->publishers.size(); j++){
                                if (game[i]->publishers[game[i]->publishers.size() - 1] == game[i]->publishers[j]){
                                    line += game[i]->publishers[j];
                                    continue;
                                }
                                line += game[i]->publishers[j] + ", ";
                            }
                            makeText(currentGame, font, line, 25, 490, y + 125);
                            searchOfGames.push_back(currentGame);

                            line.clear();
                            line += "Platform: ";
                            for (int j = 0; j < game[i]->console.size(); j++){
                                if (game[i]->console[game[i]->console.size() - 1] == game[i]->console[j]){
                                    line += game[i]->console[j];
                                    continue;
                                }
                                line += game[i]->console[j] + ", ";
                            }
                            makeText(currentGame, font, line, 25, 490, y + 150);
                            searchOfGames.push_back(currentGame);
                            y += 225;
                        }
                    } else {
                        auto game = any_cast<vector<AppNode*>>(searchRanking(typeConsole, ConsoleGames, AppGames, searchParams, givenVals));
                        int y = 250;
                        for (int i = 0; i < game.size(); i++) {
                            sf::Text currentGame;
                            string temp;
                            string line = to_string(i + 1) + ".) Title: " + game[i]->Title;
                            makeText(currentGame, font, line, 25, 425, y);
                            searchOfGames.push_back(currentGame);

                            temp = to_string(game[i]->price);
                            int index = temp.find('.');
                            temp = temp.substr(0, index + 3);
                            line = "Price: $" + temp;
                            makeText(currentGame, font, line, 25, 490, y + 25);
                            searchOfGames.push_back(currentGame);

                            //second line
                            line = "In-App Purchases: $";
                            int size = (game[i]->inAppPurchases.size() > 5) ? 5 : game[i]->inAppPurchases.size();
                            for (int j = 0; j < size; j++) {
                                string price = (game[i]->inAppPurchases[j].find(' ') == 0) ? game[i]->inAppPurchases[j].substr(1) : game[i]->inAppPurchases[j];
                                if (j == size - 1) {
                                    line += price;
                                    continue;
                                }
                                line += price + ", $";
                            }
                            line += (game[i]->inAppPurchases.size() > 5) ? ", ..." : "";
                            makeText(currentGame, font, line, 25, 490, y + 50);
                            searchOfGames.push_back(currentGame);

                            line = "Genres: ";
                            size = (game[i]->genres.size() > 5) ? 5 : game[i]->genres.size();
                            for (int j = 0; j < size; j++) {
                                if (game[i]->genres[j] == game[i]->genres[size - 1]) {
                                    line += game[i]->genres[j];
                                    continue;
                                }
                                line += game[i]->genres[j] + ",";
                            }
                            line += (game[i]->genres.size() > 5) ? ", ..." : "";
                            makeText(currentGame, font, line, 25, 490, y + 75);
                            searchOfGames.push_back(currentGame);

                            temp = to_string(game[i]->rating);
                            temp = temp.substr(0, 3);
                            line = "Review: " + temp;
                            makeText(currentGame, font, line, 25, 490, y + 100);
                            searchOfGames.push_back(currentGame);

                            line = "Age Rating: " + game[i]->age;
                            makeText(currentGame, font, line, 25, 490, y + 125);
                            searchOfGames.push_back(currentGame);

                            line = "Last Update: " + game[i]->updateData;
                            makeText(currentGame, font, line, 25, 490, y + 150);
                            searchOfGames.push_back(currentGame);

                            line = "Developer: " + game[i]->developer;
                            makeText(currentGame, font, line, 25, 490, y + 175);
                            searchOfGames.push_back(currentGame);

                            line = "Size: ";
                            temp = game[i]->size;
                            index = temp.find('.');
                            temp = temp.substr(0, index + 3);
                            line += temp;
                            makeText(currentGame, font, line + "MB", 25, 490, y + 200);
                            searchOfGames.push_back(currentGame);

                            line = "Languages: ";
                            size = (game[i]->languages.size() > 5) ? 5 : game[i]->languages.size();
                            for (int j = 0; j < size; j++) {
                                if (game[i]->languages[j] == game[i]->languages[size - 1]) {
                                    line += game[i]->languages[j];
                                    continue;
                                }
                                line += game[i]->languages[j] + ",";
                            }
                            line += (game[i]->languages.size() > 5) ? ", ..." : "";
                            makeText(currentGame, font, line, 25, 490, y + 225);
                            searchOfGames.push_back(currentGame);

                            line = "URL: " + game[i]->url;
                            makeText(currentGame, font, line, 25, 250, y + 250);
                            searchOfGames.push_back(currentGame);
                            y += 320;
                        }
                    }

                }
                if (coal.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { //option 1
                    maxNumBoxes = 1;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
                    cursor.setString("|");
                    cursor.setPosition(475, 250);
                    searchButton.setPosition(700, 320);
                }
                if (iron.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { // option 2
                    maxNumBoxes = 2;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
                    cursor.setString("|");
                    cursor.setPosition(475, 250);
                    int increment = (maxNumBoxes - 1) * 50;
                    int x = 320 + increment;
                    searchButton.setPosition(700, x);
                }
                if (gold.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { //option 3
                    maxNumBoxes = 3;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
                    cursor.setString("|");
                    cursor.setPosition(475, 250);
                    int increment = (maxNumBoxes - 1) * 50;
                    int x = 320 + increment;
                    searchButton.setPosition(700, x);
                }
                if (diamond.getGlobalBounds().contains(coordinates.x, coordinates.y) && selecting) { //option 4
                    maxNumBoxes = 4;
                    parametersQ = true;
                    isEditing = false;
                    selecting = false;
                    cursor.setString("|");
                    cursor.setPosition(475, 250);
                    int increment = (maxNumBoxes - 1) * 50;
                    int x = 320 + increment;
                    searchButton.setPosition(700, x);
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
                            int x = 250 + increment;
                            cursor.setPosition(475, x);
                            respawnClicked = false;
                            crownClicked = false;
                        } else if (!respawnClicked && !crownClicked){
                            input += event.text.unicode;
                            cursor.setString(input + "|");
                            int increment = (numBoxes - 2) * 50;
                            int x = 250 + increment;
                            cursor.setPosition(795, x);
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
                        if (!parameterGiven) { //the first input box
                            searchParameter1 = input.toAnsiString();
                            searchParams.push_back(searchParameter1);
                            if (numBoxes > maxNumBoxes) {
                                allowTextInput = false;
                            }
                            numBoxes++; //increment the number of boxes
                            crownClicked = false;
                            respawnClicked = false;
                            parameterGiven = true;
                            // place the inputted text upon enter key
                            int increment = (numBoxes - 2) * 50;
                            int x = 250 + increment;
                            makeText(searchParameterDisplay, font, searchParameter1, 30, 475, x);
                            //clear the input for next key strokes
                            input.clear();
                            searchTexts.push_back(searchParameterDisplay);
                            if (!searchParameter1.empty()) {
                                transform(searchParameter1.begin(), searchParameter1.end(),
                                          searchParameter1.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << searchParameter1 << endl;
                            }
                        } else { //value boxes
                            valueToSearch = input.toAnsiString();
                            givenVals.push_back(valueToSearch);
                            if (valBoxes > maxNumBoxes) {
                                allowTextInput = false;
                            }
                            if (!crownClicked && !respawnClicked) {
                                valBoxes++;
                            }
                            parameterGiven = false;
                            input.clear();
                            int increment = (numBoxes - 2) * 50;
                            int x = 250 + increment;
                            makeText(valueToSearchDisplay, font, valueToSearch, 25, 795, x);
                            searchVals.push_back(valueToSearchDisplay);
                            if (!valueToSearch.empty()) {
                                transform(valueToSearch.begin(), valueToSearch.end(), valueToSearch.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                cout << valueToSearch << endl;
                            }
                        }
                    }
                    // enter on the title search
                    // block describes implementation of enter key
                    if (isEditing && event.key.code == sf::Keyboard::Enter && allowTextInput) {
                        //first line
                        titleSearch = input.toAnsiString();
                        if (titleSearch[titleSearch.size() - 1] == ' ' || titleSearch.find('\r') == 0){
                            titleSearch.pop_back();
                            cout << "entered" << endl;
                        }
                        if (!titleSearch.empty()){
                            transform(titleSearch.begin(), titleSearch.end(), titleSearch.begin(),
                                      [](unsigned char c) { return std::tolower(c); });
                            isEditing = false;
                            displayGoomba = true;
                        } else {
                            cout << "empty string detected" << endl;
                            continue;
                        }
                        if (ConsoleGames.find(titleSearch) != ConsoleGames.end()){
                            cout << ConsoleGames[titleSearch]->Title << "\n";
                            auto foundVal = ConsoleGames.find(titleSearch);
                            int y = 250;
                            for (int i = 0; i < traits.size(); i++){
                                sf::Text titleTraits;
                                //string temp = traits[i];
                                if(traits[i] == "Platform" || traits[i] == "Genre" || traits[i] == "Publisher"){
                                    auto gameTraitValue = any_cast<vector<string>>(consoleTraits[traits[i]](*foundVal->second));
                                    string s;
                                    for (int j = 0; j < gameTraitValue.size(); j ++){
                                        if (gameTraitValue[gameTraitValue.size() - 1] == gameTraitValue[j]){
                                            s += gameTraitValue[j];
                                            continue;
                                        }
                                        s += gameTraitValue[j] + ", ";
                                    }
                                    makeText(titleTraits, font, traits[i] + ": " + s, 25, 545, y);
                                    titleSearchTraits.push_back(titleTraits);
                                } else {
                                    auto gameTraitValue = any_cast<string>(consoleTraits[traits[i]](*foundVal->second));
                                    int index = gameTraitValue.find('.');
                                    if (traits[i] == "Price"){
                                        gameTraitValue = gameTraitValue.substr(0, index + 3);
                                    }
                                    makeText(titleTraits, font, traits[i] + ": " + gameTraitValue, 25, 545, y);
                                    titleSearchTraits.push_back(titleTraits);
                                }
                                y += 50;
                            }
                            displayStar = true;
                        } else if (AppGames.find(titleSearch) != AppGames.end()){
                            auto foundVal = AppGames.find(titleSearch);
                            int y = 250;
                            //epic
                            for (int i = 0; i < traits1.size(); i++){
                                sf::Text titleTraits;
                                //string temp = traits[i];
                                if(traits1[i] == "Genre" || traits1[i] == "In-App Purchases" || traits1[i] == "Languages"){
                                    auto gameTraitValue = any_cast<vector<string>>(appTraits[traits1[i]](*foundVal->second));
                                    string s;
                                    int size = (gameTraitValue.size() > 5) ? 5 : gameTraitValue.size();
                                    for (int j = 0; j < size; j++) {
                                        if (gameTraitValue[size - 1] == gameTraitValue[j]) {
                                            s += gameTraitValue[j];
                                            continue;
                                        }
                                        s += gameTraitValue[j] + ",";
                                    }
                                    s += (gameTraitValue.size() > 5) ? ", ..." : "";
                                    makeText(titleTraits, font, traits1[i] + ": " + s, 25, 545, y);
                                    titleSearchTraits.push_back(titleTraits);
                                } else {
                                    auto gameTraitValue = any_cast<string>(appTraits[traits1[i]](*foundVal->second));
                                    if (traits1[i] == "Price"){
                                        int index = gameTraitValue.find('.');
                                        gameTraitValue = gameTraitValue.substr(0, index + 3);
                                    }
                                    if (traits1[i] == "Size"){
                                        int index = gameTraitValue.find('.');
                                        gameTraitValue = gameTraitValue.substr(0, index + 3);
                                        makeText(titleTraits, font, traits1[i] + ": " + gameTraitValue + "MB", 25, 545, y);
                                        titleSearchTraits.push_back(titleTraits);
                                        y += 50;
                                        continue;
                                    }
                                    if (traits1[i] == "Review"){
                                        gameTraitValue = gameTraitValue.substr(0, 3);
                                    }
                                    makeText(titleTraits, font, traits1[i] + ": " + gameTraitValue, 25, 545, y);
                                    if (traits1[i] == "URL"){
                                        titleTraits.setPosition(350, y);
                                    }
                                    titleSearchTraits.push_back(titleTraits);
                                }
                                y += 50;
                            }
                            displayStar = true;
                        }
                        else {
                            cout << "DNE" << endl;
                        }
                        titleSearch = input.substring(0, 20).toAnsiString();
                        makeText(titleSearchDisplay, font, titleSearch, 23, 10, 53);
                        if (input.toAnsiString().length() > 20) { //second line
                            titleSearch2 = input.substring(20).toAnsiString();
                            makeText(titleSearchDisplay2, font, titleSearch2, 23, 10, 83);
                        } else {
                            makeText(titleSearchDisplay2, font, "", 23, 10, 83);
                        }
                        //clear for next key strokes
                        input.clear();
                        titleSearch.clear();
                        cursor.setString("");
                        cursor2.setString("");
                    }
                }
            }
            if (displayStar) {
                allGames.clear();
            }
        }

        // order should be clear, draw, display.
        window.clear(sf::Color::Cyan);
        //permanent draw funcs
        window.setView(view);
        window.draw(waterVaporText);
        window.draw(browseConsole);
        window.draw(pacman);
        window.draw(browseIOS);
        window.draw(shield);
        window.draw(aboutUS);
        window.draw(pokeball);
        //search by title
        window.draw(sByTitle);
        window.draw(titleBox);
        //if title box is clicked write there
        if(isEditing && !parametersQ){
            window.draw(cursor);
            window.draw(cursor2);
        }
        if(displayGoomba){
            window.draw(titleSearchDisplay);
            window.draw(titleSearchDisplay2);
            window.draw(goomba);
            if (!displayStar){
                window.draw(errorTitle);
            }
            if (displayStar){ //search expected
                window.draw(star);
                for (auto& iter : titleSearchTraits){
                    window.draw(iter);
                }
            }
        }

        if (!shieldClicked && !pacmanClicked && !pokeballClicked) {
            if (typeChosen && typeConsole && !displayStar && !searchButtonPressed) {
                //display of what is valid
                window.draw(validParametersConsole);
                if (moreInfoClicked) { // more info display pops up when clicked
                    window.draw(moreInfoConsole);
                }
            } else if (typeChosen && !typeConsole && !displayStar && !searchButtonPressed) {
                window.draw(validParametersIOS);
                if (moreInfoClicked) {
                    window.draw(moreInfoIOS);
                }
            }
        }

        if (typeChosen && !pacmanClicked && !shieldClicked && !pokeballClicked && !displayStar) { // mobile or console has been chosen
            if (parametersQ) {//chosen number of parameters
                //Need to say that order matters
                window.draw(backButton);
                window.draw(crown);
                if (!searchButtonPressed){
                    window.draw(parameterType);
                    window.draw(valEntered);
                    int y = 250;
                    for (int i = 1; i <= valBoxes; i++) {
                        //number of boxes increments with each enter
                        makeInputBox(window, 200, 40, 795, y); // search boxes pop up
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

                    if (valBoxes > maxNumBoxes) {
                        window.draw(searchButton);
                    }
                    y = 250;
                    for (int i = 1; i <= numBoxes; i++) {
                        window.draw(priorities[i - 1]);
                        makeInputBox(window, 300, 40, 475, y); // search boxes pop up
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
                    for (auto &iter : searchOfGames){
                        window.draw(iter);
                    }
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
        } else if (!pacmanClicked && !shieldClicked && !pokeballClicked && !displayStar){ //display home screen
            window.draw(initialQ2);
            window.draw(nokia);
            window.draw(console);
        }
        //window.draw(chief);

        //new window display
        if (pacmanClicked || pokeballClicked || shieldClicked){ //display god
            //window.draw(bowser);
            for (sf::Text& item : allGames) {
                window.draw(item);
            }
        }
        window.draw(respawn); //home button

        window.display();
    }
}
