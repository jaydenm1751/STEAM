//
// Created by caleb on 4/22/2023.
//
#include "ConsoleAppsNodes.h"

void insertAppData(vector<string>& data, unordered_map<string, AppNode*>& catalogue) {
    string key = data.at(2);
    transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
    if (catalogue.find(key) == catalogue.end()) {
        if ((data.at(2).find("\\u") == string::npos) && (data.at(2).find("\\x") == string::npos) &&
            (data.at(3).find("ht") == 0) && (data.at(2).find("\"") != 0)) {
            int index = 7;
            catalogue.insert(make_pair(key, new AppNode));
            //cout << catalogue.size() << "\t" << data.at(2) << "\t\t\t" << data.at(4) << "\t" << data.at(6) << endl;
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
            if (data.at(index) == "N/A") {
                catalogue[key]->inAppPurchases.emplace_back("0.00");
                index++;
            } else {
                if (data.at(index).find("\"") == 0) {
                    catalogue[key]->inAppPurchases.push_back(data.at(index).substr(1));
                    index++;
                    while (true) {
                        if (data.at(index).find("\"", 1) == data.at(index).size() - 1) {
                            catalogue[key]->inAppPurchases.push_back(
                                    data.at(index).substr(0, data.at(index).size() - 1));
                            index++;
                            break;
                        }
                        catalogue[key]->inAppPurchases.push_back(data.at(index));
                        index++;
                    }
                } else {
                    catalogue[key]->inAppPurchases.push_back(data.at(7));
                    index++;
                }
            }
            catalogue[key]->developer = data.at(index);
            index++;
            catalogue[key]->age = data.at(index);
            index++;
            if (data.at(index) == "N/A") {
                catalogue[key]->languages.emplace_back("Unavailable");
                index++;
            } else {
                if (data.at(index).find("\"") == 0) {
                    catalogue[key]->languages.push_back(data.at(index).substr(1));
                    index++;
                    while (true) {
                        if (data.at(index).find("\"", 1) == data.at(index).size() - 1) {
                            catalogue[key]->languages.push_back(data.at(index).substr(0, data.at(index).size() - 1));
                            index++;
                            break;
                        }
                        catalogue[key]->languages.push_back(data.at(index));
                        index++;
                    }
                } else {
                    catalogue[key]->languages.push_back(data.at(index));
                    index++;
                }
            }
            string sizeMB = to_string(stold(data.at(index)) / 1000000.0);
            catalogue[key]->size = sizeMB;
            index++;
            if (data.at(index) == "N/A") {
                catalogue[key]->genres.emplace_back("Unavailable");
            } else {
                if (data.at(index).find("\"") == 0) {
                    catalogue[key]->genres.push_back(data.at(index).substr(1));
                    index++;
                    while (true) {
                        if (data.at(index).find("\"", 1) == data.at(index).size() - 1) {
                            catalogue[key]->genres.push_back(data.at(index).substr(0, data.at(index).size() - 1));
                            break;
                        }
                        catalogue[key]->genres.push_back(data.at(index));
                        index++;
                    }
                } else {
                    catalogue[key]->genres.push_back(data.at(index));
                }
            }
            catalogue[key]->releaseDate = data.at(data.size() - 2);
            catalogue[key]->updateData = data.back();
        }
    }
}

void insertConsoleData(vector<string>& data, unordered_map<string, ConsoleNode*>& catalogue) {
    string key = data.at(0);
    int index = 3;
    transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
    if (catalogue.count(key) == 0) {
        catalogue.insert(make_pair(key, new ConsoleNode));
        catalogue[key]->Title = data.at(0);
        //cout << catalogue.size() << "\t" << data.at(0) << "\t\t\t" << data.at(1) << "\t" << data.at(8) << endl;
        catalogue[key]->players = stoi(data.at(1));
        if(stoi(data.at(1)) == 1) {
            catalogue[key]->singlePlayer = true;
        } else {
            catalogue[key]->singlePlayer = false;
        }
        catalogue[key]->online = (data.at(2) == "TRUE");
        if (data.at(index).find("\"") == 0) {
            catalogue[key]->genres.push_back(data.at(index).substr(1));
            index++;
            while (true) {
                if (data.at(index).find("\"", 1) == data.at(index).size() - 1) {
                    catalogue[key]->genres.push_back(data.at(index).substr(0, data.at(index).size()-1));
                    index++;
                    break;
                }
                catalogue[key]->genres.push_back(data.at(index));
                index++;
            }
        } else {
            catalogue[key]->genres.push_back(data.at(3));
            index++;
        }
        if (data.at(index).find("\"") == 0) {
            catalogue[key]->publishers.push_back(data.at(index).substr(1));
            index++;
            while (true) {
                if (data.at(index).find("\"", 1) == data.at(index).size() - 1) {
                    catalogue[key]->publishers.push_back(data.at(index).substr(0, data.at(index).size()-1));
                    break;
                }
                catalogue[key]->publishers.push_back(data.at(index));
                index++;
            }
        } else {
            catalogue[key]->publishers.push_back(data.at(data.size()-17));
        }
        //Strange indexing due to need to index from back, since we don't know how many genres there are
        catalogue[key]->review = stoi(data.at(data.size()-16));
        catalogue[key]->sales = stof(data.at(data.size()-15));
        catalogue[key]->price = stof(data.at(data.size()-14));
        catalogue[key]->console.push_back(data.at(data.size()-13));
        catalogue[key]->rating = data.at(data.size()-12);
        catalogue[key]->release = stoi(data.at(data.size()-11));
        catalogue[key]->allPlayStyle = data.at(data.size()-10);
        catalogue[key]->completionists.push_back(data.at(data.size()-9));
        catalogue[key]->completionists.push_back(data.at(data.size()-8));
        catalogue[key]->completionists.push_back(data.at(data.size()-7));
        catalogue[key]->storyDLC.push_back(data.at(data.size()-6));
        catalogue[key]->storyDLC.push_back(data.at(data.size()-5));
        catalogue[key]->storyDLC.push_back(data.at(data.size()-4));
        catalogue[key]->story.push_back(data.at(data.size()-3));
        catalogue[key]->story.push_back(data.at(data.size()-2));
        catalogue[key]->story.push_back(data.at(data.size()-1));
    } else {
        catalogue[key]->console.push_back(data.at(data.size() - 13)); //minus 13 because there is unknown amount of genre, so have to index from the back
    }
}