#include "algorithmvisualisation.h"
#include "./ui_algorithmvisualisation.h"


AlgorithmVisualisation::AlgorithmVisualisation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlgorithmVisualisation)
{
    ui->setupUi(this);
    connect(ui->sortingTab,&SortingTab::setStatusBarMessage,ui->statusbar,&QStatusBar::showMessage);
    connect(ui->pathfindingTab,&PathfindingTab::setStatusBarMessage,ui->statusbar,&QStatusBar::showMessage);
}

AlgorithmVisualisation::~AlgorithmVisualisation()
{
    delete ui;
}

//select mode
void AlgorithmVisualisation::on_tabWidget_currentChanged(int index)
{
    switch (index) {
        case 0:
            currentMode = sorting;
            ui->statusbar->showMessage("Sorting tab");
        break;
        case 1:
            currentMode = pathfinding;
            ui->statusbar->showMessage("Pathfinding tab");
        break;
    }
}

