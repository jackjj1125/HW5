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



class Enemy {
public:
     Enemy(int h, int a, int p);
     void set_pos(int i, int j) { i_ = i; j_ = j; };
     int get_i() { return i_; };
     int get_j() { return j_; };
     void set_health(int h) { health_ = h; };
     void set_attack(int h) { attack_ = h; };
     void set_points(int h) { points_ = h; };
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


//class Enemy {
//public:
//    virtual ~Enemy() = default;
//    virtual Enemy* clone() = 0;

//    Enemy(int h, int a, int p);
//    void set_pos(int i, int j) { i_ = i; j_ = j; };
//    int get_i() { return i_; };
//    int get_j() { return j_; };
//    void set_health(int h) { health_ = h; };
//    void set_attack(int h) { attack_ = h; };
//    void set_points(int h) { points_ = h; };
//    int get_health() { return health_;};
//    int get_attack() { return attack_;};
//    int get_points() { return points_;};
//    void decrease_health(int h) { health_ -= h; };

//private:
//    int health_;
//    int attack_;
//    int points_;

//    //enemy position
//    int i_;
//    int j_;
//};

//class Bandit: public Enemy {
//public:
//    Bandit(int h, int a, int p)
//        : health_(h),
//          attack_(a),
//          points_(p)
//    {}
//    Enemy* clone() { return new Bandit(20, 20, 40); };

//    void set_pos(int i, int j) { i_ = i; j_ = j; };
//    int get_i() { return i_; };
//    int get_j() { return j_; };
//    void set_health(int h) { health_ = h; };
//    void set_attack(int h) { attack_ = h; };
//    void set_points(int h) { points_ = h; };
//    int get_health() { return health_;};
//    int get_attack() { return attack_;};
//    int get_points() { return points_;};
//    void decrease_health(int h) { health_ -= h; };

//private:
//    int health_;
//    int attack_;
//    int points_;

//    //enemy position
//    int i_;
//    int j_;
//};

//class Spawner{
//public:
////    Spawner(Bandit* prototype)
////    : prototype_(prototype)
////    {}

//    static Enemy* spawnEnemy();

//private:
//    static Bandit* bandit_;

//};


#endif // ENEMY_H
