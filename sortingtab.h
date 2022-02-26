#ifndef SORTINGTAB_H
#define SORTINGTAB_H

#include <QWidget>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QEventLoop>
#include <QColorDialog>
#include <algorithm>
#include <random>
#include <iterator>
#include <chrono>
#include <thread>

namespace Ui {
class sortingTab;
}

class sortingTab : public QWidget
{
    Q_OBJECT

public:
    explicit sortingTab(QWidget *parent = nullptr);
    ~sortingTab();

private slots:
    void on_runSortingAlgorithmButton_clicked();
    void windowResizedSlot();
    void on_numberOfItemsSpinBox_valueChanged(int arg1);
    void on_shuffleButton_clicked();

    void on_backgroundColorButton_clicked();

    void on_columnColorButton_clicked();

    void on_highlightColorButton_clicked();

    void on_resetColorsButton_clicked();

signals:
    void windowResizedSignal();

private:
    Ui::sortingTab *ui;
    QGraphicsScene *sortingScene;
    void renderData(int* items, int numberOfItems, int green=-1);
    void render();
    void render(int green);
    void resizeEvent(QResizeEvent *event);
    void shuffleNumbers();
    void swap(int *a, int *b);
    int quickSortPartition(int *arr, int low, int high);
    void quickSort(int *arr, int low, int high);
    void merge(int *array, int low, int mid, int high);
    void mergeSort(int *array, int low, int high);
    void bubbleSort(int arr[], int n);
    //variables
    int currentNumberOfItems;
    int* numbers = nullptr;
    QColor backgroundColor=Qt::black;;
    QColor columnColor=Qt::blue;;
    QColor highlightColor=Qt::green;
    QColor getIdealTextColor(const QColor& rBackgroundColor) const;
};

#endif // SORTINGTAB_H
