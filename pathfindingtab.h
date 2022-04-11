#ifndef PATHFINDINGTAB_H
#define PATHFINDINGTAB_H

#include <QWidget>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QEvent>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>

#include <algorithm>
#include <random>
#include <set>

#include "algorithmvisualisation.h"
#include "cell.h"

namespace Ui {
class PathfindingTab;
}

class PathfindingTab : public QWidget
{
    Q_OBJECT

public:
    explicit PathfindingTab(QWidget *parent = nullptr);
    ~PathfindingTab();

private slots:
    void windowResizedSlot();
    void on_setStartButton_clicked();
    void on_setEndButton_clicked();
    void on_runButton_clicked();
    void on_sizeXspinBox_valueChanged(int arg1);

    void on_generateMazeButton_clicked();

    void on_animateMazeGenerationCheckBox_stateChanged(int arg1);

signals:
    void windowResizedSignal();
    void setStatusBarMessage(QString message, int timeout = 0);

private:
    Ui::PathfindingTab *ui;
    QGraphicsScene *pathfindingScene;
    std::vector<std::vector<std::unique_ptr<Cell>>> cells;
    void populateCells();
    void generateRandomStartAndEnd();
    void renderCells() const;
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *ob, QEvent *e);
    void disableButtons();
    void enableButtons();
    void clearPathAndVisited();

    std::pair<int,int> startCell;
    std::pair<int,int> endCell;
    bool settingStartButton = false;
    bool settingEndButton = false;
    bool generateMazeAnimation = true;

    //helpers
    int xSize;
    int ySize;
    void calculateSizes();
    int width;
    int cellSideSize;
    double centerOffsetX;
    double centerOffsetY;

    //algorithms
    void populateAllWalls();
    std::vector<std::unique_ptr<Cell>> getChildrenForMaze(std::unique_ptr<Cell> &cell, cellState state = UNVISITED);
    void AldousBroder();
    //A*
    struct Node {
        uint G;
        double H;
        std::pair<int,int> coordinates;
        std::shared_ptr<Node> parent;
        Node(std::pair<int,int> coordinates, std::shared_ptr<Node> parent = nullptr);
        bool operator > ( const Node n) const;
    };
    std::shared_ptr<Node> Astar(std::shared_ptr<Node> &initialNode);
    std::vector<std::shared_ptr<Node>> getChildrenForAStar(std::shared_ptr<Node> &node);
    double calculateHeuristic(std::shared_ptr<Node> &node);
    static bool compareNodes(const std::shared_ptr<Node> &node1, const std::shared_ptr<Node> &node2);
    void findPath(std::shared_ptr<Node> &endNode);
};

#endif // PATHFINDINGTAB_H
