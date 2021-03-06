#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtWidgets>
#include "game.h"
#include "mainwindow.h"
#include "player.h"

// ======================= GAME CLASS =======================
game::game(int x, int y, int width, int height, QColor color){
    color_ = color;
    x_ = x * width;
    y_ = y * height;
    width_ = width;
    height_ = height;
    is_obstical_ = false;
    is_castle_ = false;
    is_bridge_ = false;
    is_enemy_ = false;
    is_player_ = false;
    i_ = y;
    j_ = x;
    is_ATK_ = false;
    is_HEALTH_ = false;
    is_PTS_ = false;

}


QRectF game::boundingRect() const {
    return QRectF(x_, y_, width_, width_);
}

QPainterPath game::shape() const {
    QPainterPath path;
    path.addRect(x_, y_, width_, width_);
    return path;
}

void game::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
}

void game::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void game::removeDefeatedEnemy(){
    qDebug() << "entered game::removeDefeatedEnemy";

    this->set_obstical(false);
    QColor color(255,255,255);
    this->set_Color(color);
    this->set_enemy(false);
    emit removeEnemySignal(this);
    update();
}

void game::attackPlayer(Player * p, int h){
    p->decrease_health(h);
    emit attackSignal(this);
    update();
}

void game::sendMessage(){
    emit messageSignal(this);
    update();
}

void game::resetPrevCell(){
    this->set_player_status(false);
    if(is_bridge_){
        QColor color(165,42,42);
        this->set_Color(color);
    }
    else{
        QColor color(255,255,255);
        this->set_Color(color);
    }
    emit resetPrevSignal(this);
    update();
}

void game::movePlayerUp(Player * p){
    // move player up
    p->move_up();
    this->set_player_status(true);
    QColor pColor(1,1,1);
    this->set_Color(pColor);

    emit moveUp(this);
    update();
}

void game::movePlayerDown(Player * p){
    p->move_down();
    this->set_player_status(true);
    QColor pColor(1,1,1);
    this->set_Color(pColor);

    emit moveDown(this);
    update();
}
void game::movePlayerLeft(Player * p){
    p->move_left();
    this->set_player_status(true);
    QColor pColor(1,1,1);
    this->set_Color(pColor);

    emit moveLeft(this);
    update();
}
void game::movePlayerRight(Player * p){
    p->move_right();
    this->set_player_status(true);
    QColor pColor(1,1,1);
    this->set_Color(pColor);

    emit moveRight(this);
    update();
}
