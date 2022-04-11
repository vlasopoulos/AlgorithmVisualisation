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

    xSize = 65;
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
}

void PathfindingTab::calculateSizes() {
//    width = ui->pathfindingGraphicsView->size().width();
    width = pathfindingScene->width();
//    cellSideSize = width/xSize;
    cellSideSize = pathfindingScene->height()/ySize;
    if (cellSideSize*xSize>width){
        cellSideSize = pathfindingScene->width()/xSize;
    }
//    cellSideSize = ui->pathfindingGraphicsView->size().height()/ySize;
//    centerOffsetX = (ui->pathfindingGraphicsView->size().width() - xSize*cellSideSize)/2;
//    centerOffsetY = (ui->pathfindingGraphicsView->size().height() - ySize*cellSideSize)/2;
    centerOffsetX = (pathfindingScene->width() - xSize*cellSideSize)/2;
    centerOffsetY = (pathfindingScene->height() - ySize*cellSideSize)/2;
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
            } else if (cells[row][col]->getState() == VISITED) {
                backgroundBrush.setColor(Qt::yellow);
            } else if (cells[row][col]->getState() == PATH) {
                backgroundBrush.setColor(Qt::magenta);
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

void PathfindingTab::disableButtons()
{
    ui->setEndButton->setDisabled(true);
    ui->setStartButton->setDisabled(true);
    ui->sizeXspinBox->setDisabled(true);
    ui->generateMazeButton->setDisabled(true);
    ui->runButton->setDisabled(true);
    pathfindingScene->removeEventFilter(this);

}

void PathfindingTab::enableButtons()
{
    ui->setEndButton->setDisabled(false);
    ui->setStartButton->setDisabled(false);
    ui->sizeXspinBox->setDisabled(false);
    ui->generateMazeButton->setDisabled(false);
    ui->runButton->setDisabled(false);
    pathfindingScene->installEventFilter(this);
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
    disableButtons();
    clearPathAndVisited();
    emit setStatusBarMessage("Searching for path.");
    if (startCell == endCell) {
        qInfo() << "Please set different start and end";
    } else {
        //run
        std::shared_ptr<Node> startingNode = std::make_shared<Node>(startCell,nullptr);
        auto endNode = Astar(startingNode);
        renderCells();
        if (endNode != nullptr){
            emit setStatusBarMessage("Found path!");
            findPath(endNode);
        } else {
            emit setStatusBarMessage("Path not found!");
        }
        renderCells();
    }
    enableButtons();
}


void PathfindingTab::clearPathAndVisited()
{
    for (int i=0; i<xSize; i++ ) {
        for (int j=0; j<ySize; j++) {
            if (cells[i][j]->getState() == VISITED || cells[i][j]->getState() == PATH) {
                cells[i][j]->setState(UNVISITED);
            }
        }
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

void PathfindingTab::on_animateMazeGenerationCheckBox_stateChanged(int arg1)
{
    if (arg1 == 0) {
        generateMazeAnimation = false;
    } else if (arg1 == 1) {
        generateMazeAnimation = true;
    } else {
        generateMazeAnimation = true;
    }
}

//Algorithms

std::vector<std::unique_ptr<Cell> > PathfindingTab::getChildrenForMaze(std::unique_ptr<Cell> &cell, cellState state) // default state unvisited
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
    disableButtons();
    emit setStatusBarMessage("Generating maze...");
    populateAllWalls();
    int currentRow = 1;
    int currentCol = 1;
    cells[currentRow][currentCol]->setState(UNVISITED);
    int numVisited = 1;

    while (numVisited < ((xSize)/2) * ((ySize)/2)) {
        auto neighbours = getChildrenForMaze(cells[currentRow][currentCol],WALL);

        if (generateMazeAnimation && neighbours.size() > 0) {
            std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
            renderCells();
            l->processEvents();
        }

        if (neighbours.size() == 0) {
            auto tempNeighbours = getChildrenForMaze(cells[currentRow][currentCol]); // find unvisited
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
    }
    cells[1][1]->setState(START);
    startCell = std::make_pair(1,1);
    if (cells[xSize-1][ySize-1]->getState() == UNVISITED) {
        cells[xSize-1][ySize-1]->setState(END);
        endCell = std::make_pair(xSize-1, ySize-1);
    }
    else if (cells[xSize-1][ySize-2]->getState() == UNVISITED) {
        cells[xSize-1][ySize-2]->setState(END);
        endCell = std::make_pair(xSize-1, ySize-2);
    }
    else if (cells[xSize-2][ySize-1]->getState() == UNVISITED) {
        cells[xSize-2][ySize-1]->setState(END);
        endCell = std::make_pair(xSize-2, ySize-1);
    }
    else if (cells[xSize-2][ySize-2]->getState() == UNVISITED) {
        cells[xSize-2][ySize-2]->setState(END);
        endCell = std::make_pair(xSize-2, ySize-2);
    }
    emit setStatusBarMessage("Maze generated succesfully.");
    renderCells();
    enableButtons();
}

//A*

PathfindingTab::Node::Node(std::pair<int,int> coordinates, std::shared_ptr<Node> parent)
{
    G = 0;
    H = 0;
    this->coordinates = coordinates;
    this->parent = parent;
}

bool PathfindingTab::Node::operator > (const Node n) const
{
    return H > n.H;
}

std::shared_ptr<PathfindingTab::Node> PathfindingTab::Astar(std::shared_ptr<Node> &initialNode)
{
    std::list<std::shared_ptr<Node>> frontier;

    if (initialNode->coordinates == endCell) return initialNode; // if is final state
    frontier.push_back(initialNode);

    while (frontier.size() > 0) {
        std::shared_ptr<Node> currentNode = frontier.front();
        if (cells[currentNode->coordinates.first][currentNode->coordinates.second]->getState() != START && cells[currentNode->coordinates.first][currentNode->coordinates.second]->getState() != END && cells[currentNode->coordinates.first][currentNode->coordinates.second]->getState() != VISITED){
            cells[currentNode->coordinates.first][currentNode->coordinates.second]->setState(VISITED);
            std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
            renderCells();
            l->processEvents();
        }
        frontier.remove(currentNode);
        if (currentNode->coordinates == endCell) return currentNode; // if is final state
        auto children = getChildrenForAStar(currentNode);
        for (auto &c : children){
            if (std::find(frontier.begin(),frontier.end(),c) == frontier.end()){
                frontier.push_back(c);
            }
        }
        frontier.sort(compareNodes);
    }
    return nullptr;
}

std::vector<std::shared_ptr<PathfindingTab::Node>> PathfindingTab::getChildrenForAStar(std::shared_ptr<Node> &node)
{
    std::vector<std::shared_ptr<PathfindingTab::Node>> children;
    if (node->coordinates.first - 1 > -1 && cells[node->coordinates.first - 1][node->coordinates.second]->getState() != WALL && cells[node->coordinates.first - 1][node->coordinates.second]->getState() != VISITED && cells[node->coordinates.first - 1][node->coordinates.second]->getState() != START) {
        children.push_back(std::make_shared<Node>(std::make_pair(node->coordinates.first-1,node->coordinates.second),node));
    }
    if (node->coordinates.second - 1 > -1 && cells[node->coordinates.first][node->coordinates.second - 1]->getState() != WALL && cells[node->coordinates.first][node->coordinates.second - 1]->getState() != VISITED && cells[node->coordinates.first][node->coordinates.second - 1]->getState() != START) {
        children.push_back(std::make_shared<Node>(std::make_pair(node->coordinates.first,node->coordinates.second - 1),node));
    }
    if (node->coordinates.first + 1 < xSize && cells[node->coordinates.first + 1][node->coordinates.second]->getState() != WALL && cells[node->coordinates.first + 1][node->coordinates.second]->getState() != VISITED && cells[node->coordinates.first + 1][node->coordinates.second]->getState() != START) {
        children.push_back(std::make_shared<Node>(std::make_pair(node->coordinates.first + 1,node->coordinates.second),node));
    }
    if (node->coordinates.second + 1 < ySize && cells[node->coordinates.first][node->coordinates.second + 1]->getState() != WALL && cells[node->coordinates.first][node->coordinates.second + 1]->getState() != VISITED && cells[node->coordinates.first][node->coordinates.second + 1]->getState() != START) {
        children.push_back(std::make_shared<Node>(std::make_pair(node->coordinates.first,node->coordinates.second + 1),node));
    }
    for (auto n : children) {
        n->G = node->G +1;
        n->H = calculateHeuristic(n) + n->G;
    }
    return children;
}

double PathfindingTab::calculateHeuristic(std::shared_ptr<Node> &node)
{
    //Euclidean
    return sqrt( pow(( endCell.first - node->coordinates.first),2) + pow((endCell.second - node->coordinates.second),2) )*10; // times 10 to be bigger than G
}

bool PathfindingTab::compareNodes(const std::shared_ptr<Node> &node1, const std::shared_ptr<Node> &node2)
{
    return (node1->H < node2->H);
}

void PathfindingTab::findPath(std::shared_ptr<Node> &endNode)
{
    auto currentNode = endNode;
    while (currentNode->parent != nullptr) {
     if (cells[currentNode->coordinates.first][currentNode->coordinates.second]->getState() != END) cells[currentNode->coordinates.first][currentNode->coordinates.second]->setState(PATH);
     currentNode = currentNode->parent;
     std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
     renderCells();
     l->processEvents();
    }
}

