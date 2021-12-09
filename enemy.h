#ifndef ENEMY_H
#define ENEMY_H

#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QColor>
#include <QGraphicsItem>
#include "player.h"

class Enemy: public QObject {
public:
    Enemy(int h, int a, int p);
    void set_pos(int i, int j) { i_ = i; j_ = j; };
    int get_i() { return i_; };
    int get_j() { return j_; };
    void set_health(int h) { health_ = h; };
    int get_health() { return health_;};
    int get_attack() { return attack_;};
    int get_points() { return points_;};
    void decrease_health(int h) { health_ -= h; };

private:
    int health_;
    int attack_;
    int points_;

    //enemy position
    int i_;
    int j_;
};

#endif // ENEMY_H
