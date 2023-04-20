#include <iostream>
#include "HashTable.h"

using namespace std;

int main() {
    HashTable map;

    map.insert("Caleb");
    map.insert("Jayden");
    map.insert("Maxwell");

    map.search("Caleb");
    map.search("aCelb");



    return 0;
}
