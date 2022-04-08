#include "cell.h"

Cell::Cell(QObject *parent, int row, int column) : QObject(parent)
{
    this->row = row;
    this->column = column;
    this->state = UNVISITED;
}

Cell::~Cell()
{
}

cellState Cell::getState() const
{
    return state;
}

void Cell::setState(cellState newState)
{
    state = newState;
}

int Cell::getRow() const
{
    return row;
}

int Cell::getColumn() const
{
    return column;
}
