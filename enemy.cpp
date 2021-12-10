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

//Bandit* Spawner::bandit_ = new Bandit(20, 20, 40);

//Enemy* Spawner::spawnEnemy(){
//    return bandit_->clone();
//}

