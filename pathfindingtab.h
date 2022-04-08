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

signals:
    void windowResizedSignal();

private:
    Ui::PathfindingTab *ui;
    QGraphicsScene *pathfindingScene;
    std::vector<std::vector<std::unique_ptr<Cell>>> cells;
    void populateCells();
    void generateRandomStartAndEnd();
    void renderCells() const;
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *ob, QEvent *e);

    std::pair<int,int> startCell;
    std::pair<int,int> endCell;
    bool settingStartButton = false;
    bool settingEndButton = false;

    //helpers
    int xSize;
    int ySize;
    void calculateSizes();
    int width;
    int cellSideSize;
    double centerOffsetX;
    double centerOffsetY;
};

#endif // PATHFINDINGTAB_H
