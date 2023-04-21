//
// Created by caleb on 4/21/2023.
//

#ifndef WATERVAPOR_HASHTABLEAPP_H
#define WATERVAPOR_HASHTABLEAPP_H

class HashTableApp {
private:
    struct Node {
        string Title;
        int players{};
        bool online{};
        vector<string> genres;
        string publishers;
        int review{};
        float sales{};
        float price{};
        string console;
        char rating{};
        int release{};
        float allPlayStyle{};
        vector<float> completionists;
        vector<float> storyDLC;
        vector<float> story;
    };
public:
    HashTableApp() {

    }
};

#endif //WATERVAPOR_HASHTABLEAPP_H
