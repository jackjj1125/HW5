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

    void set_player_status(bool status) { is_player_ = status; };
    bool get_player_status() { return is_player_; };

    void set_obstical(bool status){ is_obstical_ = status; };
    bool get_obstical_status(){ return is_obstical_; };

    void set_castle(bool c) { is_castle_ = c; };
    void set_bridge_status(bool s) { is_bridge_ = s; };

    void resetPrevCell();
    void movePlayerUp(Player * p);
    void movePlayerDown(Player * p);
    void movePlayerLeft(Player * p);
    void movePlayerRight(Player * p);

// signals for preforming game logic
signals:
    void moveUp(game * item);
    void moveDown(game * item);
    void moveLeft(game * item);
    void moveRight(game * item);
    void resetPrevSignal(game * item);

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


};

#endif // GAME_H




