#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QElapsedTimer>
#include <QTimer>
#include <vector>
#include <QTextEdit>

#include "game.h"
#include "player.h"
#include "enemy.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // set up game and UI
    void createGameGrid();
    void setupUI();
    void setupPlayers();
    void setupColors();

    void restartGame();

    // create castle/nuke that ends game
    void makeNuke(QColor color);

    // these create powerup on board
    void makeAttackPowerUp(int i, int j, QColor color);
    void makeHealthPowerUp(int i, int j, QColor color);
    void makePointsPowerUp(int i, int j, QColor color);

    // set up obsticals
    void makeBridge(QColor color);
    void makeRiver();
    void makeTree(int i, int j);
    void makeRock(int i, int j);

    // init timer
    void initTimer();

    // these create different enemy types
    void makeEnemy(int i, int j);
    void makeBandit(int i, int j);
    void makeWizard(int i, int j);

    // enemy setters and getters
    bool checkEnemy(int i, int j);
    game* getEnemyCell();
    Enemy* getEnemy();

    // ** castle and nuke are the same thing**
    // sets castle/nuke on gameboard
    void checkCastle(game * cell);
    void nukeGame();

// battle logic +++++++++++
    // check neighbor cells for enemies
    std::vector<game*> checkNeigbors(int i, int j);

    // enemy attack timer on/off
    void attackTimerOn();
    void attackTimerOff();
    int attackLogic();  // check enemy and player health
    void enemyDefeated();
    void endGame(Player * p); // end game if player dies

    // send messages (close to enemy or power-up aquired
    bool getMessageStatus() { return message_active_; };
    bool get_power_message() { return power_message_;};

    // create player
    void setPlayer(int x, int y, QColor color);
    // reset prev cell
    void updatePlayerPositionToFalse(int x, int y, QColor color);
    // move player
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

    void playerAttackSlot();
    void enemyAttackSlot();
    void removeEnemySlot(game * item);
    void messageSlot();
    void timerLabelSlot();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *MakeBoard_;

    int cell_height_;
    int cell_width_;

    int player_health_;
    int player_score_;
    int player_attack_;

    QTimer *timer;
    int time_elapsed_;
    bool start_;

    game * cells[10][20];

    Player *p1_;
    Player *p2_;

    Enemy *enemy_spawner_;
    Enemy *curr_enemy_;
    std::vector<Enemy*> enemies_;
    int i__, j__;

    std::vector<game*> neighbor_cells_;
    bool message_active_;
    bool power_message_;

    QColor newGameColor_;
    QColor player_color_;
    QColor river_color_;
    QColor rock_color_;
    QColor tree_color_;
    QColor enemy_color_;
    QColor bandit_color_;
    QColor wizard_color_;

};
#endif // MAINWINDOW_H
