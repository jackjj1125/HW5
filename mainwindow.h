#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include "game.h"
#include <QTextEdit>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createGameGrid();


    void makeNuke(QColor color);

    void makeBridge(QColor color);
    void makeRiver();
    void makeTree(int i, int j);
    void makeRock(int i, int j);

    void makeEnemy(int i, int j);

    void nukeGame();


    void setPlayer(int x, int y, QColor color);
    void updatePlayerPositionToFalse(int x, int y, QColor color);
    void movePlayer(int option);


private slots: // slots for buttons on UI

    void on_playButton_click(); //slot for play game button
    void on_restartButton_click(); // restarts game (One of our additional features)

    //Buttons for movement
    void up_button_clicked();

    void down_button_clicked();

    void left_button_clicked();

    void right_button_clicked();

    void on_actionOpen_Rules_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *MakeBoard_;

    int cell_height_;
    int cell_width_;

    int player_health_;
    int player_score_;
    int player_attack_;

    game * cells[10][20];

    Player *p1_;
    Player *p2_;

    QColor newGameColor_;
    QColor player_color_;
    QColor river_color_;
    QColor rock_color_;
    QColor tree_color_;

    QColor enemy_color_;

};
#endif // MAINWINDOW_H
