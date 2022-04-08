#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QDebug>

enum cellState {
    UNVISITED,
    VISITED,
    START,
    END,
    WALL,
    PATH
};

class Cell : public QObject
{
    Q_OBJECT
public:
    explicit Cell(QObject *parent = nullptr, int x = 0, int y = 0);
    ~Cell();
    cellState getState() const;
    void setState(cellState newState);

    int getRow() const;
    int getColumn() const;

signals:

private:
    cellState state;
    int row;
    int column;
};

#endif // CELL_H
