#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtWidgets>
#include <vector>
#include <QPixmap>

#include "mainwindow.h"
#include "game.h"
#include "player.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QColor color(255,255,255);
    QColor river(0,200,255); //blue
    QColor tree(0,200,13); //green
    QColor rock(40,80,100);
    QColor enemy(250,0,0); //red
    newGameColor_ = color;
    river_color_ = river;
    tree_color_ = tree;
    rock_color_ = rock;
    enemy_color_ = enemy;
    // randomize tree and rock
    srand(time(0));
    createGameGrid();

    // init player
    QColor pColor(100,40,100);
    player_color_ = pColor;
    p1_ = new Player(0, 9, 0, 100, 20, pColor, "Player 1");
    setPlayer(0, 9, pColor);

    player_health_ = p1_->get_health();
    player_score_ = p1_->get_points();
    player_attack_ = p1_->get_attack();

    // set turn count and population labels to initial state after grid creation
    ui->score->setText(QString("Player Score: ")+QString::number(player_score_));
    ui->attackLabel->setText(QString("Player Attack: ")+QString::number(player_attack_));
    ui->health->setText(QString("Player Health: ")+QString::number(player_health_) + QString(" (") + QString::number((player_score_ * 100)/200) +QString("%)" ));

    // Connect ui buttons and sliders to slots
    connect(ui->playButton, &QAbstractButton::pressed, this, &MainWindow::on_playButton_click);
    connect(ui->restartButton, &QAbstractButton::pressed, this, &MainWindow::on_restartButton_click);

    connect(ui->up_button, &QAbstractButton::clicked, this, &MainWindow::up_button_clicked);
    connect(ui->down_button, &QAbstractButton::clicked, this, &MainWindow::down_button_clicked);
    connect(ui->left_button, &QAbstractButton::clicked, this, &MainWindow::left_button_clicked);
    connect(ui->right_button, &QAbstractButton::clicked, this, &MainWindow::right_button_clicked);

    qDebug() << "*** Loading Complete ***";
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createGameGrid(){
    // create grid layout and set UI scene
    MakeBoard_ = new QGraphicsScene(this);
    QGraphicsView * grid_view = ui->gameGraphicsView;
    ui->gameGraphicsView->setScene(MakeBoard_);
    ui->gameGraphicsView->fitInView(MakeBoard_->sceneRect(), Qt::KeepAspectRatio);

    // define cell dimensions
    cell_height_ = grid_view->frameSize().height() - 3;
    cell_width_ = grid_view->frameSize().width() - 3;
    // create cells. these are the gameboard peices
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            game * item = new game(j,i,cell_width_/20, cell_height_/10, newGameColor_);
            cells[i][j] = item;
            MakeBoard_->addItem(item);
            //  connect(item, &game::moveUp, this, &MainWindow::up_button_clicked);
        }
    }


    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            int rn = rand() % 16;
            if(j > 5){
                if(rn == 7){
                    qDebug() << "Random Tree Added!";
                    makeTree(i, j);
                }
                if(rn == 2){
                    qDebug() << "Random Rock Added!";
                    makeRock(i, j);
                }
                if(rn == 2)
                {
                     qDebug() << "Random Enemy Added!";
                     makeEnemy(i,j);

                }
            }
        }
    }

    // make structures
    QColor bridge(100,100,100);
    QColor nuke(250,250,0); //yellow
    QColor attackPowerUp(250,100,0);
    QColor healthPowerUp(0,100,50);
    QColor pointsPowerUp(1,1,1);
    makeRiver();
    makeNuke(nuke);
    makeAttackPowerUp(attackPowerUp);
    makeHealthPowerUp(healthPowerUp);
    makePointsPowerUp(pointsPowerUp);
    makeBridge(bridge);


}

    //--------------------
        //make an enemy
void MainWindow::makeEnemy(int i, int j)
{
    cells[i][j]->set_Color(enemy_color_);
    cells[i][j]->set_enemy(true);
//    QPixmap pic("/path/to/your/image");
//    ui->gameGraphicsView->setPixmap(pic);
}
    //-------------------


void MainWindow::setPlayer(int x, int y, QColor color){
    cells[y][x]->set_player_status(true);
    cells[y][x]->set_Color(color);
}
        //nuke
void MainWindow::makeNuke(QColor color){ //one nuke on map ends game
    cells[0][1]->set_Color(color);
    cells[0][1]->set_Nuke(true);

}

void MainWindow::makeBridge(QColor color){
    cells[2][5]->set_Color(color);
    cells[2][5]->set_bridge_status(true);
}

void MainWindow::makeRiver(){
    for(int j = 0; j < 4; j++){
        cells[3][j]->set_Color(river_color_);
        cells[3][j]->set_obstical(true);
    }
    cells[2][4]->set_Color(river_color_);
        cells[2][4]->set_obstical(true);
    cells[2][6]->set_Color(river_color_);
        cells[2][6]->set_obstical(true);
    cells[1][7]->set_Color(river_color_);
        cells[1][7]->set_obstical(true);
    cells[1][8]->set_Color(river_color_);
        cells[1][8]->set_obstical(true);
    cells[0][9]->set_Color(river_color_);
        cells[0][9]->set_obstical(true);
}

void MainWindow::makeTree(int i, int j){
    cells[i][j]->set_Color(tree_color_);
    cells[i][j]->set_obstical(true);
}

void MainWindow::makeRock(int i, int j){
    cells[i][j]->set_Color(rock_color_);
    cells[i][j]->set_obstical(true);
}

void MainWindow::nukeGame() //ends the game if player lands on nuke
{
    QTextEdit* GameEndsByNuke= new QTextEdit();
    GameEndsByNuke->setWindowFlags(Qt::Window);
    GameEndsByNuke->setReadOnly(true);
    GameEndsByNuke->append("Congratulations! You have won the game by way of Nuclear Bomb!");
    GameEndsByNuke->show();
}

        //POWER UPS
void MainWindow::makeAttackPowerUp(QColor color)
{
    cells[0][6]->set_Color(color);
    cells[0][6]->set_attackPowerUp(true);

}

void MainWindow::makeHealthPowerUp(QColor color)
{
    cells[8][5]->set_Color(color);
    cells[8][5]->set_HealthPowerUp(true);

}

void MainWindow::makePointsPowerUp(QColor color)
{
    cells[6][4]->set_Color(color);
    cells[6][4]->set_PtsPowerUp(true);

}

void MainWindow::movePlayer(int option){
    int i = p1_->get_pos_y();
    int j = p1_->get_pos_x();
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    qDebug() << "player pos = i: " << i << ", j: " << j;
    if(option == 1){  // UP
        if(i != 0 && !cells[i-1][j]->get_obstical_status()){
            cells[i-1][j]->movePlayerUp(p1_);
            cells[i][j]->resetPrevCell();

            if(cells[i][j]->get_nuke() && count1 == 0) //if we land on nuke
             {
                 nukeGame();
                 count1++;
                 //end the game
             }
            else if(cells[i][j]->get_atkPowerUp() && count2 == 0) //if we land on attack power up
             {
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                 //increase player attack
                QTextEdit* AtkPowerUp= new QTextEdit();
                AtkPowerUp->setWindowFlags(Qt::Window);
                AtkPowerUp->setReadOnly(true);
                AtkPowerUp->append("Attack Power Up picked up! Player Attack Damage is increased by 0.5%");
                AtkPowerUp->show();
                count2++;
                qDebug() << "Attack upgrade picked up.";

             }
            else if(cells[i][j]->get_HealthPowerUp() && count3 == 0) //if we land on health power up
            {
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                //increase player health
                QTextEdit* HealthPowerUp= new QTextEdit();
                HealthPowerUp->setWindowFlags(Qt::Window);
                HealthPowerUp->setReadOnly(true);
                HealthPowerUp->append("Health Power Up picked up! Player Health is increased by 0.5%");
                HealthPowerUp->show();
                count3++;
                qDebug() << "Health upgrade picked up";

            }
            else if(cells[i][j]->get_PtsPowerUp() && count4 == 0) //if we land on points power up
            {
                p1_->add_points(100);
                //increase player pts
                QTextEdit* PtsPowerUp= new QTextEdit();
                PtsPowerUp->setWindowFlags(Qt::Window);
                PtsPowerUp->setReadOnly(true);
                PtsPowerUp->append("Points Power Up picked up! Player Points are increased by 100");
                PtsPowerUp->show();
                count4++;
                qDebug() << "points upgrade picked up.";
            }
        }
        else{
            qDebug() << "Cannot move here!";
        }
    }
    if(option == 2){ // DOWN
        if(i != 19 && !cells[i+1][j]->get_obstical_status()){
            cells[i+1][j]->movePlayerDown(p1_);
            cells[i][j]->resetPrevCell();
            if(cells[i][j]->get_nuke() && count1 == 0) //if we land on nuke
             {
                 nukeGame();
                 count1++;
             }
            else if(cells[i][j]->get_atkPowerUp() && count2 == 0) //if we land on attack power up
             {
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                 //increase player attack
                QTextEdit* AtkPowerUp= new QTextEdit();
                AtkPowerUp->setWindowFlags(Qt::Window);
                AtkPowerUp->setReadOnly(true);
                AtkPowerUp->append("Attack Power Up picked up! Player Attack Damage is increased by 0.5%");
                AtkPowerUp->show();
                count2++;
                            qDebug() << "Attack upgrade picked up.";
             }
            else if(cells[i][j]->get_HealthPowerUp() && count3 == 0) //if we land on health power up
            {
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                //increase player health
                QTextEdit* HealthPowerUp= new QTextEdit();
                HealthPowerUp->setWindowFlags(Qt::Window);
                HealthPowerUp->setReadOnly(true);
                HealthPowerUp->append("Health Power Up picked up! Player Health is increased by 0.5%");
                HealthPowerUp->show();
                count3++;
                      qDebug() << "Health upgrade picked up";

            }
            else if(cells[i][j]->get_PtsPowerUp() && count4 == 0) //if we land on points power up
            {
                p1_->add_points(100);
                //increase player pts
                QTextEdit* PtsPowerUp= new QTextEdit();
                PtsPowerUp->setWindowFlags(Qt::Window);
                PtsPowerUp->setReadOnly(true);
                PtsPowerUp->append("Points Power Up picked up! Player Points are increased by 100");
                PtsPowerUp->show();
                count4++;
                         qDebug() << "points upgrade picked up.";

            }
        }
        else{
            qDebug() << "Cannot move here!";
        }
    }
    if(option == 3){ // LEFT
        if(j != 0 && !cells[i][j-1]->get_obstical_status()){
            cells[i][j-1]->movePlayerLeft(p1_);
            cells[i][j]->resetPrevCell();
            if(cells[i][j]->get_nuke() && count1 == 0) //if we land on nuke
             {
                 nukeGame();
                 count1++;
             }
            if(cells[i][j]->get_atkPowerUp() && count2 == 0) //if we land on attack power up
             {
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                 //increase player attack
                QTextEdit* AtkPowerUp= new QTextEdit();
                AtkPowerUp->setWindowFlags(Qt::Window);
                AtkPowerUp->setReadOnly(true);
                AtkPowerUp->append("Attack Power Up picked up! Player Attack Damage is increased by 0.5%");
                AtkPowerUp->show();
                count2++;
                            qDebug() << "Attack upgrade picked up.";

             }
            else if(cells[i][j]->get_HealthPowerUp() && count3 == 0) //if we land on health power up
            {
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                //increase player health
                QTextEdit* HealthPowerUp= new QTextEdit();
                HealthPowerUp->setWindowFlags(Qt::Window);
                HealthPowerUp->setReadOnly(true);
                HealthPowerUp->append("Health Power Up picked up! Player Health is increased by 0.5%");
                HealthPowerUp->show();
                count3++;
                      qDebug() << "Health upgrade picked up";

            }
            else if(cells[i][j]->get_PtsPowerUp() && count4 == 0) //if we land on points power up
            {
                p1_->add_points(100);
                //increase player points

                QTextEdit* PtsPowerUp= new QTextEdit();
                PtsPowerUp->setWindowFlags(Qt::Window);
                PtsPowerUp->setReadOnly(true);
                PtsPowerUp->append("Points Power Up picked up! Player Points are increased by 100");
                PtsPowerUp->show();
                count4++;
                         qDebug() << "points upgrade picked up.";

            }
        }
        else{
            qDebug() << "Cannot move here!";
        }
    }
    if(option == 4){ // RIGHT
        if(j != 19 && !cells[i][j+1]->get_obstical_status()){
            cells[i][j+1]->movePlayerRight(p1_);
            cells[i][j]->resetPrevCell();
            if(cells[i][j]->get_nuke()) //if we land on nuke
             {
                 nukeGame();
             }
            if(cells[i][j]->get_atkPowerUp() && count2 == 0) //if we land on attack power up
             {
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                 //increase player attack

                QTextEdit* AtkPowerUp= new QTextEdit();
                AtkPowerUp->setWindowFlags(Qt::Window);
                AtkPowerUp->setReadOnly(true);
                AtkPowerUp->append("Attack Power Up picked up! Player Attack Damage is increased by 0.5%");
                AtkPowerUp->show();
                count2++;
                            qDebug() << "Attack upgrade picked up.";

             }
            else if(cells[i][j]->get_HealthPowerUp() && count3 == 0) //if we land on health power up
            {
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                //increase player health
                QTextEdit* HealthPowerUp= new QTextEdit();
                HealthPowerUp->setWindowFlags(Qt::Window);
                HealthPowerUp->setReadOnly(true);
                HealthPowerUp->append("Health Power Up picked up! Player Health is increased by 0.5%");
                HealthPowerUp->show();
                count3++;
                qDebug() << "health power up picked up";

            }
            else if(cells[i][j]->get_PtsPowerUp() && count4 == 0) //if we land on points power up
            {
                p1_->add_points(100);
                //increase player pts
                QTextEdit* PtsPowerUp= new QTextEdit();
                PtsPowerUp->setWindowFlags(Qt::Window);
                PtsPowerUp->setReadOnly(true);
                PtsPowerUp->append("Points Power Up picked up! Player Points are increased by 100");
                PtsPowerUp->show();
                count4++;
                qDebug() << "points power up picked up";
                }
            }
        }
        else{
            qDebug() << "Cannot move here!";
        }
    }



// ===== SLOTS =====

// slot for play button: starts or resumes timer
void MainWindow::on_playButton_click(){
    qDebug() << "Play button clicked!";
}

// button for restarting game
void MainWindow::on_restartButton_click(){
    qDebug() << "Restart Game Button Clicked";
}

//SLOTS FOR PLAYER MOVEMENT
void MainWindow::up_button_clicked() //slot for moving player up a block
{
    qDebug() << "Move up";
    movePlayer(1);

}

void MainWindow::left_button_clicked() //slot for moving player left
{
    qDebug() << "Move left";
    movePlayer(3);
}

void MainWindow::down_button_clicked() //slot for moving player down
{
    qDebug() << "Move down";
    movePlayer(2);
}

void MainWindow::right_button_clicked() //slot for moving player right
{
    qDebug() << "Move right";
    movePlayer(4);
}


void MainWindow::on_actionOpen_Rules_triggered()
{
    QTextEdit* Rules= new QTextEdit();
    Rules->setWindowFlags(Qt::Window);
    Rules->setReadOnly(true);
    Rules->append("These are the rules for the game.");
    Rules->show();
}


void MainWindow::on_pushButton_clicked()
{
    QTextEdit* Rules= new QTextEdit();
    Rules->setWindowFlags(Qt::Window);
    Rules->setReadOnly(true);
    Rules->append("King Game: The objective of this game is to navigate the map, aquire power ups to make your character more powerful, and defeat any enemies you encounter on the way.To win the game, a player must defeat all the enemies on the gameboard while navigating the obstacles on the map. Once this is complete, the player becomes the new King.");
    Rules->show();
}

