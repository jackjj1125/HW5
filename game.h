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

    int get_status() {return is_alive;}; //getter for status of cell(alive or dead)
    int get_neighborStatus(); //getter for status of cell's neighbor
    int get_nextStatus()  {return next_turn;};

    void set_status(bool status) {is_alive = status;}; //setter for status of cell
    void set_NeighborStatus(); //setter for status of neighbor
    void set_nextStatus(int next_turn_) {next_turn = next_turn_;}; //sets status for next turn

    QColor get_color() { return color_; }; //getter for color of cell
    void set_Color(QColor color) { color_ = color; }; //setter for color of cell(handles color logic)

    // methods to kill or revive cells called every step of the game
    void kill();
    void revive(QColor newGameColor);



    // signals for preforming game logic
    signals:
        void reviveCell(game * cell);
        void killCell(game * cell);
        void neighbors(int x, int y);


protected:
        // overridden mousePressEvent to handle user interaction with cells
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

private:
    // cell fields
    int x_;
    int y_;
    int width_;
    int height_;
    bool is_alive;
    int next_turn;
    QColor color_;
};


// bar graph to track population
class Bar: public QObject, public QGraphicsItem //bar class
{
    Q_OBJECT;

public:
    Bar(int x, int y, int h,  QColor color); //constructor

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    static int get_width() {return width_; }; //getter for width
    double getHeight() { return height_; };

    int get_x() {return x_; }; //getter for x
    void set_x(int s) {x_ = x_ + s; }; //setter for x
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void setBarColor(QColor color);

private:
    int x_;
    int y_;
    static const int width_ = 30; //width for bars is const
    double height_;

    QColor color_; //color for bar
};

#endif // GAME_H
