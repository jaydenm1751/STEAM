//
// Created by jayde on 4/19/2023.
//

#ifndef WATERVAPOR_HASHTABLE_H
#define WATERVAPOR_HASHTABLE_H

#include <iostream>
#include <utility>
#include <vector>
#include <list>

using namespace std;

class HashTable{
private:
    struct Node {
        string data;
    };

    // hash table
    vector<list<Node*>> map;
    const float maxLoadFactor = 0.75;
    int size;
    int capacity;
    Node* searchData(string data){ //called from inside
        //find index
        int index = hash(data);
        //loop over the specific list in the table
        for (auto iter = map[index].begin(); iter != map[index].end(); iter++){
            if ((*iter)->data == data){
                return *iter; //value found
            }
        }
        return nullptr;
    }
public:

    HashTable(){ // hash table with seperate chaining implementation
        list<Node*> nodes; // empty lists
        size = 0;
        capacity = 100;
        map.resize(capacity, nodes);
    }
    int hash(string data){ // raw hash index
        int index = 0;
        int power = 1; //power of 2
        for (int i = 0; i < data.size(); i++){
            index += power * (data[i]);
            power *= 2;
        }
        while (index > map.size()){ // make sure the index is valid
            index /= 10;
        }
        return index;
    }

    void insert(string data){
        Node* node = new Node;
        node->data = data;
        int index = hash(data);

        map[index].push_back(node); //append to the list
        size++;

        float currentLoadFactor = (float) size / (float) capacity;
        if(currentLoadFactor >= maxLoadFactor){
            list<Node*> temp;
            map.resize(capacity * 2, temp);
            capacity *= 2;
        }
    }

    void search(string data){ //called from logic of search
        Node* node = searchData(data);
        if (node != nullptr){
            //do something with the value found
            cout << node->data << endl;
        } else {
            cout << "Could not find " << data << ". " << endl;
        }
    }
};



#endif //WATERVAPOR_HASHTABLE_H
