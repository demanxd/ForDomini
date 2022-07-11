#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include "Pawn.hpp"
#include "Boarddata.hpp"

using namespace sf;


std::vector<Pawn*> pawns;
std::string position = "";

enum PLAYER_STATE{
    PLAYER,
    AI,
    GAME_OVER
};

PLAYER_STATE state;


void loadPosition(){
    Pawn* pawn;
    for (int i = 0; i < boardData.getSize(); i++)
    {
        for (int j = 0; j < boardData.getSize(); j++)
        {
            if (boardData.getData(i,j) == 0)
            {
                continue;
            }

            if (boardData.getData(i,j) == 1){
                pawn = new PCPawn;
                pawn->setPosition(Vector2f(size * j, size * i));
                pawns.push_back(pawn);

            }
            else if (boardData.getData(i,j) == 2){
                pawn = new PlayeblePawn;
                pawn->setPosition(Vector2f(size * j, size * i));
                pawns.push_back(pawn);
            }
        }
    }
};

void playerMove(Event* e, Vector2i* pos, bool* isMove,
                Vector2f *oldPos, Vector2f *newPos,
                int *n,float *dx,float *dy,std::string *str){
    //drag & drop
    if (e->type == Event::MouseButtonPressed)
    {
        if (e->key.code == Mouse::Left)
        {
            for (int i = 0; i < pawns.size(); i++)
            {
                if (pawns.at(i)->getSprite().getGlobalBounds().contains(pos->x,pos->y)
                        && pawns.at(i)->getTypePawn() == 2)
                {
                    *isMove = true; *n = i;
                    *dx = pos->x - pawns.at(i)->getSprite().getPosition().x;
                    *dy = pos->y - pawns.at(i)->getSprite().getPosition().y;
                    *oldPos = pawns.at(i)->getSprite().getPosition();
                }
            }
        }
    }

    if (e->type == Event::MouseButtonReleased)
    {
        if (e->key.code == Mouse::Left)
        {
            if (*isMove == true){
                *isMove = false;
                Vector2f p = pawns.at(*n)->getSprite().getPosition() + Vector2f(size/2,size/2);
                *newPos = Vector2f(size*int(p.x/size),size*int(p.y/size));
                *str = toChessNote(*oldPos) + toChessNote(*newPos);
                std::cout << *str << std::endl;
                pawns.at(*n)->move(*str);
                if (*newPos != *oldPos){
                    if (boardData.checkGameEnd() == 0){
                        state = PLAYER_STATE::AI;
                    }
                    else{
                        state = PLAYER_STATE::GAME_OVER;
                    }
                }
            }

        }
    }
    if (*isMove)
    {
        pawns.at(*n)->setPosition(Vector2f(pos->x - *dx, pos->y - *dy));
    }
}

std::string randomDirection(Vector2f oldPos){
    Vector2f tmp;
    tmp.x = oldPos.x / size;
    tmp.y = oldPos.y / size;
    Vector2f tmpO = tmp;

    while (tmpO == tmp)
    {
        tmp.x += (1 - (rand() % 3));
        tmp.y += (1 - (rand() % 3));
        std::cout << "randomizer! x = " << tmp.x << "; y = " << tmp.y << std::endl;
        std::cout << "randomizer0! x = " << tmpO.x << "; y = " << tmpO.y << std::endl;

    }

    Vector2f newPos;
    newPos.x = tmp.x * size;
    newPos.y = tmp.y * size;

    std::string str = toChessNote(oldPos) + toChessNote(newPos);

    return str;
}

void AIMove(){
    bool moved = false;
    int i;
    while (!moved){
        i = rand()%((pawns.size() /2));
        if (pawns.at(i)->canMove())
        {
            while (!moved)
            {
                if (pawns.at(i)->move( randomDirection( pawns.at(i)->getCoord() ) ))
                {
                    moved = true;
                }
            }
        }
    }
    if (boardData.checkGameEnd() == 0){
        state = PLAYER_STATE::PLAYER;
    }
    else{
        state = PLAYER_STATE::GAME_OVER;
    }
}

std::string winLoseIcon(){
    if (boardData.checkGameEnd() == 1){
        std::cout << "Lose" <<std::endl;
        return "images/Lose.png";
    }
    else{
        std::cout << "Win" <<std::endl;
        return "images/Win.png";
    }
}


int main() {
    RenderWindow window(VideoMode(453,454),"The Game");

    Texture final_S;
    Sprite finalS;
    Vector2i pos;
    loadPosition();
    srand(time(0));
    state = PLAYER_STATE::PLAYER;

    bool isMove = false;
    float dx = 0, dy = 0;
    Vector2f oldPos,newPos;
    std::string str;
    int n = 0;


    while (window.isOpen()){
        pos = Mouse::getPosition(window);

        Event e;
        while (window.pollEvent(e)){
            if (e.type == Event::Closed){
                window.close();
            }
            pos = Mouse::getPosition(window);

            switch (state){
            case PLAYER_STATE::PLAYER:
                playerMove(&e, &pos, &isMove, &oldPos, &newPos,
                    &n, &dx, &dy, &str);
                break;
            case PLAYER_STATE::AI:
                AIMove();
                break;
            case PLAYER_STATE::GAME_OVER:
                final_S.loadFromFile(winLoseIcon());
                finalS.setTexture(final_S);
                break;
            }
        }

        //draw
        window.clear();
        window.draw(boardData.getSprite());
        for (int i = 0; i < pawns.size(); i++){
            window.draw(pawns.at(i)->getSprite());
        }
        window.draw(finalS);

        window.display();
    }
}
