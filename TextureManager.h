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
public:
    static sf::Texture& GetTexture(string textureName){
        if (textures.find(textureName) == textures.end()){
            LoadTexture(textureName);
        }
        return textures[textureName];
    }
};