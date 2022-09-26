#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>

using namespace sf;
using namespace std;

struct Tiles
{
    Tiles();
    void NumT(Texture&);
    void HiddenT(Texture&);
    void RevealT(Texture&);
    void MineT(Texture&);
    void FlagT(Texture&);
    void NeighborNum(int);
    Sprite& getRevealS();
    Sprite& getMineS();
    Sprite& getNumberS();
    Sprite& getHiddenTileS();
    Sprite& getFlagS();

    bool mineCondition;
    bool placedFlag;
    bool revealed;
    bool debugger;
    int numNeighbors;
    Sprite sprites[5];
};