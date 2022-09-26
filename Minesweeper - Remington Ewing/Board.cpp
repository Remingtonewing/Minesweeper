#pragma once
#include "Board.h"
#include <fstream>
using namespace std;
int main() {
    int boardHeight = Board::allColumns() * 32 + 100;
    int boardWidth = Board::allRows() * 32;
    RenderWindow window(sf::VideoMode(boardWidth, boardHeight), "Minesweeper");
    Board board;

    bool gameOn = true;
    while (gameOn) {
        window.clear();
        window.clear(sf::Color(255,255,255,255)); //Paints background white
        board.updateBoard(window);
        window.display();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                gameOn = false;
            }
            if (event.type == Event::MouseButtonPressed) { //Fixes drag reveal
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    Vector2i localPosition = Mouse::getPosition(window); //
                    board.Mouse(0,localPosition,window);
                }
                if (Mouse::isButtonPressed(Mouse::Right)) {
                    Vector2i positionA = Mouse::getPosition(window);
                    board.Mouse(1,positionA, window);
                }
            }
        }
    }
}

int Board::allRows() {
    ifstream boardDim("boards/config.cfg");
    string spacer;
    getline(boardDim, spacer);
    int boardRows = stoi(spacer);
    return boardRows;
}
int Board::allColumns() {
    ifstream boardDim("boards/config.cfg");
    string spacer;
    getline(boardDim, spacer);
    int boardRows = stoi(spacer);
    getline(boardDim, spacer);
    int boardColumns = stoi(spacer);
    return boardColumns;
}
int Board::totalBombs() {
    ifstream boardDim("boards/config.cfg");
    string spacer;
    getline(boardDim, spacer);
    int boardRows = stoi(spacer);
    getline(boardDim, spacer);
    int boardColumns = stoi(spacer);
    getline(boardDim, spacer);
    int bombs = stoi(spacer);
    return bombs;
}
vector<Texture> Board::LoadT() {
    vector<Texture> textureArr;

    Texture revealed;
    revealed.loadFromFile("images\\tile_revealed.png");
    Texture hidden;
    hidden.loadFromFile("images\\tile_hidden.png");
    Texture win;
    win.loadFromFile("images\\face_win.png");
    Texture lose;
    lose.loadFromFile("images\\face_lose.png");
    Texture face;
    face.loadFromFile("images\\face_happy.png");
    Texture test1;
    test1.loadFromFile("images\\test_1.png");
    Texture test2;
    test2.loadFromFile("images\\test_2.png");
    Texture test3;
    test3.loadFromFile("images\\test_3.png");
    Texture debug;
    debug.loadFromFile("images\\debug.png");
    Texture mine;
    mine.loadFromFile("images\\mine.png");
    Texture flag;
    flag.loadFromFile("images\\flag.png");
    Texture flagCount;
    flagCount.loadFromFile("images\\digits.png");
    Texture num1;
    num1.loadFromFile("images\\number_1.png");
    Texture num2;
    num2.loadFromFile("images\\number_2.png");
    Texture num3;
    num3.loadFromFile("images\\number_3.png");
    Texture num4;
    num4.loadFromFile("images\\number_4.png");
    Texture num5;
    num5.loadFromFile("images\\number_5.png");
    Texture num6;
    num6.loadFromFile("images\\number_6.png");
    Texture num7;
    num7.loadFromFile("images\\number_7.png");
    Texture num8;
    num8.loadFromFile("images\\number_8.png");

    textureArr.push_back(face); //0
    textureArr.push_back(num1); //1
    textureArr.push_back(num2);//2
    textureArr.push_back(num3);//3
    textureArr.push_back(num4);//4
    textureArr.push_back(num5);//5
    textureArr.push_back(num6);//6
    textureArr.push_back(num7);//7
    textureArr.push_back(num8);//8
    textureArr.push_back(win);//9
    textureArr.push_back(debug);//10
    textureArr.push_back(flagCount);//11
    textureArr.push_back(flag);//12
    textureArr.push_back(mine);//13
    textureArr.push_back(lose);//14
    textureArr.push_back(revealed); //15
    textureArr.push_back(hidden); //16
    textureArr.push_back(test1); //17
    textureArr.push_back(test2); //18
    textureArr.push_back(test3); //19

    return textureArr;
}
Board::Board() {

    gameCondition = true;
    textures = LoadT();

    for (int i = 0; i < allRows(); i++) {
        for(int j = 0; j < allColumns(); j++) {
            tiles[i][j].RevealT(textures.at(15));
            tiles[i][j].MineT(textures.at(13));
            tiles[i][j].HiddenT(textures.at(16));
            tiles[i][j].FlagT(textures.at(12));
            tiles[i][j].getFlagS().setPosition(32.f * i, 32.f * j);
            tiles[i][j].getHiddenTileS().setPosition(32.f * i, 32.f * j);
            tiles[i][j].getMineS().setPosition(32.f * i, 32.f * j);
            tiles[i][j].getRevealS().setPosition(32.f * i, 32.f * j);
        }
    }
    ones.setTexture(textures.at(11));
    ones.setPosition(63, allColumns() *32);
    tenths.setTexture(textures.at(11));
    tenths.setPosition(42, allColumns() *32);
    hundredths.setTexture(textures.at(11));
    hundredths.setPosition(21, allColumns() *32);
    thousandths.setTexture(textures.at(11));
    thousandths.setPosition(0, allColumns() *32);
    baseFace.setTexture(textures.at(0));
    baseFace.setPosition(((float)allRows() * 16 - 17), (float)allColumns() * 32);
    debugMode.setTexture(textures.at(10));
    debugMode.setPosition((float)(allRows() - 9) * 32, (float)allColumns() * 32);
    Test1.setTexture(textures.at(17));
    Test1.setPosition(((float)allRows() - 7) * 32, (float)allColumns() * 32);
    Test2.setTexture(textures.at(18));
    Test2.setPosition(((float)allRows() - 5) * 32, (float)allColumns() * 32);
    Test3.setTexture(textures.at(19));
    Test3.setPosition(((float)allRows() - 3) * 32, (float)allColumns() * 32);

    iterCount = 0;
    mines = totalBombs();
    flags = mines;

    Board::setBombs();
    Board::setBoard();
}
void Board::updateBoard(RenderWindow &window) {
    if (boardCondition()) {
        for (int i = 0; i < allRows();i++) {
            for (int j = 0; j < allColumns();j++) {
                if (tiles[i][j].mineCondition) {
                    window.draw(tiles[i][j].getHiddenTileS());
                    window.draw(tiles[i][j].getFlagS());
                } else if (tiles[i][j].revealed) {
                    window.draw(tiles[i][j].getRevealS());
                    if (nearMines(i,j) > 0) {
                        window.draw(tiles[i][j].getNumberS());
                    }
                }
            }
        }
        baseFace.setTexture(textures.at(9));
    } else if (!gameCondition) {
        for (int i = 0; i < allRows();i++) {
            for (int j = 0; j < allColumns();j++) {
                if (tiles[i][j].mineCondition) {
                    window.draw(tiles[i][j].getRevealS());
                    window.draw(tiles[i][j].getMineS());
                } else if (tiles[i][j].revealed) {
                    window.draw(tiles[i][j].getRevealS());
                    if (nearMines(i,j) > 0) {
                        window.draw(tiles[i][j].getNumberS());
                    }
                } else if (!tiles[i][j].revealed) {
                    window.draw(tiles[i][j].getHiddenTileS());
                    if (tiles[i][j].placedFlag) {
                        window.draw(tiles[i][j].getFlagS());
                    }
                }
            }
        }
        baseFace.setTexture(textures.at(14));
    } else if (gameCondition) {
        for (int i = 0; i < allRows();i++) {
            for (int j = 0; j < allColumns();j++) {
                if (!tiles[i][j].revealed) {
                    window.draw(tiles[i][j].getHiddenTileS());
                } if (tiles[i][j].placedFlag) {
                    window.draw(tiles[i][j].getFlagS());
                } if (tiles[i][j].debugger) {
                    if (tiles[i][j].mineCondition) {
                        window.draw(tiles[i][j].getMineS());
                    }
                } if (tiles[i][j].revealed) {
                    window.draw(tiles[i][j].getRevealS());
                    if (nearMines(i,j) > 0) {
                        window.draw(tiles[i][j].getNumberS());
                    }
                }
            }
        }
    }
    int a;
    int b;

    if (flags >= 0) {
        thousandths.setTextureRect(IntRect((10 * 21), 0, 21, 32));
        hundredths.setTextureRect(IntRect(((flags / 100) * 21), 0, 21, 32));
        tenths.setTextureRect(IntRect((((flags / 10) % 10) * 21), 0, 21, 32));
        ones.setTextureRect(IntRect(((flags  % 10) * 21), 0, 21, 32));
        window.draw(hundredths);
        window.draw(tenths);
        window.draw(ones);
    } else {
        if (flags < 0) {
            window.draw(thousandths);
        }
        a = -1 * flags;
        b = (a % 10);
        thousandths.setTextureRect(IntRect((10 * 21), 0, 21, 32));
        hundredths.setTextureRect(IntRect((-(flags / 100) * 21), 0, 21, 32));
        tenths.setTextureRect(IntRect((((a / 10) % 10) * 21), 0, 21, 32));
        ones.setTextureRect(IntRect(((b  % 10) * 21), 0, 21, 32));
        window.draw(hundredths);
        window.draw(tenths);
        window.draw(ones);

    }
    window.draw(baseFace);
    window.draw(Test1);
    window.draw(Test2);
    window.draw(Test3);
    window.draw(debugMode);

}
void Board::Mouse(int click, Vector2i vector, RenderWindow &window) {
    if (gameCondition) {
        if (vector.y < (allColumns() * 32)) {
            int posX = (vector.x / 32);
            int posY = (vector.y / 32);

            if (click == 0) {
                if (tiles[posX][posY].placedFlag == false) {
                    if (tiles[posX][posY].mineCondition) {
                        gameCondition = false;
                    } else {
                        recursiveBoardR(posX, posY);
                    }
                }
            } else if (click == 1) {
                if (!tiles[posX][posY].revealed) {
                    if (!tiles[posX][posY].placedFlag) {
                        tiles[posX][posY].placedFlag = true;
                        flags--;
                    } else {
                        tiles[posX][posY].placedFlag = false;
                        flags++;
                    }
                }
            }
        } else if (vector.y > (allColumns() * 32) && vector.y < allColumns() * 32 + 64) {
            if (vector.x >= ((float) allRows() * 16) - 17 && vector.x < ((float) allRows() * 16) + 47) {
                Board::ResetBoard();
            } else if (vector.x >= ((float) allRows() - 9) * 32 && vector.x < ((float) allRows() - 9) * 32 + 64) {
                Board::DebugMode(window);
            } else if (vector.x >= ((float) allRows() - 7) * 32 && vector.x < ((float) allRows() - 7) * 32 + 64) {
               Board::loadBoardOne();
            } else if (vector.x >= (float) (allRows() - 5) * 32 && vector.x < ((float) allRows() - 5) * 32 + 64) {
                Board::loadBoardTwo();
            } else if (vector.x >= (float)(allRows() - 3) * 32 && vector.x < (float)(allRows() - 3) * 32 + 64) {
                Board::loadBoardThree();
            }
        }

    }if (Board::boardCondition()) {
        baseFace.setTexture(textures.at(9));
    }else if (!gameCondition) {
        if (vector.x >= ((float)allRows() * 16) - 17 && vector.x < ((float)allRows() * 16) + 47 && vector.y > (allColumns() * 32) && vector.y < allColumns() * 32 + 64) {
            Board::ResetBoard();
        } else if (vector.x >= ((float) allRows() - 9) * 32 && vector.x < ((float) allRows() - 9) * 32 + 64 && vector.y > (allColumns() * 32) && vector.y < allColumns() * 32 + 64) {
            Board::DebugMode(window);
        } else if (vector.x >= ((float) allRows() - 7) * 32 && vector.x < ((float) allRows() - 7) * 32 + 64 && vector.y > (allColumns() * 32) && vector.y < allColumns() * 32 + 64) {
            Board::loadBoardOne();
        } else if (vector.x >= (float) (allRows() - 5) * 32 && vector.x < ((float) allRows() - 5) * 32 + 64 && vector.y > (allColumns() * 32) && vector.y < allColumns() * 32 + 64) {
            Board::loadBoardTwo();
        } else if (vector.x >= (float)(allRows() - 3) * 32 && vector.x < (float)(allRows() - 3) * 32 + 64 && vector.y > (allColumns() * 32) && vector.y < allColumns() * 32 + 64) {
            Board::loadBoardThree();
        }
    }
}
int Board::nearMines(int row, int column) {
    numNeighbors = 0;

    if (row == 0 && column == 0) //
        {
            if (tiles[row + 1][column].mineCondition)
            {
                numNeighbors++;
            }
            if (tiles[row][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column + 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else if (row == 0 && column == allColumns() - 1)
        {
            if (tiles[row++][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column - 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else if (row == allRows() - 1 && column == 0)
        {
            if (tiles[row - 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column + 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else if (row == allRows() - 1 && column == allColumns() - 1)
        {
            if (tiles[row - 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column - 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else if (row == 0)
        {
            if (tiles[row + 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column + 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else if (column == 0) //
        {
            if (tiles[row + 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column + 1].mineCondition )
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column + 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else if (row == allRows() - 1)
        {
            if (tiles[row - 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column + 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else if (column == allColumns() - 1)
        {
            if (tiles[row + 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column - 1].mineCondition)
            {
               numNeighbors++;
            }
        }
        else
        {
            if (tiles[row + 1][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column + 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row + 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row][column - 1].mineCondition)
            {
               numNeighbors++;
            }
            if (tiles[row - 1][column - 1].mineCondition)
            {
               numNeighbors++;
            }
        }
    return numNeighbors;
}
void Board::Clear() { //Clears the board (for test 1, 2, 3)
    mines = 0;
    flags = mines;
    for (int i = 0; i < allRows(); i++) {
        for (int j = 0; j < allColumns(); j++)
        {
            tiles[i][j].mineCondition = false;
            tiles[i][j].placedFlag = false;
            tiles[i][j].debugger = false;
            tiles[i][j].revealed = false;
            tiles[i][j].NeighborNum(0);
        }
    }
}
void Board::setBoard() { //Sets the board
    for (int i = 0; i < allRows(); i++) {
        for (int j = 0; j < allColumns(); j++) {
            if (nearMines(i, j) > 0) {
                tiles[i][j].NeighborNum(nearMines(i, j));
                tiles[i][j].NumT(textures[nearMines(i, j)]);
                tiles[i][j].getNumberS().setPosition((float) 32 * (float) i, (float) 32 * float(j));
            }
        }
    }
}
bool Board::boardCondition() { //Checks for win / lose condition
    iterCount = 0;

    for (int i = 0; i < allRows(); i++) {
        for (int j = 0; j < allColumns(); j++) {
            if (tiles[i][j].revealed) {
                if (!tiles[i][j].mineCondition) {
                    iterCount++;
                }
            }

        }
    }
    if ((allRows() * allColumns() - mines - iterCount == 0)) {
        flags = 0;
        return true;
    } else {
        return false;
    }
}
void Board::ResetBoard() { //Resets entire board to base (starting)
    gameCondition = true;
    Clear();
    mines = totalBombs();
    flags = mines;
    iterCount = 0;

    Board::setBombs();
    Board::setBoard();

    for (int i = 0; i < allRows(); i++) {
        for (int j = 0; j < allColumns(); j++) {
            tiles[i][j].revealed = false;
        }
    }
    baseFace.setTexture(textures.at(0));
}
void Board::DebugMode(RenderWindow &window) { //Enters debugger mode (shows mines)
    for (int i = 0; i < allRows(); i++) {
        for (int j = 0; j < allColumns(); j++) {
            if (!tiles[i][j].debugger) {
                tiles[i][j].debugger = true;
            } else {
                tiles[i][j].debugger = false;
            }
        }
    }
}
void Board::recursiveBoardR(int row, int column) {
    tiles[row][column].revealed = true;
    if (nearMines(row,column) == 0) {
        if (column != allColumns() - 1 && !tiles[row][column + 1].revealed && !tiles[row][column + 1].placedFlag) {
            recursiveBoardR(row,column + 1);
        } if (row != allRows() - 1 && !tiles[row + 1][column].revealed && !tiles[row + 1][column].placedFlag) {
            recursiveBoardR(row + 1,column);
        } if (column != 0 && !tiles[row][column - 1].revealed && !tiles[row][column - 1].placedFlag) {
            recursiveBoardR(row, column - 1);
        } if (row != 0 && !tiles[row - 1][column].revealed && !tiles[row - 1][column].placedFlag) {
            recursiveBoardR(row - 1, column);
        } if ((row != allRows() - 1 && column != allColumns() - 1) && !tiles[row + 1][column + 1].revealed && !tiles[row + 1][column + 1].placedFlag) {
            recursiveBoardR(row + 1, column + 1);
        } if (row != 0 && column != 0 && !tiles[row - 1][column - 1].revealed && !tiles[row - 1][column - 1].placedFlag) {
            recursiveBoardR(row-1,column-1);
        } if ((row != allRows() - 1 && column != 0) && !tiles[row + 1][column - 1].revealed && !tiles[row + 1][column - 1].placedFlag) {
            recursiveBoardR(row + 1, column - 1);
        }if ((row != 0 && column != allColumns() - 1) && !tiles[row - 1][column + 1].revealed && !tiles[row - 1][column + 1].placedFlag) {
            recursiveBoardR(row - 1, column + 1);
        }
    }
}
void Board::loadBoardOne() {
    gameCondition = true;
    Clear();
    int yAxis = 0;
    ifstream inFile3("boards/testboard1.brd");
    string boardNum;
    while (getline(inFile3, boardNum)) {
        for (size_t i = 0; i < boardNum.length(); i++) { //https://en.cppreference.com/w/cpp/types/size_t
            if (boardNum.at(i) == '1') {
                tiles[i][yAxis].mineCondition = true;
                mines++;
            }
        }
        yAxis++;
    }
    flags = mines;
    setBoard();
    baseFace.setTexture(textures.at(0));
}
void Board::loadBoardTwo() {
    gameCondition = true;
    Clear();
    int yAxis = 0;
    ifstream inFile3("boards/testboard2.brd");
    string boardNum;
    while (getline(inFile3, boardNum)) {
        for (size_t i = 0; i < boardNum.length(); i++) {
            if (boardNum.at(i) == '1') {
                tiles[i][yAxis].mineCondition = true;
                mines++;
            }
        }
        yAxis++;
    }
    flags = mines;
    setBoard();
    baseFace.setTexture(textures.at(0));
}
void Board::loadBoardThree() {
    gameCondition = true;
    Clear();
    int yAxis = 0;
    ifstream inFile3("boards/testboard3.brd");
    string boardNum;
    while (getline(inFile3, boardNum)) {
        for (size_t i = 0; i < boardNum.length(); i++) {
            if (boardNum.at(i) == '1') {
                tiles[i][yAxis].mineCondition = true;
                mines++;
            }
        }
        yAxis++;
    }
    flags = mines;
    setBoard();
    baseFace.setTexture(textures.at(0));
}
void Board::setBombs() {
    while (iterCount < totalBombs()) {
        int rand1 = rand() % allRows();
        int rand2 = rand() % allColumns();
        if (tiles[rand1][rand2].mineCondition != true) {
            tiles[rand1][rand2].mineCondition = true;
            iterCount++;
        }
    }
}