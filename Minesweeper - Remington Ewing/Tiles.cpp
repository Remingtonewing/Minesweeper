#pragma once
#include "Tiles.h"
void Tiles::NeighborNum(int _numNeighbors) {
    numNeighbors = _numNeighbors;
}
Tiles::Tiles() {
    revealed = false;
}
Sprite& Tiles::getRevealS() {
    return sprites[0];
}
Sprite& Tiles::getMineS() {
    return sprites[1];
}
Sprite& Tiles::getNumberS() {
    return sprites[2];
}
Sprite& Tiles::getHiddenTileS() {
    return sprites[3];
}
Sprite& Tiles::getFlagS() {
    return sprites[4];
}
void Tiles::FlagT(Texture &flagTexture) {
    sprites[4].setTexture(flagTexture);
}
void Tiles::HiddenT(Texture &hiddenTexture) {
    sprites[3].setTexture(hiddenTexture);
}
void Tiles::NumT(Texture &numTexture) {
    sprites[2].setTexture(numTexture);
}
void Tiles::MineT(Texture &mineTexture) {
    sprites[1].setTexture(mineTexture);
}
void Tiles::RevealT(Texture &revealTexture) {
    sprites[0].setTexture(revealTexture);
}