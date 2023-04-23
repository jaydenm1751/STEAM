#include <iostream>
#include "HashTable.h"
#include "HashTableApp.h"
#include "PriorityQ.h"
//#include "TextureManager.h"
#include "StorePage.cpp"
//#include <SFML/Graphics.hpp>
//#include <unordered_map>
//#include "ConsoleAppsNodes.cpp"
//#include <chrono>
//#include <stdexcept>

using namespace std;


int main(){

    makeGUI();

    PriorityQ q;
    //InitializeMapConsole(ConsoleGames);

    q.insert("A");
    q.insert("B");
    q.insert("D");
    q.insert("H");
    q.insert("C");
    q.insert("Anthony");
    q.extractedVal();
    q.extractedVal();
    q.extractedVal();

    return 0;
}