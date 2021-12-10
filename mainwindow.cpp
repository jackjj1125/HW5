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
    // final boss
    Enemy* finalBoss = new Enemy(6000, 200, 10000);
    enemies_.push_back(finalBoss);
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

    //enemy_spawner_ = new Enemy(20, 20, 40);
    //enemies_.back() = NULL;

//    Enemy * BanditPrototype = new Bandit(20, 20, 40);
//    Spawner * banditSpawner = new Spawner(BanditPrototype);

    // create cells. these are the gameboard peices
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            game * item = new game(j,i,cell_width_/20, cell_height_/10, newGameColor_);
            cells[i][j] = item;
            MakeBoard_->addItem(item);
            connect(item, &game::messageSignal, this, &MainWindow::messageSlot);
            connect(item, &game::removeEnemySignal, this, &MainWindow::removeEnemySlot);
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            int rn = rand() % 33;
            if(j > 5){
                if(rn == 7 || rn == 29){
                    qDebug() << "Random Tree Added!";
                    makeTree(i, j);
                }
                if(rn == 2 || rn == 24){
                    qDebug() << "Random Rock Added!";
                    makeRock(i, j);
                }
                if(rn == 3 || rn == 16 ){
                    int rPower = rand() % 3;
                    if(rPower == 0){
                        makeEnemy(i, j);
                    }
                    if(rPower == 1){
                        makeBandit(i, j);
                    }
                    if(rPower == 2){
                        makeWizard(i, j);
                    }

                    qDebug() << "Random Enemy Added!";

                }
                if(rn == 4){
                    int rPower = rand() % 3;
                    if(rPower == 0){
                        QColor attackPowerUp(205, 204, 153); //
                        makeAttackPowerUp(i, j, attackPowerUp);
                    }
                    if(rPower == 1){
                        QColor healthPowerUp(204,253,152); //
                        makeHealthPowerUp(i, j, healthPowerUp);

                    }
                    if(rPower == 2){
                        QColor pointsPowerUp(255,255,80); //
                        makePointsPowerUp(i, j, pointsPowerUp);
                    }
                }
            }
        }
    }
    // make structures
    QColor bridge(145,62,62); // brown
    QColor nuke(90,80,90); //grey
    makeRiver();
    makeNuke(nuke);
    makeBridge(bridge);
    //hardcodeEnemy();  // for testing
    power_message_ = false;
}

void MainWindow::setupPlayers(){
    // init player
    QColor pColor(1,1,1);
    player_color_ = pColor;
    p1_ = new Player(0, 9, 0, 100, 5, pColor, "Player 1");
    setPlayer(0, 9, pColor);

    player_health_ = p1_->get_health();
    player_score_ = p1_->get_points();
    player_attack_ = p1_->get_attack();
}

void MainWindow::setupUI(){
    // set turn count and population labels to initial state after grid creation
    ui->score->setText(QString("Score: ")+QString::number(player_score_));
    ui->attackLabel->setText(QString("Attack: ")+QString::number(player_attack_));
    ui->health->setText(QString("Health: ")+QString::number(player_health_) + QString(" (") + QString::number((player_health_ * 100)/100) +QString("%)" ));
    ui->messageLabel->setText("No enemies near you");
    ui->enemyHealth->setText("");
    ui->timerLabel->setText("Time in battle: 0 sec");
    message_active_ = false;
}

void MainWindow::setupColors(){
    QColor color(255,255,255); // white for empty game tiles
    QColor river(0,200,255); //blue for river
    QColor tree(0,200,13); //green for trees
    QColor rock(200,200,200); // grey for rock
    QColor enemy(255,0,0); //red for enemies
    QColor bandit(255, 153, 153);
    QColor wiz(127, 0, 200);

    newGameColor_ = color;
    river_color_ = river;
    tree_color_ = tree;
    rock_color_ = rock;
    enemy_color_ = enemy;
    bandit_color_ = bandit;
    wizard_color_ = wiz;
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
    Enemy* bandit = new Enemy(60, 25, 85);
    enemies_.push_back(bandit);
    cells[i][j]->set_Color(enemy_color_);
    cells[i][j]->set_enemy(true);
    cells[i][j]->set_obstical(true);
}

void MainWindow::makeBandit(int i, int j){
     Enemy* bandit = new Enemy(20, 20, 40);
     enemies_.push_back(bandit);
     cells[i][j]->set_Color(bandit_color_);
     cells[i][j]->set_enemy(true);
     cells[i][j]->set_obstical(true);
 }
void MainWindow::makeWizard(int i, int j){
    Enemy* bandit = new Enemy(40, 100, 40);
    enemies_.push_back(bandit);
    cells[i][j]->set_Color(wizard_color_);
    cells[i][j]->set_enemy(true);
    cells[i][j]->set_obstical(true);
}

//void MainWindow::hardcodeEnemy(){
//    Enemy* bandit = new Enemy(20, 20, 40);
//    enemies_.push_back(bandit);
//    cells[7][0]->set_enemy(true);
//    cells[7][0]->set_obstical(true);
//    cells[7][0]->set_Color(enemy_color_);
//    connect(cells[7][0], &game::removeEnemySignal, this, &MainWindow::removeEnemySlot);
//}

Enemy* MainWindow::getEnemy(){
    return enemies_.back();
}

//POWER UPS
void MainWindow::makeAttackPowerUp(int i, int j, QColor color)
{
    cells[i][j]->set_Color(color);
    cells[i][j]->set_attackPowerUp(true);

}

void MainWindow::makeHealthPowerUp(int i, int j, QColor color)
{
    cells[i][j]->set_Color(color);
    cells[i][j]->set_HealthPowerUp(true);

}
void MainWindow::makePointsPowerUp(int i, int j, QColor color)
    {
    cells[i][j]->set_Color(color);
    cells[i][j]->set_PtsPowerUp(true);

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

void MainWindow::makeBridge(QColor color){ // makes bridge cell that lets player cross river
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
    if(neighbor_cells_.empty()){
        return false;
    }
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
    else if(enemies_.back()->get_health() <= 0){
        return 1;
    }
    else{
        return -1;
    }
}

void MainWindow::movePlayer(int option){
    attackTimerOff();
    int i = p1_->get_pos_y();
    int j = p1_->get_pos_x();
    if(option == 1){  // UP
        if(i == 0){
            return;
        }

        if(checkEnemy(i-1, j)){
            message_active_ = true;
            cells[i][j]->sendMessage();
            attackTimerOn();
        }
        else{
            message_active_ = false;
            cells[i][j]->sendMessage();
        }
        if(!cells[i-1][j]->get_obstical_status()){
            cells[i-1][j]->movePlayerUp(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i-1][j]);
            if(cells[i-1][j]->get_atkPowerUp()) //if we land on attack power up
            {
                power_message_ = true;
                cells[i][j]->sendMessage();
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                ui->attackLabel->setText(QString("Attack: ")+QString::number(p1_->get_attack()) + QString(" (") + QString::number((p1_->get_attack() * 100)/100) +QString("%)" ));

            }
            else if(cells[i-1][j]->get_HealthPowerUp()) //if we land on health power up
            {
                power_message_ = true;
                cells[i][j]->sendMessage();
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                ui->health->setText(QString("Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));


            }
            else if(cells[i-1][j]->get_PtsPowerUp()) //if we land on points power up
            {
                power_message_ = true;
                cells[i][j]->sendMessage();
                p1_->add_points(100);
                ui->score->setText(QString("Points: ")+QString::number(p1_->get_points()) + QString(" (") + QString::number((p1_->get_points() * 100)/100) +QString("%)" ));

            }
            else{
                power_message_ = false;
                cells[i][j]->sendMessage();
            }
            update();
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
            attackTimerOn();
        }
        else{
            message_active_ = false;
            cells[i][j]->sendMessage();

        }
        if(!cells[i+1][j]->get_obstical_status()){
            cells[i+1][j]->movePlayerDown(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i+1][j]);
            if(cells[i+1][j]->get_atkPowerUp()) //if we land on attack power up
            {
                power_message_ = true;
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                ui->attackLabel->setText(QString("Attack: ")+QString::number(p1_->get_attack()) + QString(" (") + QString::number((p1_->get_attack() * 100)/100) +QString("%)" ));
            }
            else if(cells[i+1][j]->get_HealthPowerUp()) //if we land on health power up
            {
                power_message_ = true;
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                ui->health->setText(QString("Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));
                update();

            }
            else if(cells[i+1][j]->get_PtsPowerUp()) //if we land on points power up
            {
                power_message_ = true;
                p1_->add_points(100);
                ui->score->setText(QString("Points: ")+QString::number(p1_->get_points()) + QString(" (") + QString::number((p1_->get_points() * 100)/100) +QString("%)" ));

            }
            else{
                power_message_ = false;
            }
            cells[i][j]->sendMessage();
            update();
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
            attackTimerOn();
        }
        else{
            message_active_ = false;
            cells[i][j]->sendMessage();
        }
        if(j != 0 && !cells[i][j-1]->get_obstical_status()){
            cells[i][j-1]->movePlayerLeft(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i][j-1]);
            if(cells[i][j-1]->get_atkPowerUp()) //if we land on attack power up
            {
                power_message_ = true;
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                ui->attackLabel->setText(QString("Attack: ")+QString::number(p1_->get_attack()) + QString(" (") + QString::number((p1_->get_attack() * 100)/100) +QString("%)" ));

            }
            else if(cells[i][j-1]->get_HealthPowerUp()) //if we land on health power up
            {
                power_message_ = true;
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                //increase player health
                ui->health->setText(QString("Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));


            }
            else if(cells[i][j-1]->get_PtsPowerUp()) //if we land on points power up
            {
                power_message_ = true;
                p1_->add_points(100);
                ui->score->setText(QString("Points: ")+QString::number(p1_->get_points()) + QString(" (") + QString::number((p1_->get_points() * 100)/100) +QString("%)" ));

            }
            else{
                power_message_ = false;
            }
            cells[i][j]->sendMessage();
            update();
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
            attackTimerOn();
        }
        else{
            message_active_ = false;
            cells[i][j]->sendMessage();
        }
        if(!cells[i][j+1]->get_obstical_status()){
            cells[i][j+1]->movePlayerRight(p1_);
            cells[i][j]->resetPrevCell();
            checkCastle(cells[i][j+1]);
            if(cells[i][j+1]->get_atkPowerUp()) //if we land on attack power up
            {
                power_message_ = true;
                int a = p1_->get_attack();
                p1_->add_attack(a * 1.5);
                 //increase player attack
                ui->attackLabel->setText(QString("Attack: ")+QString::number(p1_->get_attack()) + QString(" (") + QString::number((p1_->get_attack() * 100)/100) +QString("%)" ));


            }
            else if(cells[i][j+1]->get_HealthPowerUp()) //if we land on health power up
            {
                power_message_ = true;
                int p = p1_->get_health();
                p1_->add_health(p * 1.5);
                ui->health->setText(QString("Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));

            }
            else if(cells[i][j+1]->get_PtsPowerUp()) //if we land on points power up
            {
                power_message_ = true;
                p1_->add_points(100);
                //increase player pts
                ui->score->setText(QString("Points: ")+QString::number(p1_->get_points()) + QString(" (") + QString::number((p1_->get_points() * 100)/100) +QString("%)" ));

            }
            else{
                qDebug() << " power_message_ = false!";
                power_message_ = false;
            }
            cells[i][j]->sendMessage();
            update();
        }
    }
    else{
        qDebug() << "Cannot move here!";
    }
}


void MainWindow::enemyDefeated(){
     qDebug() << "entered MainWindow::enemyDefeated";

    //delete enemies_.back();
    neighbor_cells_.erase(neighbor_cells_.begin());
    p1_->add_points(100);
    p1_->add_attack(20);
    p1_->add_health(20);
    cells[p1_->get_pos_y()][p1_->get_pos_x()]->sendMessage();
    ui->score->setText(QString("Player Score: ")+QString::number(p1_->get_points()));
    ui->attackLabel->setText(QString("Player Attack: ")+QString::number(p1_->get_attack()));
    ui->health->setText(QString("Player Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));
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
    if(checkEnemy(p1_->get_pos_y(), p1_->get_pos_x())){
        qDebug() << "Attack button clicked";
        int status = attackLogic();
        if(status == 1){ // enemy health is 0
            qDebug() << "entered conditional 1 (enemy dead)";
            attackTimerOff();
            getEnemyCell()->removeDefeatedEnemy();
            message_active_ = false;
            getEnemyCell()->sendMessage();
            return;
        }
        else{
            if(rand() % 10 == 1){
                qDebug() << "Critical hit!";
                enemies_.back()->decrease_health(p1_->get_attack() * 2);
            }
            else{
                enemies_.back()->decrease_health(p1_->get_attack());
            }
            ui->enemyHealth->setText(QString("Enemy Health: ")+QString::number(enemies_.back()->get_health()) + QString(" (") + QString::number((enemies_.back()->get_health() * 100)/100) +QString("%)" ));
            status = attackLogic();
            if(status == 1){ // enemy health is 0
                 qDebug() << "entered conditional 2 (enemy dead)";
                attackTimerOff();
                attackTimerOff();
                getEnemyCell()->removeDefeatedEnemy();
                message_active_ = false;
                getEnemyCell()->sendMessage();
                return;
            }
        }
    }
}

void MainWindow::enemyAttackSlot(){ // connects to timer
    qDebug() << "enemyAttackSlot";
    int status = attackLogic();
    if(status == 0){ // player health is 0
        endGame(p1_);
    }
    if(status == 1){ // enemy health is 0
        attackTimerOff();
    }
    else{
        // invoke enemy attack
        p1_->decrease_health(enemies_.back()->get_attack());
        ui->health->setText(QString("Health: ")+QString::number(p1_->get_health()) + QString(" (") + QString::number((p1_->get_health() * 100)/100) +QString("%)" ));
        update();

        // check if player or enemy defeated again after attack
        status = attackLogic();
        if(status == 0){
            endGame(p1_);
        }
        if(status == 1){ // enemy health is 0
            attackTimerOff();
        }
    }
}

void MainWindow::removeEnemySlot(game * item){
    qDebug() << "removeEnemySlot";
    enemies_.pop_back();
    enemyDefeated();
}

void MainWindow::messageSlot(){
    if(getMessageStatus()){
        ui->messageLabel->setText(QString("*WARNING* Enemy nearby!"));
        ui->enemyHealth->setText(QString("Enemy Health: ")+QString::number(enemies_.back()->get_health()) + QString(" (") + QString::number((enemies_.back()->get_health() * 100)/100) +QString("%)" ));
    }
    else{
        if(get_power_message()){
            ui->messageLabel->setText(QString("POWER UP! You have aquired a ") + QString("Bonus!"));
        }
        else{
            ui->messageLabel->setText("No enemies near you");
        }
        ui->enemyHealth->setText(QString(""));
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

