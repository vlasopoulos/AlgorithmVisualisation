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
    void windowResizedSlot() const;
    void on_numberOfItemsSpinBox_valueChanged(const int arg1);
    void on_shuffleButton_clicked() const;
    void on_backgroundColorButton_clicked();
    void on_columnColorButton_clicked();
    void on_highlightColorButton_clicked();
    void on_resetColorsButton_clicked();

signals:
    void windowResizedSignal();

private:
    Ui::sortingTab *ui;
    QGraphicsScene *sortingScene;
    void renderData(const int* items,const int numberOfItems,const int green=-1) const;
    void render() const;
    void render(const int green) const;
    void resizeEvent(QResizeEvent *event);
    void shuffleNumbers() const;
    QColor getIdealTextColor(const QColor& rBackgroundColor) const;
    //sorting
    void swap(int *a, int *b);
    int quickSortPartition(int *arr, const int low, const int high);
    void quickSort(int *arr, const int low, const int high);
    void merge(int *array, const int low, const int mid, const int high);
    void mergeSort(int *array, const int low, const int high);
    void bubbleSort(int arr[], const int n);
    //variables
    int currentNumberOfItems;
    int* numbers = nullptr;
    QColor backgroundColor=Qt::black;;
    QColor columnColor=Qt::blue;;
    QColor highlightColor=Qt::green;

};

#endif // SORTINGTAB_H