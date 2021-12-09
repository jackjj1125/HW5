#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QColor>
#include <QGraphicsItem>
#include "enemy.h"

Enemy::Enemy(int h, int a, int p){
    health_ = h;
    attack_ = a;
    points_ = p;
}

