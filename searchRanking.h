//
// Created by caleb on 4/24/2023.
//

#ifndef SEARCH_RANKING_H
#define SEARCH_RANKING_H

#include "ConsoleAppsNodes.cpp"
#include "searchRanking.h"
#include <unordered_map>
#include <iostream>
#include <any>
#include <functional>
#include "PriorityQ.h"
#include <iomanip>

//string anyToString(const any& value);

void searchRanking(bool console, unordered_map<string, ConsoleNode*>& ConsoleGames, unordered_map<string, AppNode*>& AppGames, vector<string>& searchParams, vector<string>& givenVals);

#endif
