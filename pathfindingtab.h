#ifndef PATHFINDINGTAB_H
#define PATHFINDINGTAB_H

#include <QWidget>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Ui {
class PathfindingTab;
}

class PathfindingTab : public QWidget
{
    Q_OBJECT

public:
    explicit PathfindingTab(QWidget *parent = nullptr);
    ~PathfindingTab();

private:
    Ui::PathfindingTab *ui;
    QGraphicsScene *pathfindingScene;
};

#endif // PATHFINDINGTAB_H
