//
// Created by caleb on 4/25/2023.
//

#ifndef SEARCH_CONSOLE_RANKING_H
#define SEARCH_CONSOLE_RANKING_H
#include <iostream>
#include "ConsoleAppsNodes.h"
using namespace std;

vector<ConsoleNode*> searchConsoleRanking (unordered_map<string, ConsoleNode*>& ConsoleGames, vector<string>& searchParams, vector<string>& givenVals);

#endif