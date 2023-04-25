#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class TextureManager{
private:
    //static members do not require declaring instances
    //static unordered_map<string, sf::Texture> textures;
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string textureName){
        string path = "files/images/";
        path += textureName + ".png";
        textures[textureName].loadFromFile(path);
    }
//    static void LoadTextureDigits(string textureName, int number){
//        string path = "files/images/" + textureName + ".png";
//        //works for 1
//        textures[textureName + std::to_string(number)].loadFromFile(path, sf::IntRect(5 * number, 0, 50, 50));
//    }
public:
    static sf::Texture& GetTexture(string textureName){
        if (textures.find(textureName) == textures.end()){
            LoadTexture(textureName);
        }
        return textures[textureName];
    }
//    static sf::Texture& GetTextureDigits(string textureName, int number){
//        if (textures.find(textureName + (std::to_string(number))) == textures.end()){
//            LoadTextureDigits(textureName, number);
//        }
//        return textures[textureName + (std::to_string(number))];
//    }
};