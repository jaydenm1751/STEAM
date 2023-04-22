//
// Created by caleb on 4/22/2023.
//
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
    float price;
    float inAppPurchases;
    string Developer;
    string age;
    float size;
    vector<string> genres;
    string releaseDate;
    string updateData;
};

struct ConsoleNode {
    string Title;
    int players{};
    bool online{};
    vector<string> genres;
    string publishers;
    int review{};
    float sales{};
    float price{};
    vector<string> console;
    char rating{};
    int release{};
    float allPlayStyle{};
    vector<float> completionists;
    vector<float> storyDLC;
    vector<float> story;
};


static void insertAppData(vector<string>& data, unordered_map<string, AppNode*>& catalogue) {

}

static void insertConsoleData(vector<string>& data, unordered_map<string, ConsoleNode*>& catalogue) {
    string key = data.at(0);
    transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
    if (catalogue.count(key) == 0) {
        catalogue.insert(make_pair(key, new ConsoleNode));
    }
    catalogue[key]->Title = data.at(0);
    //cout << catalogue.size() << "\t" << data.at(0) << "\t\t\t" << data.at(1) << "\t" << data.at(8) << endl;
    catalogue[key]->players = stoi(data.at(1));
    catalogue[key]->online = (data.at(2) == "TRUE");
    //catalogue[data.at(0)]->
}