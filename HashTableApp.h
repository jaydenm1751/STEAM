//
// Created by caleb on 4/21/2023.
//

#ifndef WATERVAPOR_HASHTABLEAPP_H
#define WATERVAPOR_HASHTABLEAPP_H

class HashTableApp {
private:
    struct Node {
        string Title;
        string url;
        int ID;
        string iconURL;
        float rating;
        int ratingCount;
        float price;
        float inAppPurchases;
        string Developer;
        string age;
        float size;
        vector<string> genres;
        string releaseDate;
        string updateData;
    };
public:
    HashTableApp() {

    }
};

#endif //WATERVAPOR_HASHTABLEAPP_H
