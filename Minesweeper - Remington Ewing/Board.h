#pragma once
#include "Tiles.h"

struct Board {
    Board();
    bool boardCondition();
    vector<Texture> LoadT();
    vector<Texture> textures;
    void updateBoard(RenderWindow& window);
    void Mouse(int click, Vector2i vector, RenderWindow& window);
    int totalBombs();
    static int allColumns();
    static int allRows();
    void ResetBoard();
    void recursiveBoardR(int row, int column);
    void DebugMode(RenderWindow& window);
    int nearMines(int row, int column);
    void Clear();
    void setBoard();
    void loadBoardOne();
    void loadBoardTwo();
    void loadBoardThree();
    void setBombs();

private:
    int flags;
    int mines;
    int numNeighbors;
    unsigned int iterCount;
    Sprite baseFace;
    Sprite debugMode;
    Sprite thousandths;
    Sprite hundredths;
    Sprite tenths;
    Sprite ones;
    Sprite Test1;
    Sprite Test2;
    Sprite Test3;
    bool gameCondition;
    Tiles tiles[35][35];
};