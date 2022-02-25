#include "algorithmvisualisation.h"
#include "./ui_algorithmvisualisation.h"


AlgorithmVisualisation::AlgorithmVisualisation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlgorithmVisualisation)
{
    ui->setupUi(this);
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
        break;
        case 1:
            currentMode = pathfinding;
        break;
    }
}

