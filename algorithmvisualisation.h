#ifndef ALGORITHMVISUALISATION_H
#define ALGORITHMVISUALISATION_H

#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class AlgorithmVisualisation; }
QT_END_NAMESPACE

enum mode{sorting, pathfinding};

class AlgorithmVisualisation : public QMainWindow
{
    Q_OBJECT

public:
    AlgorithmVisualisation(QWidget *parent = nullptr);
    ~AlgorithmVisualisation();

private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::AlgorithmVisualisation *ui;
    mode currentMode = sorting;
};
#endif // ALGORITHMVISUALISATION_H
