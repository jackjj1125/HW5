#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>

#include "game.h"

namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // constructor and destructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createGameGrid(QColor color);


    // methods for preforming turn/step of game
    void turnCount(); //handles turns
    void checkAlive(); //checks if cells should be alive or dead

    // getters and setters for cell neighbors
    int checkNeighbors(std::vector<game*> neighbors); //checks neighbors
    std::vector<game*> getNeighbors(int i, int j); // gets cell neighbors

    bool is_alive(); //checks if a given cell is alive

    // increase and decrease population counter
    void increasePopulation();
    void decreasePopulation();

    void neighborSlot(int x, int y);

    int population_; // population counter

    // timer

    void changeInterval(int t);

    void updateGraph();


private slots: // slots for buttons on UI

    void on_playButton_click(); //slot for play game button
    void on_restartButton_click(); // restarts game (One of our additional features)

    void clickCellSlot(game * cell); // slot for mouse press events (clicking cells on UI)


        //Buttons for movement
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    QGraphicsScene *MakeBoard_;
    QGraphicsScene *MakePopGraph_;

    int cell_height_;
    int cell_width_;
    int turnCounter_ = 0;
    QTimer *timer;

    game * cells[10][10];
    std::vector <Bar*> popBar_;
    Bar * prev_bar_;

    // bar graph fields
    int y_bar;
    double h_bar;
    int bar_height_;
    int bar_width_;

    // start flag for timer
    bool start_;

    QColor newGameColor;
};
#endif // MAINWINDOW_H
