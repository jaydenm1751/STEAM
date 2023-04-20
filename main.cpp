#include <iostream>
#include "HashTable.h"
#include "PriorityQ.h"

using namespace std;

int main() {
    HashTable map;

//    map.insert("Caleb");
//    map.insert("Jayden");
//    map.insert("Maxwell");
//    map.search("Caleb");
//    map.search("aCelb");

    PriorityQ q;

    q.insert("Caleb");
    q.insert("Jayden");
    q.insert("Maxwell");
    q.insert("Anthony");
    q.extractedVal();
    q.extractedVal();



    return 0;
}
