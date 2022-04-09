#include "pathfindingtab.h"
#include "ui_pathfindingtab.h"

PathfindingTab::PathfindingTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathfindingTab)
{
    ui->setupUi(this);
    pathfindingScene = new QGraphicsScene(this);
    ui->pathfindingGraphicsView->setScene(pathfindingScene);
    connect(this, &PathfindingTab::windowResizedSignal, this, &PathfindingTab::windowResizedSlot);
    pathfindingScene->installEventFilter(this);

    xSize = 50;
    ySize = xSize/2.05;
    populateCells();
    generateRandomStartAndEnd();
    calculateSizes();
    renderCells();


}

PathfindingTab::~PathfindingTab()
{
    delete ui;
}

void PathfindingTab::windowResizedSlot()
{
    pathfindingScene->setSceneRect(0,0,ui->pathfindingGraphicsView->size().width()-2,ui->pathfindingGraphicsView->size().height()-2);
    calculateSizes();
    renderCells();
}

void PathfindingTab::populateCells()
{
    //clear vectors
    cells.clear();

    //calculate appropriate height

    //populate cells
    for (int r=0; r<xSize ; r++ ) {
        std::vector<std::unique_ptr<Cell>> column;
        for (int col=0; col<ySize; col++) {
            column.push_back(std::make_unique<Cell>(this, r, col));
        }
        cells.push_back(std::move(column));
    }
}

void PathfindingTab::generateRandomStartAndEnd()
{
    int x = QRandomGenerator::global()->bounded(xSize);
    int y = QRandomGenerator::global()->bounded(ySize);
    cells[x][y]->setState(START);
    startCell = std::make_pair(x,y);
    x = QRandomGenerator::global()->bounded(xSize);
    y = QRandomGenerator::global()->bounded(ySize);
    cells[x][y]->setState(END);
    endCell = std::make_pair(x,y);
    if (startCell == endCell) {
        generateRandomStartAndEnd();
    }
    qInfo() << startCell << endCell;
}

void PathfindingTab::calculateSizes() {
    width = ui->pathfindingGraphicsView->size().width();
    cellSideSize = width/xSize;
    centerOffsetX = (ui->pathfindingGraphicsView->size().width() - xSize*cellSideSize)/2;
    centerOffsetY = (ui->pathfindingGraphicsView->size().height() - ySize*cellSideSize)/2;
}




void PathfindingTab::renderCells() const
{
    pathfindingScene->clear();
    QPen outlinePen(Qt::black);
    QBrush backgroundBrush(Qt::white);
    for (int row=0; row<xSize ; row++ ) {
        for (int col=0; col<ySize ; col++ ) {
            if (cells[row][col]->getState() == START) {
                backgroundBrush.setColor(Qt::green);
            } else if (cells[row][col]->getState() == END) {
                backgroundBrush.setColor(Qt::red);
            } else if (cells[row][col]->getState() == WALL) {
                backgroundBrush.setColor(Qt::black);
            } else {
                backgroundBrush.setColor(Qt::white);
            }
            pathfindingScene->addRect(row*cellSideSize+centerOffsetX, col*cellSideSize+centerOffsetY, cellSideSize, cellSideSize, outlinePen, backgroundBrush);
        }
    }
}

void PathfindingTab::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    emit windowResizedSignal();
}

bool PathfindingTab::eventFilter(QObject *ob, QEvent *e)
{
     if ( ob == pathfindingScene ){
         QGraphicsSceneMouseEvent* event;
         cellState state = WALL;
         if (!settingStartButton && !settingEndButton){
             if(e->type() == QEvent::GraphicsSceneMousePress || e->type() == QEvent::GraphicsSceneMouseMove){
                 event= static_cast<QGraphicsSceneMouseEvent*>(e);
                 if (event->buttons() == Qt::LeftButton) state = WALL;
                 else if (event->buttons() == Qt::RightButton) state = UNVISITED;
                 int x = (event->scenePos().x() - centerOffsetX)/(cellSideSize);
                 int y = (event->scenePos().y() - centerOffsetY)/(cellSideSize);
                 if (x >= 0 && y >= 0 && x < xSize && y < ySize && event->scenePos().x() > centerOffsetX && event->scenePos().y() > centerOffsetY){
                     if (cells[x][y]->getState() != START && cells[x][y]->getState() != END)
                     cells[x][y]->setState(state);
                 }
                 renderCells();
             }
         } else if (settingStartButton){
             if(e->type() == QEvent::GraphicsSceneMousePress){
                 event= static_cast<QGraphicsSceneMouseEvent*>(e);
                 int x = (event->scenePos().x() - centerOffsetX)/(cellSideSize);
                 int y = (event->scenePos().y() - centerOffsetY)/(cellSideSize);
                 if (x >= 0 && y >= 0 && x < xSize && y < ySize &&  event->scenePos().x() > centerOffsetX && event->scenePos().y() > centerOffsetY){
                     if (cells[startCell.first][startCell.second]->getState() != END) cells[startCell.first][startCell.second]->setState(UNVISITED);
                     startCell.first = x;
                     startCell.second = y;
                     cells[x][y]->setState(START);
                 }
                 settingStartButton = false;
                 renderCells();
             }
         } else if (settingEndButton){
             if(e->type() == QEvent::GraphicsSceneMousePress){
                 event= static_cast<QGraphicsSceneMouseEvent*>(e);
                 int x = (event->scenePos().x() - centerOffsetX)/(cellSideSize);
                 int y = (event->scenePos().y() - centerOffsetY)/(cellSideSize);
                 if (x >= 0 && y >= 0 && x < xSize && y < ySize &&  event->scenePos().x() > centerOffsetX && event->scenePos().y() > centerOffsetY){
                     if (cells[endCell.first][endCell.second]->getState() != START) cells[endCell.first][endCell.second]->setState(UNVISITED);
                     endCell.first = x;
                     endCell.second = y;
                     cells[x][y]->setState(END);
                 }
                 settingEndButton = false;
                 renderCells();
             }
         }
    }
    return false;
}


void PathfindingTab::on_setStartButton_clicked()
{
        settingStartButton = true;
        ui->setStartButton->setDown(true);
}



void PathfindingTab::on_setEndButton_clicked()
{
    settingEndButton = true;
    ui->setEndButton->setDown(true);
}


void PathfindingTab::on_runButton_clicked()
{

    if (startCell == endCell) {
        qInfo() << "Please set different start and end";
    } else {
        //run

    }
}


void PathfindingTab::on_generateMazeButton_clicked()
{
    AldousBroder();
}


void PathfindingTab::on_sizeXspinBox_valueChanged(int arg1)
{
    xSize = arg1;
    ySize = xSize/2.05;
    calculateSizes();
    populateCells();
    generateRandomStartAndEnd();
    renderCells();
}

//Algorithms

std::vector<std::unique_ptr<Cell> > PathfindingTab::getChildren(std::unique_ptr<Cell> &cell, cellState state) // default state unvisited
{
    std::vector<std::unique_ptr<Cell>> neighbourCells;


    if (cell->getRow()>1 && cells[cell->getRow() -2][cell->getColumn()]->getState() == state) {
        neighbourCells.push_back(std::make_unique<Cell>(this, cell->getRow() - 2, cell->getColumn()));
    }
    if (cell->getRow()< xSize - 2 && cells[cell->getRow() + 2][cell->getColumn()]->getState() == state) {
        neighbourCells.push_back(std::make_unique<Cell>(this, cell->getRow() + 2, cell->getColumn()));
    }
    if (cell->getColumn()>1 && cells[cell->getRow()][cell->getColumn() - 2]->getState() == state) {
        neighbourCells.push_back(std::make_unique<Cell>(this, cell->getRow(), cell->getColumn() - 2));
    }
    if (cell->getColumn()< ySize - 2 && cells[cell->getRow()][cell->getColumn() + 2]->getState() == state) {
        neighbourCells.push_back(std::make_unique<Cell>(this, cell->getRow(), cell->getColumn() + 2));
    }
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(neighbourCells), std::end(neighbourCells), rng);
    return neighbourCells;
}

void PathfindingTab::populateAllWalls()
{
    for (auto &r : cells){
        for (auto &c : r) {
            c->setState(WALL);
        }
    }
}

void PathfindingTab::AldousBroder()
{
    populateAllWalls();
    int currentRow = 1;
    int currentCol = 1;
    cells[currentRow][currentCol]->setState(UNVISITED);
    int numVisited = 1;


    qInfo() << ((xSize)/2) * ((ySize)/2);
    while (numVisited < ((xSize)/2) * ((ySize)/2)) {
        auto neighbours = getChildren(cells[currentRow][currentCol],WALL);

        if (neighbours.size() > 0) {
            std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
            renderCells();
            l->processEvents();
        }

        if (neighbours.size() == 0) {
            auto tempNeighbours = getChildren(cells[currentRow][currentCol]); // find unvisited
//            int randomNeighbour = QRandomGenerator::global()->bounded(tempNeighbours.size());
//            currentRow = tempNeighbours[randomNeighbour]->getRow(); // mark random neighbor as current
//            currentCol = tempNeighbours[randomNeighbour]->getColumn(); // mark random neighbor as current
            currentRow = tempNeighbours[0]->getRow(); // mark random neighbor as current
            currentCol = tempNeighbours[0]->getColumn(); // mark random neighbor as current
            continue;
        }

        for (auto &n : neighbours){
            if (cells[n->getRow()][n->getColumn()]->getState() == WALL) {
                cells[(n->getRow() + currentRow) / 2][(n->getColumn() + currentCol)/2]->setState(UNVISITED); // open up wall to new neighbor
                cells[n->getRow()][n->getColumn()]->setState(UNVISITED); // mark neighbor as visited
                numVisited++; // bump the number visited
                currentRow = n->getRow(); //current becomes new neighbor
                currentCol = n->getColumn(); //current becomes new neighbor
                break; // break loop
            }
        }
        qInfo() << numVisited;
    }
    qInfo() << "Done generating maze";
    renderCells();
}

