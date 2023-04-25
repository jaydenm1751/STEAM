//
// Created by jayde on 4/20/2023.
//

#ifndef WATERVAPOR_PRIORITYQ_H
#define WATERVAPOR_PRIORITYQ_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class PriorityQ {
private:
    struct Node {
        string data;
        string title;
        int aScore{};
        float fScore{};
    };

    bool min;
    vector<Node*> pQ;
    int size;

    Node* extractMin(){ //remove the root
        if (size == 1){
            Node* root = pQ[0];
            pQ.pop_back();
            size--;
            return root;
        }
        Node* root = pQ[0];
        pQ[0] = pQ[size - 1];
        pQ.pop_back();
        size--;
        heapify(0);
        return root;
    }
    //heapify doesn't seem to work
    void heapify(int index){
        int left = leftChild(index);
        int right = rightChild(index);

        if(min) {
            int min = index;

            //these don't change??
            if (left < size && pQ[left]->aScore < pQ[min]->aScore) {
                min = left;
            }
            if (right < size && pQ[right]->aScore < pQ[min]->aScore) {
                min = right;
            }
            if (min != index) { //never enters this if statement??
                swap(pQ[index], pQ[min]);
                heapify(min);
            }
        } else {
            int left = leftChild(index);
            int right = rightChild(index);
            int max = index;

            if (left < size && pQ[left]->fScore > pQ[max]->fScore){
                max = left;
            }
            if (right < size && pQ[right]->fScore > pQ[max]->fScore){
                max = right;
            }
            if (max != index){
                swap(pQ[index], pQ[max]);
                heapify(max);
            }
        }
    }

    int parent(int index){
        return (index - 1) / 2;
    }

    int leftChild(int index){
        return (index * 2) + 1;
    }

    int rightChild(int index){
        return (index * 2) + 2;
    }
    int debugScore(Node* node){ //hash function used as debugger
        int score = 0; //stores ascii vals * 2^n
        int power = 1; //power of 2
        for (int i = 0; i < node->data.size(); i++){
            score += power * (node->data[i]);
            power *= 2;
        }
        return score;
    }
public:
    PriorityQ(bool isMin) : min(isMin){
        size = 0; //size of queue
    }
    ~PriorityQ() {
        for (Node* node: pQ) {
            if (node != nullptr) {
                delete node;
            }
        }
        pQ.clear();
    }
    int getSize() {
        return size;
    }
    void insert(string data, string title){
        Node* node  = new Node;
        node->data = data;
        node->title = title;
        if (min) {
            node->aScore = debugScore(node); // debug tactic to make sure the minheap works
        } else {
            node->fScore = stof(data);
        }
        pQ.push_back(node);
        size++; //increment overall size
        int index = pQ.size() - 1;

        if (min) {
            while (index > 0 && pQ[parent(index)]->aScore > pQ[index]->aScore) {
                swap(pQ[index], pQ[parent(index)]);
                index = parent(index);
            }
        } else {
            while (index > 0 && pQ[parent(index)]->fScore < pQ[index]->fScore) {
                swap(pQ[index], pQ[parent(index)]);
                index = parent(index);
            }
        }
    }

    string extractedVal(){
        Node* node = extractMin();
        string title = node->title;
        auto it = find(pQ.begin(), pQ.end(), node);
        if (it != pQ.end()) {
            *it = nullptr;
        }
        delete node;
        return title;
    }
};


#endif //WATERVAPOR_PRIORITYQ_H
