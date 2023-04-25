//
// Created by caleb on 4/24/2023.
//

#include "searchRanking.h"

//string anyToString(const std::any& value) {
//    stringstream ss;
//
//    if (value.type() == typeid(int)) {
//        return to_string(any_cast<int>(value));
//    } else if (value.type() == typeid(float)) {
//        ss << fixed << setprecision(2) << any_cast<float>(value);
//        return ss.str();
//    } else if (value.type() == typeid(string)) {
//        return any_cast<string>(value);
//    }
//
//    return "Unknown type";
//}

void searchRanking(bool console, unordered_map<string, ConsoleNode*>& ConsoleGames, unordered_map<string, AppNode*> AppGames, vector<string>& searchParams, vector<string>& givenVals) {
    unordered_map <string, function<any(const ConsoleNode&)>> consoleTraits = {
            {"title", [](const ConsoleNode& node) -> string { return node.Title; }},
            {"player count", [](const ConsoleNode& node) -> float { return (float)node.players; }},
            {"game type", [](const ConsoleNode& node) -> bool { return node.players > 1; }},
            {"connectivity", [](const ConsoleNode& node) -> bool { return node.online; }},
            {"review", [](const ConsoleNode& node) -> float { return node.review; }},
            {"genre", [](const ConsoleNode& node) -> vector<string> { return node.genres; }},
            {"price", [](const ConsoleNode& node) -> float { return node.price; }},
            {"platform", [](const ConsoleNode& node) -> vector<string> { return node.console; }},
            {"age rating", [](const ConsoleNode& node) -> string { return node.rating; }},
            {"release year", [](const ConsoleNode& node) -> float { return node.release; }},
    };

    unordered_map<std::string, std::function<any(const AppNode&)>> appTraits = {
            {"title", [](const AppNode& node) -> string { return node.Title; }},
            {"review", [](const AppNode& node) -> float { return node.rating; }},
            {"price", [](const AppNode& node) -> float { return node.price; }},
            {"in-app purchases", [](const AppNode& node) -> vector<float> { return node.inAppPurchases; }},
            {"age rating", [](const AppNode& node) -> string { return node.age; }},
            {"size", [](const AppNode& node) -> long long unsigned { return node.size; }},
            {"genre", [](const AppNode& node) -> vector<string> { return node.genres; }},
            {"update year", [](const AppNode& node) -> string { return node.updateData; }},
    };

    unordered_map <string, float> mappedRanks;
    PriorityQ ranked(false);
    for (int j = 0; j < searchParams.size(); j++) {
        string param = searchParams.at(j);
        string val = givenVals.at(j);
        //Input handling to remove any of the formatting characters
        // that may have been added on to the input strings
        if(param.find('\r') == 0)
            param = param.substr(1);
        if(param.find(' ') == param.size() - 1)
            param = param.substr(0,param.size() - 1);
        if(val.find('\r') == 0)
            val = val.substr(1);
        transform(param.begin(), param.end(), param.begin(),
                  [](unsigned char c) { return std::tolower(c); });
        transform(val.begin(), val.end(), val.begin(),
                  [](unsigned char c) { return std::tolower(c); });
        if (console) {
            if (consoleTraits.find(param) != consoleTraits.end()) {
                auto iter = ConsoleGames.begin();
                PriorityQ pQ(true);
                vector<string> titles;
                //TODO: Have input handling for all possible search parameters
                if (param == "price" || param == "release year" || param == "review" || param == "player count") {
                    //if statement for all the float based search parameters
                    // since they share operations
                    while (iter != ConsoleGames.end()) {
                        auto gameTraitValue = any_cast<float>(consoleTraits[param](*iter->second));
                        float queueVal = abs(stof(val) - gameTraitValue);
                        pQ.insert(to_string(queueVal), iter->second->Title);
                        iter++;
                    }
                } else if (param == "rating" || param == "genre" || param == "platform") {
                    //if statement to handle all the string based search parameter
                    // since they have similar qualities
                    int base = 0;
                    if (val.size() == 1) {
                        if (val == "E")
                            base = 1;
                        else if (val == "T")
                            base = 2;
                        else
                            base = 4; //set M games to equal 4 so that there is no tie
                        // between E and M in the case the user sets T as their rating
                    }
                    if (param != "rating") {
                        while (iter != ConsoleGames.end()) {
                            auto vec = any_cast < vector < string >> (consoleTraits[param](*iter->second));
                            for (string s: vec) {
                                transform(s.begin(), s.end(), s.begin(),
                                          [](unsigned char c) { return std::tolower(c); });
                                if (s == val) {
                                    titles.push_back(iter->second->Title);
                                    break;
                                }
                            }
                            iter++;
                        }
                    } else {
                        while (iter != ConsoleGames.end()) {
                            int comp;
                            if (iter->second->rating == "E")
                                comp = 1;
                            else if (iter->second->rating == "T")
                                comp = 2;
                            else
                                comp = 4;
                            int queueVal = abs(base - comp);
                            pQ.insert(to_string(queueVal), iter->second->Title);
                            iter++;
                        }
                    }
                } else if (param == "game type" || param == "connectivity") {
                    if (param == "game type") {
                        while (iter != ConsoleGames.end()) {
                            if (val == "single-player") {
                                if (!any_cast<bool>(consoleTraits[param](*iter->second))) {
                                    titles.push_back(iter->second->Title);
                                }
                            } else if (val == "co-op") {
                                if (any_cast<bool>(consoleTraits[param](*iter->second))) {
                                    titles.push_back(iter->second->Title);
                                }
                            }
                            iter++;
                        }
                    } else {
                        while (iter != ConsoleGames.end()) {
                            if (val == "online") {
                                if (any_cast<bool>(consoleTraits[param](*iter->second))) {
                                    titles.push_back(iter->second->Title);
                                }
                            } else if (val == "local") {
                                if (!any_cast<bool>(consoleTraits[param](*iter->second))) {
                                    titles.push_back(iter->second->Title);
                                }
                            }
                            iter++;
                        }
                    }
                }
                if (param != "genre" && param != "platform" && param != "connectivity" && param != "game type") {
                    //Genre and console do not get stored into a priority queue
                    // because they are then handled in a different way
                    int size = (pQ.getSize() > 50) ? 50 : pQ.getSize();
                    double normalizeFactor =  100.0 / size;
                    for (int i = 0; i < size; i++) {
                        string key = pQ.extractedVal();
                        transform(key.begin(), key.end(), key.begin(),
                                  [](unsigned char c) { return std::tolower(c); });
                        mappedRanks[key] += (float)((size - i) / 100.0 * normalizeFactor) * (float)(4 - j);
                        //cout << ConsoleGames[key]->Title << "\t\t" << anyToString(consoleTraits[param](*ConsoleGames[key])) << "\n";
                    }
                    cout << "Here is " << size << " game(s) that fit your preferences of " << param << "\n";
                } else {
                    unsigned int size = (titles.size() > 50) ? 50 : titles.size();
                    double normalizeFactor =  100.0 / size;
                    for (unsigned int i = 0; i < size; i++) {
                        string key = titles.at(i);
                        transform(key.begin(), key.end(), key.begin(),
                                  [](unsigned char c) { return std::tolower(c); });
                        mappedRanks[key] += (float)((size - i) / 100.0 * normalizeFactor) * (float)(4 - j);
                    }
                    cout << "Here is " << size << " game(s) that fit your preferences of " << param << "\n";
                }
            } else {
                cout << "This is an invalid parameter. We're sorry for any inconvenience this may cause!\n";
            }
        } else {
            if (appTraits.find(param) != appTraits.end()) {
                auto iter = AppGames.begin();
            } else {
                cout << "This is an invalid parameter. We're sorry for any inconvenience this may cause!\n";
            }
        }
    }
    auto iter = mappedRanks.begin();
    while (iter != mappedRanks.end()) {
        ranked.insert(to_string(iter->second), iter->first);
        iter++;
    }
    int games = (ranked.getSize() > 10) ? 10 : ranked.getSize();
    for (int i = 0; i < games; i++) {
        string bestFits = ranked.extractedVal();
        cout << i+1 << ": " << ConsoleGames[bestFits]->Title << '-' << fixed << setprecision(4) << mappedRanks[bestFits] << '\n';
    }
}
