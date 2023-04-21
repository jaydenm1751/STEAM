//
// Created by jayde on 4/19/2023.
//

#ifndef WATERVAPOR_HASHTABLE_H
#define WATERVAPOR_HASHTABLE_H

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>

using namespace std;

class HashTable{
private:
    struct Node {
        string Title;
        int players;

    };

    // hash table
    vector<list<Node*>> map;
    const float maxLoadFactor = 0.75;
    int size;
    int capacity;
    Node* searchData(string title){ //called from inside
        //find index
        int index = hash(title);
        //loop over the specific list in the table
        for (auto iter = map[index].begin(); iter != map[index].end(); iter++){
            if ((*iter)->Title == title){
                return *iter; //value found
            }
        }
        return nullptr;
    }
    int hash(string title){ // raw hash index
        int index = 0;
        int power = 1; //power of 2
        for (int i = 0; i < title.size(); i++){
            index += power * (title[i]);
            power *= 2;
        }
        while (index > map.size()){ // make sure the index is valid
            index /= 10;
        }
        return index;
    }

public:
    HashTable(){ // hash table with seperate chaining implementation
        list<Node*> nodes; // empty lists
        size = 0;
        capacity = 100;
        map.resize(capacity, nodes);
    }
    void insert(string title){
        Node* node = new Node;
        node->Title = title;
        int index = hash(title);

        map[index].push_back(node); //append to the list
        size++;

        float currentLoadFactor = (float) size / (float) capacity;
        if(currentLoadFactor >= maxLoadFactor){
            list<Node*> temp;
            map.resize(capacity * 2, temp);
            capacity *= 2;
        }
    }

    void search(string title){ //called from logic of search
        Node* node = searchData(title);
        if (node != nullptr){
            //do something with the value found
            cout << node->Title << endl;
        } else {
            cout << "Could not find " << title << ". " << endl;
        }
    }

};



#endif //WATERVAPOR_HASHTABLE_H
