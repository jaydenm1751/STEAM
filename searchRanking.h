//
// Created by caleb on 4/24/2023.
//

#ifndef SEARCH_RANKING_H
#define SEARCH_RANKING_H

#include "ConsoleAppsNodes.h"
#include <unordered_map>
#include <iostream>
#include <any>
#include <functional>
#include "PriorityQ.h"
#include <iomanip>
#include <any>

//string anyToString(const any& value);

any searchRanking(bool console, unordered_map<string, ConsoleNode*>& ConsoleGames, unordered_map<string, AppNode*>& AppGames, vector<string>& searchParams, vector<string>& givenVals);

#endif