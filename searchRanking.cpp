//
// Created by caleb on 4/24/2023.
//

#include "searchRanking.h"
#include "searchConsoleRanking.h"
#include "searchAppRanking.h"
#include <any>

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

any searchRanking(bool console, unordered_map<string, ConsoleNode*>& ConsoleGames, unordered_map<string, AppNode*>& AppGames, vector<string>& searchParams, vector<string>& givenVals) {
    if (console) {
        return searchConsoleRanking(ConsoleGames, searchParams, givenVals);
    } else {
        return searchAppRanking(AppGames, searchParams, givenVals);
    }
}