//
// Created by caleb on 4/25/2023.
//

#include "searchAppRanking.h"

void searchAppRanking(unordered_map<string, AppNode*>& AppGames, vector<string>& searchParams, vector<string>& givenVals) {
    unordered_map<string, function<any(const AppNode&)>> appTraits = {
            {"review", [](const AppNode& node) -> float { return node.rating; }},
            {"price", [](const AppNode& node) -> float { return node.price; }},
            {"in-app purchases", [](const AppNode& node) -> bool { return node.inAppPurchases.at(0) == "0"; }},
            {"age rating", [](const AppNode& node) -> string { return node.age; }},
            {"size", [](const AppNode& node) -> double { return stod(node.size); }},
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
        if (param.find('\r') == 0)
            param = param.substr(1);
        if (param.find(' ') == param.size() - 1)
            param = param.substr(0, param.size() - 1);
        if (val.find('\r') == 0)
            val = val.substr(1);
        transform(param.begin(), param.end(), param.begin(),
                  [](unsigned char c) { return std::tolower(c); });
        transform(val.begin(), val.end(), val.begin(),
                  [](unsigned char c) { return std::tolower(c); });
        if (appTraits.find(param) != appTraits.end()) {
            auto iter = AppGames.begin();
            PriorityQ pQ(true);
            vector<string> titles;
            if (param == "price" || param == "review") {
                while (iter != AppGames.end()) {
                    auto gameTraitValue = any_cast<float>(appTraits[param](*iter->second));
                    float queueVal = abs(stof(val) - gameTraitValue);
                    pQ.insert(to_string(queueVal), iter->first);
                    iter++;
                }
            } else if (param == "age rating" || param == "genre" || param == "update year") {

            } else if (param == "in-app purchases") {
                while (iter != AppGames.end()) {
                    if (val == "y" || val == "yes") {
                        if (any_cast<bool>(appTraits[param](*iter->second))) {
                            titles.push_back(iter->first);
                        }
                    }
                }
            } else {
                while (iter != AppGames.end()) {
                   auto gameTraitValue = any_cast<double>(appTraits[param](*iter->second));
                   double queueVal = abs(stod(val) - gameTraitValue);
                   pQ.insert(to_string(queueVal), iter->first);
                   iter++;
               }
            }
        } else {
            cout << "This is an invalid parameter. We're sorry for any inconvenience this may cause!\n";
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
        cout << i+1 << ": " << AppGames[bestFits]->Title << '-' << fixed << setprecision(4) << mappedRanks[bestFits] << '\n';
    }
}