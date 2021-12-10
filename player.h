#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QColor>
#include <QGraphicsItem>

#ifndef PLAYER_H
#define PLAYER_H

struct Position {
    int x = 0;
    int y = 0;
};

class Player: public QObject {

public:
    Player(int x, int y, int points, int health, int attack, QColor color, std::string name);


    int get_pos_x() { return pos_.x; };
    int get_pos_y() { return pos_.y; };
    void set_pos_x(int x) { pos_.x = x; };
    void set_pos_y(int y) { pos_.y = y; };

    void move_up();
    void move_down();
    void move_left();
    void move_right();

    void add_points(int p) { points_ += p; };
    void add_attack(int a) { attack_ += a; };
    void add_health(int h) { health_ += h; };

    void decrease_health(int h) {health_ -= h; };

    int get_points() { return points_; };
    int get_attack() { return attack_; };
    int get_health() { return health_; };

// signals for preforming game logic
signals:
    void movePlayer(Player * p);

private:

    //player position
    Position pos_;

    std::string name_;

    int health_;
    int points_;
    int attack_;

    QColor Pcolor_;
};



#endif // PLAYER_H
