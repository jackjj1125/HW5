#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QtWidgets>
#include <vector>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QColor color(0,150,0);
    newGameColor = color;
    createGameGrid(newGameColor);


    // set turn count and population labels to initial state after grid creation
    ui->label->setText(QString("Turn: ")+QString::number(turnCounter_));
    ui->label_2->setText(QString("Population: ")+QString::number(population_) + QString(" (") + QString::number((population_ * 100)/200) +QString("%)" ));
    //ui->label_2->setText(QString("Population: ")+QString::number(population_));

    // Connect ui buttons and sliders to slots
    connect(ui->playButton, &QAbstractButton::pressed, this, &MainWindow::on_playButton_click);
    connect(ui->pauseButton, &QAbstractButton::pressed, this, &MainWindow::on_pauseButton_click);
    connect(ui->stepButton, &QAbstractButton::pressed, this, &MainWindow::on_stepButton_click);
    connect(ui->restartButton, &QAbstractButton::pressed, this, &MainWindow::on_restartButton_click);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::createGameGrid(QColor color){
    // set population and turn counters to 0
    population_ = 0;
    turnCounter_ = 0;

    // set up timer and set start flag to false
    initTimer();
    start_ = false;

    // create grid layout and set UI scene
    MakeBoard_ = new QGraphicsScene(this);
    QGraphicsView * grid_view = ui->gameGraphicsView;
    ui->gameGraphicsView->setScene(MakeBoard_);
    ui->gameGraphicsView->fitInView(MakeBoard_->sceneRect(), Qt::KeepAspectRatio);

    // define cell dimensions
    cell_height_ = grid_view->frameSize().height() - 3;
    cell_width_ = grid_view->frameSize().width() - 3;

    // initialize cells randomly
    srand(time(0));
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            game * item = new game(j,i,cell_width_/20, cell_height_/10, color);
            cells[i][j] = item;
            cells[i][j]->set_nextStatus(-1);
            MakeBoard_->addItem(item);
            connect(item, &game::reviveCell, this, &MainWindow::clickCellSlot);
            connect(item, &game::killCell, this, &MainWindow::clickCellSlot);
            if(item->get_status()){
                increasePopulation();
            }
        }
    }
}



void MainWindow::initTimer(){
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_stepButton_click()));
}

void MainWindow::changeInterval(int t){
    if(t < 2){
        return;
    }

    timer->setInterval(1000 / t);
}

void MainWindow::increasePopulation(){
    population_++;
    ui->label_2->setText(QString("Population: ")+QString::number(population_)+QString(" (") +QString::number((population_ * 100)/200)+QString("%)"));

}
void MainWindow::decreasePopulation(){
    population_--;
    ui->label_2->setText(QString("Population: ")+QString::number(population_)+QString(" (") +QString::number((population_ * 100)/200)+QString("%)"));

}

void MainWindow::turnCount() //handles number of turns
{
    turnCounter_++; //increment turn counter
    ui->label->setText(QString("Turn: ")+QString::number(turnCounter_)); //print number turn
}

/*
 getNeighbors(int i, int j)
 this function gets the neigbors of the current cell being checked and returns them in a temp vector
 this also makes sure to catch the edge cases when checking neigbors,
      edge cases: current cell is in one of the corners of the grid
                  current cell lies on one of the edges
 @params = indecies (row, col) of current cell
*/
std::vector<game*> MainWindow::getNeighbors(int i, int j){

    // temp vector of all the neigbors of cells[i][j]
    std::vector<game*> neighbors;

    if(i == 0 && j == 0){  // top left corner
        neighbors.push_back(cells[9][19]);
        neighbors.push_back(cells[9][j]);
        neighbors.push_back(cells[9][j+1]);

        neighbors.push_back(cells[i][19]);
        neighbors.push_back(cells[i][j+1]);

        neighbors.push_back(cells[i+1][19]);
        neighbors.push_back(cells[i+1][j]);
        neighbors.push_back(cells[i+1][j+1]);

        return neighbors;
    }
    else if(i == 9 && j == 19){ // bottom right corner
        neighbors.push_back(cells[i-1][j-1]);
        neighbors.push_back(cells[i-1][j]);
        neighbors.push_back(cells[i-1][0]);

        neighbors.push_back(cells[i][j-1]);
        neighbors.push_back(cells[i][0]);

        neighbors.push_back(cells[0][j-1]);
        neighbors.push_back(cells[0][j]);
        neighbors.push_back(cells[0][0]);

        return neighbors;
    }
    else if(i == 0 && j == 19){ // top right corner
        neighbors.push_back(cells[9][j-1]);
        neighbors.push_back(cells[9][j]);
        neighbors.push_back(cells[9][19]);

        neighbors.push_back(cells[i][j-1]);
        neighbors.push_back(cells[i][0]);

        neighbors.push_back(cells[i+1][j-1]);
        neighbors.push_back(cells[i+1][j]);
        neighbors.push_back(cells[i+1][0]);

        return neighbors;
    }
    else if(i == 9 && j == 0){ // bottom left corner
        neighbors.push_back(cells[i-1][19]);
        neighbors.push_back(cells[i-1][j]);
        neighbors.push_back(cells[i-1][j+1]);

        neighbors.push_back(cells[i][19]);
        neighbors.push_back(cells[i][j+1]);

        neighbors.push_back(cells[0][19]);
        neighbors.push_back(cells[0][j]);
        neighbors.push_back(cells[0][j+1]);

        return neighbors;
    }
    else if(i == 0 && j != 0 && j != 19){  // top row, no corners
        neighbors.push_back(cells[9][j-1]);
        neighbors.push_back(cells[9][j]);
        neighbors.push_back(cells[9][j+1]);

        neighbors.push_back(cells[i][j-1]);
        neighbors.push_back(cells[i][j+1]);

        neighbors.push_back(cells[i+1][j-1]);
        neighbors.push_back(cells[i+1][j]);
        neighbors.push_back(cells[i+1][j+1]);

        return neighbors;
    }
    else if(i != 0 && i != 9 && j == 0){ // far right column, no corners
        neighbors.push_back(cells[i-1][19]);
        neighbors.push_back(cells[i-1][j]);
        neighbors.push_back(cells[i-1][j+1]);

        neighbors.push_back(cells[i][19]);
        neighbors.push_back(cells[i][j+1]);

        neighbors.push_back(cells[i+1][19]);
        neighbors.push_back(cells[i+1][j]);
        neighbors.push_back(cells[i+1][j+1]);

        return neighbors;
    }
    else if(i == 9 && j != 19 && j != 0){ // bottom row, no corners
        neighbors.push_back(cells[i-1][j-1]);
        neighbors.push_back(cells[i-1][j]);
        neighbors.push_back(cells[i-1][j+1]);

        neighbors.push_back(cells[i][j-1]);
        neighbors.push_back(cells[i][j+1]);

        neighbors.push_back(cells[0][j-1]);
        neighbors.push_back(cells[0][j]);
        neighbors.push_back(cells[0][j+1]);

        return neighbors;
    }
    else if(i != 0 && i != 9 && j == 19){ // far left column, no corners
        neighbors.push_back(cells[i-1][j-1]);
        neighbors.push_back(cells[i-1][j]);
        neighbors.push_back(cells[i-1][0]);

        neighbors.push_back(cells[i][j-1]);
        neighbors.push_back(cells[i][0]);

        neighbors.push_back(cells[i+1][j-1]);
        neighbors.push_back(cells[i+1][j]);
        neighbors.push_back(cells[i+1][0]);

        return neighbors;
    }
    else{ // no edges or corners
        neighbors.push_back(cells[i-1][j-1]);
        neighbors.push_back(cells[i-1][j]);
        neighbors.push_back(cells[i-1][j+1]);

        neighbors.push_back(cells[i][j-1]);
        neighbors.push_back(cells[i][j+1]);

        neighbors.push_back(cells[i+1][j-1]);
        neighbors.push_back(cells[i+1][j]);
        neighbors.push_back(cells[i+1][j+1]);
    }

    return neighbors;
}

/*
 checks the status of neighbor cells stored in temp array from getNeigbors() function
 @param = temp vector of neighbor cells
*/
int MainWindow::checkNeighbors(std::vector<game*> neighbors){
    int aliveCount = 0; // counter to track number of alive cells

    // Loop that runs 8 times, once for each neighbor cell and increments counter if a cell is alive
    for(int i = 0; i < neighbors.size(); i++){
        if(neighbors[i]->get_status()){
            aliveCount++;
        }
    }
    // these conditionals return status to be handled by the checkAlive function
    if(aliveCount < 2){ // less than two living neighbors (alive cell will die)
        return 1;
    }
    if(aliveCount == 2){ // 2 neighbors (alive cell will remain alive, dead cell remains dead)
        return 2;
    }
    if(aliveCount > 3){ // more than 3 neighbors (alive cell will die, dead stays dead)
        return 3;
    }
    if(aliveCount == 3){ // 3 neighbors (dead cell will become alive, living cells stay alive)
        return 4;
    }
}

/*
    - this function runs each turn and when step button is pressed
    - gets the neighbors for each cell and checks neighbor status
    - sets next tur status for each cell before making any changes
    - after each cell's next turn status is determined, preform step
        - kill or revive cells based on next turn status
    - after cells are changed, update graph

*/
void MainWindow::checkAlive()
{
    //qDebug() << "checking if cells are dead or alive";

    int status = 0;

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            status = checkNeighbors(getNeighbors(i, j));

            if(cells[i][j]->get_status()){
                if(status == 1 || status == 3){
                    // kill cell
                    cells[i][j]->set_nextStatus(0);
                }
                // if status == 2, do nothing
            }
            else{
                if(status == 4){
                    // make cell alive
                    cells[i][j]->set_nextStatus(1);
                }
            }
        }
    }
     // this debug statement lets us know that next status loop has finished
    qDebug() << "next status set";

    // this loop actually preforms current turn after statuses are set
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            if(cells[i][j]->get_nextStatus() == 1){ // if nextStatus is 1, current cell becomes alive
                cells[i][j]->revive(newGameColor);
            }
            else if(cells[i][j]->get_nextStatus() == 0){ // if next status is 0, current cell dies
                cells[i][j]->kill();
            }
            cells[i][j]->set_nextStatus(-1); // set cell status to -1 for next turn
        }
    }
    updateGraph();
}


//  Graph update logic, called every turn
void MainWindow::updateGraph(){
    MakePopGraph_->update(); //update our graph for population every time we update cells
    int w = 0;
    if(popBar_.size() > 20) //start moving to the left as graph keeps going
    {
        int prev = 0; //prev bar
        for(Bar* bar : popBar_){ //for the graph
            bar->set_x(-1 * bar->get_width()); //x is our width
            prev = bar->get_x(); //set prev to x
        }
        w = prev;
    }
    else{
        w = turnCounter_ * 30;
    }
    double pop_percent = (double(population_) / 200.0); //population as a percent
    int barHeight = int(pop_percent * h_bar);
    QColor color;
    if(-(prev_bar_->getHeight()) < barHeight ){
        color.setRgb(0,200,0);
    }
    else{
        color.setRgb(200,0,0);
    }
    Bar* bar = new Bar(w + 30, y_bar, barHeight, color); //making new bar with num turns as x
    popBar_.push_back(bar); //pushing bar onto vector
    MakePopGraph_->addItem(bar); //adding it to the ui
    prev_bar_ = bar;
}


// this updates population counter if user revives or kills a cell by clicking
// called from mousePressEvent
void MainWindow::clickCellSlot(game * cell){
    if(cell->get_status()){
        increasePopulation();
    }
    else{
        decreasePopulation();
    }
}

//button for restarting game
void MainWindow::on_restartButton_click(){
    qDebug() << "Restart Game Button Clicked";
    if(start_){
        timer->stop();
        start_ = false;
    }
    popBar_.clear();
    timer->~QTimer();
    ui->~MainWindow();
    initTimer();
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    QColor color(r, g, b);
    newGameColor = color;
    createGameGrid(color);

}

// slot for play button: starts or resumes timer
void MainWindow::on_playButton_click(){
    qDebug() << "Play button clicked!";

    if(!start_){
        timer->start();
        start_ = true;
    }
}

// slot for pause button: stops timer
void MainWindow::on_pauseButton_click(){
    qDebug() << "Pause button clicked!";
    if(start_){
        timer->stop();
        start_ = false;
    }
}

// slot for step button: advances game by one step
// this slot is also called every step when timer is running
//  - connected to the reviveCell and killCell signals
void MainWindow::on_stepButton_click(){
    //qDebug() << "Step button clicked!";
    checkAlive();
    turnCount();
}

