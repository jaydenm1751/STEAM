//
// Created by caleb on 4/25/2023.
//

#ifndef SEARCH_APP_RANKING_H
#define SEARCH_APP_RANKING_H
#include <iostream>
#include "ConsoleAppsNodes.h"
#include <any>
using namespace std;

vector<AppNode*> searchAppRanking(unordered_map<string, AppNode*>& AppGames, vector<string>& searchParams, vector<string>& givenVals);


#endif