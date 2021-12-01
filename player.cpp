#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtWidgets>
#include "game.h"
#include "mainwindow.h"
#include "player.h"

Player::Player(int x, int y, int points, int health, int attack, QColor color, std::string name){
    // set starting position
    set_pos_x(x);
    set_pos_y(y);

    name_ = name;

    // set player stats
    points_ = points;
    health_ = health;
    attack_ = attack;
    Pcolor_ = color;

}

void Player::move_up(){
    pos_.y--;
    //emit moveUp(this);
}
void Player::move_down(){
    pos_.y++;
}
void Player::move_left(){
    pos_.x--;
}
void Player::move_right(){
    pos_.x++;
}
