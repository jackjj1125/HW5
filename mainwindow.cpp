#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtWidgets>
#include <QElapsedTimer>
#include <QTimer>
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

    // seed random generator
    srand(time(0));

    // setup board and game elements
    initTimer();
    setupColors();
    createGameGrid();
    setupPlayers();
    setupUI();

    // Connect ui buttons and sliders to slots
    connect(ui->playButton, &QAbstractButton::pressed, this, &MainWindow::on_playButton_click);
    connect(ui->restartButton, &QAbstractButton::pressed, this, &MainWindow::on_restartButton_click);
    connect(ui->up_button, &QAbstractButton::clicked, this, &MainWindow::up_button_clicked);
    connect(ui->down_button, &QAbstractButton::clicked, this, &MainWindow::down_button_clicked);
    connect(ui->left_button, &QAbstractButton::clicked, this, &MainWindow::left_button_clicked);
    connect(ui->right_button, &QAbstractButton::clicked, this, &MainWindow::right_button_clicked);
    connect(ui->attackButton, &QAbstractButton::clicked, this, &MainWindow::playerAttackSlot);

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

    enemy_spawner_ = new Enemy(20, 20, 40);
    //curr_enemy_ = NULL;

    // create cells. these are the gameboard peices
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            game * item = new game(j,i,cell_width_/20, cell_height_/10, newGameColor_);
            cells[i][j] = item;
            MakeBoard_->addItem(item);
            connect(item, &game::messageSignal, this, &MainWindow::messageSlot);
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
                if(rn == 3){
                    qDebug() << "Random Enemy Added!";
                    makeEnemy(i, j);
                }
            }
        }
    }
    // make structures
    QColor bridge(100,100,100);
    QColor nuke(250,250,0); //yellow
    makeRiver();
    makeNuke(nuke);
    makeBridge(bridge);
}

void MainWindow::setupPlayers(){
    // init player
    QColor pColor(100,40,100);
    player_color_ = pColor;
    p1_ = new Player(0, 9, 0, 100, 20, pColor, "Player 1");
    setPlayer(0, 9, pColor);

    player_health_ = p1_->get_health();
    player_score_ = p1_->get_points();
    player_attack_ = p1_->get_attack();
}

void MainWindow::setupUI(){
    // set turn count and population labels to initial state after grid creation
    ui->score->setText(QString("Player Score: ")+QString::number(player_score_));
    ui->attackLabel->setText(QString("Player Attack: ")+QString::number(player_attack_));
    ui->health->setText(QString("Player Health: ")+QString::number(player_health_) + QString(" (") + QString::number((player_health_ * 100)/100) +QString("%)" ));
    ui->messageLabel->setText("No enemies near you");
    ui->enemyHealth->setText("");
    ui->timerLabel->setText("Time in battle: 0 sec");
    message_active_ = false;
}

void MainWindow::setupColors(){
    QColor color(255,255,255); // white for empty game tiles
    QColor river(0,200,255); //blue for river
    QColor tree(0,200,13); //green for trees
    QColor rock(40,80,100); // grey for rock
    QColor enemy(250,0,0); //red for enemies

    newGameColor_ = color;
    river_color_ = river;
    tree_color_ = tree;
    rock_color_ = rock;
    enemy_color_ = enemy;
}

void MainWindow::initTimer(){
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(enemyAttackSlot()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerLabelSlot()));
    start_ = false;
    time_elapsed_ = 0;
}

void MainWindow::restartGame(){
    if(start_){
        timer->stop();
        start_ = false;
    }
    delete p1_;
    p1_ = NULL;
    timer->~QTimer();
    ui->~MainWindow();
    initTimer();
    setupColors();
    createGameGrid();
    setupPlayers();
    setupUI();
}



// set up enemies
void MainWindow::makeEnemy(int i, int j){
    cells[i][j]->set_Color(enemy_color_);
    cells[i][j]->set_enemy(true, enemy_spawner_);
    cells[i][j]->set_obstical(true);
    cells[i][j]->getEnemyObject()->set_pos(i, j);

//    QPixmap pic("/path/to/your/image");
//    ui->gameGraphicsView->setPixmap(pic);
}


void MainWindow::setPlayer(int x, int y, QColor color){
    cells[y][x]->set_player_status(true);
    cells[y][x]->set_Color(color);
}

        //castle
void MainWindow::makeNuke(QColor color){ //one nuke on map ends game
    cells[0][1]->set_Color(color);
    cells[0][1]->set_castle(true);
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
// =============== End Of Game Setup ===============



// =============== Game Logic Begins here ===============
void MainWindow::nukeGame() //ends the game if player lands on nuke
{
    QTextEdit* GameEndsByNuke= new QTextEdit();
    GameEndsByNuke->setWindowFlags(Qt::Window);
    GameEndsByNuke->setReadOnly(true);
    GameEndsByNuke->append("Congratulations! You have won the game by way of Nuclear Bomb!");
    GameEndsByNuke->show();

    restartGame();
}

//if next cell is nuke
void MainWindow::checkCastle(game * cell){
    //&& p1_->get_points() >= 100
    if(cell->get_castle()){
        nukeGame();
    }
}

std::vector<game*> MainWindow::checkNeigbors(int i, int j){
    std::vector<game*> g;
    if(i > 0){
        g.push_back(cells[i-1][j]); // above
    }
    if(i > 0 && j > 0){
        g.push_back(cells[i-1][j-1]); //above, right
    }
    if(i > 0 && j < 19){
        g.push_back(cells[i-1][j+1]); //above, left
    }
    if(j > 0){
        g.push_back(cells[i][j-1]);//same row, right
    }
    if(j < 19){
        g.push_back(cells[i][j+1]);//same row, left
    }
    if(i < 9 && j > 0){
        g.push_back(cells[i+1][j-1]);//below, right
    }
    if(i < 9){
        g.push_back(cells[i+1][j]); // below
    }
    if(i < 9 && j < 19){
        g.push_back(cells[i+1][j+1]); // below, left
    }
    return g;
}

bool MainWindow::checkEnemy(int i, int j){
    neighbor_cells_ = checkNeigbors(i, j);
    qDebug() << "neighbors retrieved: "<< neighbor_cells_.size();
    for(int x = 0; x < neighbor_cells_.size(); x++){
        if(neighbor_cells_[x]->get_enemy()){
            return true;
        }
    }
    neighbor_cells_.clear();
    return false;
}

game* MainWindow::getEnemyCell(){
    for(int x = 0; x < neighbor_cells_.size(); x++){
        if(neighbor_cells_[x]->get_enemy()){
            return neighbor_cells_[x];
        }
    }
}

void MainWindow::attackTimerOn(){
    if(!start_){
        qDebug() << "attack Timer On";
        timer->start();
        start_ = true;
    }
}
void MainWindow::attackTimerOff(){
    if(start_){
        qDebug() << "attack Timer OFF";
        timer->stop();
        start_ = false;
    }
}

int MainWindow::attackLogic(){
    //qDebug() << "entered attack logic";
    if(p1_->get_health() <= 0){
        return 0;
    }
    else if(curr_enemy_->get_health() <= 0){
        return 1;
    }
    else{
        return -1;
    }
}

void MainWindow::movePlayer(int option){
    if(start_){
        attackTimerOff();
    }
    int i = p1_->get_pos_y();
    int j = p1_->get_pos_x();
    if(option == 1){  // UP
        if(i == 0){
            return;
        }
        if(checkEnemy(i-1, j)){
            message_active_ = true;
            cells[i][j]->sendMessage();
            curr_enemy_ = getEnemyCell()->getEnemyObject();
            attackTimerOn();
        }
        else{
            message_active_ = false;
        }
        if(!cells[i-1][j]->get_obstical_status()){
            cells[i-1][j]->movePlayerUp(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i-1][j]);
        }
        else{
            qDebug() << "Cannot move here!";
        }
    }
    if(option == 2){ // DOWN
        if(i == 9){
            return;
        }
        if(checkEnemy(i+1, j)){
            message_active_ = true;
            cells[i][j]->sendMessage();
            ui->enemyHealth->setText(QString("Enemy Health: ")+QString::number(curr_enemy_->get_health()) + QString(" (") + QString::number((curr_enemy_->get_health() * 100)/100) +QString("%)" ));
            curr_enemy_ = getEnemyCell()->getEnemyObject();
            attackTimerOn();
        }
        else{
            message_active_ = false;
            //curr_enemy_ = nullptr;
        }
        if(!cells[i+1][j]->get_obstical_status()){
            cells[i+1][j]->movePlayerDown(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i+1][j]);
        }
        else{
            qDebug() << "Cannot move here!";
        }
    }
    if(option == 3){ // LEFT
        if(j == 0){
            return;
        }
        if(checkEnemy(i, j-1)){
            message_active_ = true;
            cells[i][j]->sendMessage();
            curr_enemy_ = getEnemyCell()->getEnemyObject();
            attackTimerOn();
        }
        else{
            message_active_ = false;
            //curr_enemy_ = nullptr;
        }
        if(j != 0 && !cells[i][j-1]->get_obstical_status()){
            cells[i][j-1]->movePlayerLeft(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i][j-1]);

        }
        else{
            qDebug() << "Cannot move here!";
        }
    }
    if(option == 4){ // RIGHT
        if(j == 19){
            return;
        }
        if(checkEnemy(i, j+1)){
            message_active_ = true;
            cells[i][j]->sendMessage();
            curr_enemy_ = getEnemyCell()->getEnemyObject();
            attackTimerOn();
        }
        else{
            message_active_ = false;
            //curr_enemy_ = nullptr;
        }
        if(!cells[i][j+1]->get_obstical_status()){
            cells[i][j+1]->movePlayerRight(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i][j+1]);
        }
        else{
            qDebug() << "Cannot move here!";
        }
    }
}

void MainWindow::enemyDefeated(){
    cells[curr_enemy_->get_i()][curr_enemy_->get_j()]->removeDefeatedEnemy();
    //delete curr_enemy_;
    curr_enemy_ = nullptr;
    p1_->add_points(100);
    p1_->add_attack(20);
    p1_->add_health(20);
    //cells[p1_->get_pos_y()][p1_->get_pos_x()]->sendMessage();
    ui->score->setText(QString("Player Score: ")+QString::number(p1_->get_points()));
    ui->attackLabel->setText(QString("Player Attack: ")+QString::number(p1_->get_attack()));
    ui->health->setText(QString("Player Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));
//    QTextEdit* enemyDefeated= new QTextEdit();
//    enemyDefeated->setWindowFlags(Qt::Window);
//    enemyDefeated->setReadOnly(true);
//    enemyDefeated->append("You have defeated an enemy! You are rewarded with 100 points, and an additional health and attack bonus.");
//    enemyDefeated->show();
}

void MainWindow::endGame(Player *p){
    qDebug() << "GAME OVER";
    QTextEdit* playerDefeated= new QTextEdit();
    playerDefeated->setWindowFlags(Qt::Window);
    playerDefeated->setReadOnly(true);
    playerDefeated->append("GAME OVER! You were defeated by an enemy!");
    playerDefeated->show();
    restartGame();
}
// =============== End of Game Logic ===============



// =============== SLOTS ===============

void MainWindow::on_playButton_click(){
    qDebug() << "Play button clicked!";
}

// button for restarting game
void MainWindow::on_restartButton_click(){
    qDebug() << "Restart Game Button Clicked";
    restartGame();
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

void MainWindow::playerAttackSlot(){ // connects to attack button
    if(curr_enemy_ != nullptr){
        //qDebug() << "Attack button clicked";
        int status = attackLogic();
        if(status == 1){ // enemy health is 0
            attackTimerOff();
            enemyDefeated();
        }
        else{
            if(rand() % 10 == 1){
                qDebug() << "Critical hit!";
                curr_enemy_->decrease_health(p1_->get_attack() * 2);
            }
            else{
                curr_enemy_->decrease_health(p1_->get_attack());
            }
            ui->enemyHealth->setText(QString("Enemy Health: ")+QString::number(curr_enemy_->get_health()) + QString(" (") + QString::number((curr_enemy_->get_health() * 100)/100) +QString("%)" ));
            status = attackLogic();
            if(status == 1){ // enemy health is 0
                attackTimerOff();
                enemyDefeated();
            }
        }
    }
}

void MainWindow::enemyAttackSlot(){ // connects to timer
    //qDebug() << "enemyAttackSlot";
    int status = attackLogic();
    if(status == 0){ // player health is 0
        endGame(p1_);
    }
    if(status == 1){ // enemy health is 0
        attackTimerOff();
        enemyDefeated();
    }
    else{
        // invoke enemy attack
        p1_->decrease_health(curr_enemy_->get_attack());
        ui->health->setText(QString("Player Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));
        update();

        // check if player or enemy defeated again after attack
        status = attackLogic();
        if(status == 0){
            endGame(p1_);
        }
        if(status == 1){ // enemy health is 0
            attackTimerOff();
            enemyDefeated();
        }
    }
}

void MainWindow::messageSlot(){
    if(getMessageStatus()){
        ui->messageLabel->setText(QString("*WARNING* Enemy nearby!"));
        ui->enemyHealth->setText(QString("Enemy Health: ")+QString::number(curr_enemy_->get_health()) + QString(" (") + QString::number((curr_enemy_->get_health() * 100)/100) +QString("%)" ));
    }
    else{
        ui->enemyHealth->setText(QString(""));
        ui->messageLabel->setText("No enemies near you");
    }

}
void MainWindow::timerLabelSlot(){
    time_elapsed_++;
    ui->timerLabel->setText(QString("Time in battle: ") + QString::number(time_elapsed_) + QString(" sec") );
    update();
}

void MainWindow::on_actionOpen_Rules_triggered(){
    QTextEdit* Rules= new QTextEdit();
    Rules->setWindowFlags(Qt::Window);
    Rules->setReadOnly(true);
    Rules->append("These are the rules for the game.");
    Rules->show();
}


void MainWindow::on_pushButton_clicked(){
     QTextEdit* Rules= new QTextEdit();
     Rules->setWindowFlags(Qt::Window);
     Rules->setReadOnly(true);
     Rules->append("King Game: The objective of this game is to navigate the map, aquire power ups to make your character more powerful, and defeat any enemies you encounter on the way.To win the game, a player must defeat all the enemies on the gameboard while navigating the obstacles on the map. Once this is complete, the player becomes the new King.");
     Rules->show();
 }

