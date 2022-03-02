#include "pathfindingtab.h"
#include "ui_pathfindingtab.h"

PathfindingTab::PathfindingTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathfindingTab)
{
    ui->setupUi(this);
    pathfindingScene = new QGraphicsScene(this);
    ui->pathfindingGraphicsView->setScene(pathfindingScene);
}

PathfindingTab::~PathfindingTab()
{
    delete ui;
}
