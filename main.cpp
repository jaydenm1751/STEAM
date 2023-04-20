#include <iostream>
#include <SFML/Graphics.hpp>
#include "HashTable.h"
#include "StorePage.h"

using namespace std;

int main() {
    HashTable map;

    map.insert("Caleb");
    map.insert("Jayden");
    map.insert("Maxwell");

    map.search("Caleb");
    map.search("aCelb");

    StorePage Steam;
    sf::RenderWindow window(sf::VideoMode(Steam.width, Steam.height), "WaterVapor");

    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}
