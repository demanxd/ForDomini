#ifndef PAWN_HPP
#define PAWN_HPP

#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include "Boarddata.hpp"

using namespace sf;

Vector2f toCoord(char a, char b){
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;
    return Vector2f(x* size, y* size);
};


class Pawn{
public:
    Pawn(){};
    virtual ~Pawn(){};

    bool coordinate_checker(Vector2f oldCoord, Vector2f newCoord){
        if ((abs(oldCoord.x - newCoord.x) == size) && (abs(oldCoord.y - newCoord.y) == 0)){
            return true;
        }
        else if ((abs(oldCoord.x - newCoord.x) == 0) && (abs(oldCoord.y - newCoord.y) == size)){
            return true;
        }
        else{
            return false;
        }
    }


    void setTexture(std::string wayToTexture){
        this->_texture.loadFromFile(wayToTexture);
        this->_sprite.setTexture(this->_texture);
    };

    void setPosition(Vector2f coordinate){
        this->_coordinate = coordinate;
        this->_sprite.setPosition(coordinate);
    };

    bool move(std::string str){
        Vector2f oldPos = toCoord(str[0], str[1]);
        Vector2f newPos = toCoord(str[2], str[3]);
        if (this->coordinate_checker(oldPos,newPos)){
            if (boardData.moveAccepted(newPos)){
                this->setPosition(newPos);
                boardData.notifyMove(this->_typePawn, newPos, oldPos);
                return true;
            }
        }
        this->setPosition(oldPos);
        return false;
    };
    Sprite getSprite(){
        return _sprite;
    };

    void setTypePawn(char type){
        this->_typePawn = type;
    }

    char getTypePawn(){
        return this->_typePawn;
    }
    bool canMove(){
        if (this->_typePawn == 1){
            return boardData.checkCanMove(_coordinate);
        }
        return false;
    }
    Vector2f getCoord(){
        return this->_coordinate;
    }

private:
    Vector2f _coordinate;
    Sprite  _sprite;
    Texture _texture;
    char _typePawn = 0;
};


class PlayeblePawn : public Pawn{
public:
    PlayeblePawn() : Pawn(){
        setTexture("images/WhitePawn.png");
        this->setTypePawn(2);
    };
};

class PCPawn : public Pawn{
public:
    PCPawn() : Pawn(){
        setTexture("images/DarkPawn.png");
        this->setTypePawn(1);
    };
};

#endif
