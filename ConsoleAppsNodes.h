//
// Created by caleb on 4/25/2023.
//

#ifndef WATERVAPOR_CONSOLEAPPSNODES_H
#define WATERVAPOR_CONSOLEAPPSNODES_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct AppNode {
    string Title;
    string url;
    int ID;
    string iconURL;
    float rating;
    int ratingCount;
    float price; //search
    vector<string> inAppPurchases; //search
    string developer;
    string age; //search
    vector<string> languages;
    string size; //search
    vector<string> genres; //search
    string releaseDate;
    string updateData; //search
};

struct ConsoleNode {
    string Title;
    int players{}; //search if it is single player or coop (1 or more than 1)
    bool singlePlayer{};
    bool online{}; //search
    vector<string> genres; //search
    vector<string> publishers;
    float review{}; //search
    float sales{};
    float price{}; //search
    vector<string> console; //search
    string rating; //search
    float release{}; //search
    string allPlayStyle;
    vector<string> completionists;
    vector<string> storyDLC;
    vector<string> story;
};

void insertAppData(vector<string>& data, unordered_map<string, AppNode*>& catalogue);

void insertConsoleData(vector<string>& data, unordered_map<string, ConsoleNode*>& catalogue);

#endif //WATERVAPOR_CONSOLEAPPSNODES_H
