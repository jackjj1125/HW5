#ifndef GAME_H
#define GAME_H
#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QColor>
#include <QGraphicsItem>
#include "player.h"
#include "enemy.h"

class game: public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    game(int x, int y, int width, int height, QColor c); //constructor

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;


    int get_x() {return x_;}; //getter for x coord
    int get_y(){return y_;}; //getter for y cord

    int get_width() {return width_;}; //getter for cell width
    int get_height() {return height_;};

    QColor get_color() { return color_; }; //getter for color of cell
    void set_Color(QColor color) { color_ = color; }; //setter for color of cell(handles color logic)

    // game board cell status
    void set_player_status(bool status) { is_player_ = status; };
    bool get_player_status() { return is_player_; };

    // flag for obsticals --> river, rock, or tree
    void set_obstical(bool status){ is_obstical_ = status; };
    bool get_obstical_status(){ return is_obstical_; };

    void set_castle(bool c) { is_castle_ = c; };
    bool get_castle() { return is_castle_; };
    void set_bridge_status(bool s) { is_bridge_ = s; };

    // cell indices on gameboard
    int get_i_() { return i_;};
    int get_j_() { return j_;}

    // enemy flag setters and getters
    void set_enemy(bool e){ is_enemy_ = e;};
    bool get_enemy() { return is_enemy_; };

    //send enemy or bonus message to UI
    void sendMessage();

    // set previous cell to white
    void resetPrevCell();

    // move player methods
    void movePlayerUp(Player * p);
    void movePlayerDown(Player * p);
    void movePlayerLeft(Player * p);
    void movePlayerRight(Player * p);

    // enemy encounter methods
    void attackEnemy(Enemy * e);
    void attackPlayer(Player * p, int h);
    void removeDefeatedEnemy();

    // Power up methods
    void set_attackPowerUp(bool a) {is_ATK_ = a;};
    bool get_atkPowerUp(){return is_ATK_;};

    void set_HealthPowerUp(bool h){is_HEALTH_ = h;};
    bool get_HealthPowerUp(){return is_HEALTH_;};

    void set_PtsPowerUp(bool p){is_PTS_ = p;};
    bool get_PtsPowerUp(){return is_PTS_;};

// signals for preforming game logic
signals:
    void moveUp(game * item);
    void moveDown(game * item);
    void moveLeft(game * item);
    void moveRight(game * item);
    void resetPrevSignal(game * item);

    void attackSignal(game * item); // emitted when enemy attacks
    void removeEnemySignal(game * item);
    void messageSignal(game * item);
    //void messagePowerSignal(game * item);


protected:
    // overridden mousePressEvent to handle user interaction with cells
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

private:
    int x_;
    int y_;
    int width_;
    int height_;
    QColor color_;

    bool is_player_;

    // for obstical cells
    bool is_obstical_;

    // for structures
    bool is_castle_;
    bool is_bridge_;


    //for enemies
    bool is_enemy_;
    int i_;
    int j_;
    //Enemy *enemy_;

    bool is_ATK_;
    bool is_HEALTH_;
    bool is_PTS_;

};

#endif // GAME_H




