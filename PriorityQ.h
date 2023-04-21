//
// Created by jayde on 4/20/2023.
//

#ifndef WATERVAPOR_PRIORITYQ_H
#define WATERVAPOR_PRIORITYQ_H

#include <iostream>
#include <vector>

using namespace std;

class PriorityQ {
private:
    struct Node {
        string data;
        int score;
    };

    vector<Node*> pQ;
    int size;

    Node* extractMin(){ //remove the root
        if (size == 1){
            size--;
            return pQ[0];
        }
        Node* root = pQ[0];
        pQ[0] = pQ[size - 1];
        size--;
        heapify(0);
        return root;
    }
    //heapify doesnt seem to work
    void heapify(int index){
        int left = leftChild(index);
        int right = rightChild(index);
        int min = index;

        //these dont change??
        if (left < size && pQ[left]->score < pQ[min]->score){
            min = left;
        } if (right < size && pQ[right]->score < pQ[min]->score){
            min = right;
        }
        if (min != index){ //never enters this if statement??
            swap(pQ[index], pQ[min]);
            heapify(min);
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
    int debugScore(Node* node){ //hash fucntion used as debugger
        int score = 0; //stores ascii vals * 2^n
        int power = 1; //power of 2
        for (int i = 0; i < node->data.size(); i++){
            score += power * (node->data[i]);
            power *= 2;
        }
        return score;
    }
public:
    PriorityQ(){
        size = 0; //size of queue
    }
    void insert(string data){
        Node* node  = new Node;
        node->data = data;
        node->score = debugScore(node); // debug tactic to make sure the minheap works

        pQ.push_back(node);
        size++; //increment overall size
        int index = pQ.size() - 1;


        while (index > 0 && pQ[parent(index)]->score > pQ[index]->score){
            swap(pQ[index], pQ[parent(index)]);
            index = parent(index);
        }
    }

    void extractedVal(){
        Node* node = extractMin();
        cout << node->data << endl;
    }
};


#endif //WATERVAPOR_PRIORITYQ_H
