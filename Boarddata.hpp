#ifndef BOARDDATA_HPP
#define BOARDDATA_HPP

#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>

using namespace sf;

int size = 56;



std::string toChessNote(Vector2f p){
    std::string s = "";
    s += char((p.x / size) + 97);
    s += char(7 - (p.y / size) + 49);
    return s;
};


class BoardData{
public:
    BoardData(){
        this->_boardTexture.loadFromFile("images/board0.png");
        this->_sprite.setTexture(this->_boardTexture);
        char b[8][8] = {1, 1, 1, 0, 0, 0, 0, 0, //0
                        1, 1, 1, 0, 0, 0, 0, 0, //1
                        1, 1, 1, 0, 0, 0, 0, 0, //2
                        0, 0, 0, 0, 0, 0, 0, 0, //3
                        0, 0, 0, 0, 0, 0, 0, 0, //4
                        0, 0, 0, 0, 0, 2, 2, 2, //5
                        0, 0, 0, 0, 0, 2, 2, 2, //6
                        0, 0, 0, 0, 0, 2, 2, 2, //7
                       };
        this->_board = (char *) malloc( 64 + 1 );
        memcpy( this->_board, b, 64);
    }
    ~BoardData(){
        free(this->_board);
    }
    char getData(int i, int j){
        return this->_board[i + j * this->_sizeBoard];
    }
    Sprite getSprite(){
        return this->_sprite;
    }
    int getSize(){
        return this->_sizeBoard;
    }
    bool moveAccepted(Vector2f coordinate){
        Vector2f tmp;
        tmp.x = coordinate.x / size;
        tmp.y = coordinate.y / size;
        if ((tmp.x > -1)&&(tmp.x < this->_sizeBoard)){
            if ((tmp.y > -1)&&(tmp.y < this->_sizeBoard)){
                if(this->_board[int(tmp.x + tmp.y * this->_sizeBoard)] == 0){
                    return true;
                }
            }
        }
        return false;
    }
    void notifyMove(char typePawn, Vector2f newPos, Vector2f oldPos){
        Vector2f tmp;
        tmp.x = newPos.x / size;
        tmp.y = newPos.y / size;
        this->_board[int(tmp.x + tmp.y * this->_sizeBoard)] = typePawn;
        tmp.x = oldPos.x / size;
        tmp.y = oldPos.y / size;
        this->_board[int(tmp.x + tmp.y * this->_sizeBoard)] = 0;
    }
    char checkGameEnd(){
        bool answer = true;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if ( this->_board[i + j * this->_sizeBoard] != 2){
                    answer = false;
                    break;
                }
            }
            if (answer == false){
                break;
            }
        }
        if (answer == true){
            return 2;//player win
        }
        answer = true;
        for (int i = 5; i < 8; i++){
            for (int j = 5; j < 8; j++){
                if ( this->_board[i + j * this->_sizeBoard] != 1){
                    answer = false;
                    break;
                }
            }
            if (answer == false){
                break;
            }
        }
        if (answer == true){
            return 1;//pc win
        }
        return 0;
    }
    bool checkCanMove(Vector2f coord){
        Vector2f tmp;
        tmp.x = coord.x / size;
        tmp.y = coord.y / size;
        bool answer = false;
        if (((tmp.x - 1) >= 0)&&
            (this->_board[int((tmp.x - 1) + tmp.y * (this->_sizeBoard))] == 0)){
            return true;
        }
        else if (((tmp.x + 1) <= (this->_sizeBoard - 1))&&
                        (this->_board[int((tmp.x + 1) + tmp.y * (this->_sizeBoard))] == 0)){
            return true;//
        }
        if (((tmp.y - 1) >= 0)&&
                        (this->_board[int(tmp.x + (tmp.y + 1) * (this->_sizeBoard))] == 0)){
            return true;
        }
        else if (((tmp.y + 1) <= (this->_sizeBoard - 1))&&
                        (this->_board[int(tmp.x + (tmp.y + 1) * (this->_sizeBoard))] == 0)){
            return true;
        }
        else {
            return false;
        }
    }

private:
    Texture _boardTexture;
    Sprite _sprite;
    char* _board;
    int _sizeBoard = 8;
};

BoardData boardData;
#endif
