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
    vector<float> inAppPurchases;
    string description;
    string developer;
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
    string rating;
    int release{};
    float allPlayStyle{};
    vector<float> completionists;
    vector<float> storyDLC;
    vector<float> story;
};


static void insertAppData(vector<string>& data, unordered_map<string, AppNode*>& catalogue) {
    if ((data.at(2).find("\\u") == string::npos) && (data.at(3).find("ht") == 0)) {
        string key = data.at(2);
        int index = 7;
        transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
        catalogue.insert(make_pair(key, new AppNode));
        cout << catalogue.size() << "\t" << data.at(2) << "\t\t\t" << data.at(4) << "\t" << data.at(6) << endl;
        catalogue[key]->url = data.at(0);
        catalogue[key]->ID = stoi(data.at(1));
        catalogue[key]->Title = data.at(2);
        catalogue[key]->iconURL = data.at(3);
        if (data.at(4) != "N/A") {
            catalogue[key]->rating = stof(data.at(4));
            catalogue[key]->ratingCount = stoi(data.at(5));
        } else {
            catalogue[key]->rating = 0.0;
            catalogue[key]->ratingCount = 0;
        }
        catalogue[key]->price = stof(data.at(6));
        if (data.at(7) == "N/A") {
            catalogue[key]->inAppPurchases.push_back(0.0);
        } else {
            if (data.at(index).find("\"") == 0) {
                catalogue[key]->inAppPurchases.push_back(stof(data.at(index).substr(1)));
                index++;
                while (true) {
                    if (data.at(index).find("\"", 1) == data.at(index).size() - 1) {
                        catalogue[key]->inAppPurchases.push_back(
                                stof(data.at(index).substr(0, data.at(index).size() - 1)));
                        break;
                    }
                    catalogue[key]->inAppPurchases.push_back(stof(data.at(index)));
                    index++;
                }
            } else {
                catalogue[key]->inAppPurchases.push_back(stof(data.at(7)));
                index++;
            }
        }
    }
}

static void insertConsoleData(vector<string>& data, unordered_map<string, ConsoleNode*>& catalogue) {
    string key = data.at(0);
    int index = 3;
    transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
    if (catalogue.count(key) == 0) {
        catalogue.insert(make_pair(key, new ConsoleNode));
        catalogue[key]->Title = data.at(0);
        cout << catalogue.size() << "\t" << data.at(0) << "\t\t\t" << data.at(1) << "\t" << data.at(8) << endl;
        catalogue[key]->players = stoi(data.at(1));
        catalogue[key]->online = (data.at(2) == "TRUE");
        if (data.at(index).find("\"") == 0) {
            catalogue[key]->genres.push_back(data.at(index).substr(1));
            index++;
            while (true) {
                if (data.at(index).find("\"", 1) == data.at(index).size() - 1) {
                    catalogue[key]->genres.push_back(data.at(index).substr(0, data.at(index).size()-1));
                    break;
                }
                catalogue[key]->genres.push_back(data.at(index));
                index++;
            }
        } else {
            catalogue[key]->genres.push_back(data.at(3));
        }
        //Strange indexing due to need to index from back, since we don't know how many genres there are
        catalogue[key]->publishers = data.at(data.size()-17);
        catalogue[key]->review = stoi(data.at(data.size()-16));
        catalogue[key]->sales = stof(data.at(data.size()-15));
        catalogue[key]->price = stof(data.at(data.size()-14));
        catalogue[key]->console.push_back(data.at(data.size()-13));
        catalogue[key]->rating = data.at(data.size()-12);
        catalogue[key]->release = stoi(data.at(data.size()-11));
        catalogue[key]->allPlayStyle = stof(data.at(data.size()-10));
        catalogue[key]->completionists.push_back(stof(data.at(data.size()-9)));
        catalogue[key]->completionists.push_back(stof(data.at(data.size()-8)));
        catalogue[key]->completionists.push_back(stof(data.at(data.size()-7)));
        catalogue[key]->storyDLC.push_back(stof(data.at(data.size()-6)));
        catalogue[key]->storyDLC.push_back(stof(data.at(data.size()-5)));
        catalogue[key]->storyDLC.push_back(stof(data.at(data.size()-4)));
        catalogue[key]->story.push_back(stof(data.at(data.size()-3)));
        catalogue[key]->story.push_back(stof(data.at(data.size()-2)));
        catalogue[key]->story.push_back(stof(data.at(data.size()-1)));
    } else {
        catalogue[key]->console.push_back(data.at(data.size() - 13)); //minus 13 because there is unknown amount of genre, so have to index from the back
    }


}