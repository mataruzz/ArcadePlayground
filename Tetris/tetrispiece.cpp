#include "tetrispiece.h"

TetrisPiece::TetrisPiece()
{}

void TetrisPiece::setRandomShape()
{
    setShape(TetrisShape(QRandomGenerator::global()->bounded(1, 8)));
}

void TetrisPiece::setShape(TetrisShape shape){

    static constexpr int coords_table[8][4][2]{
        { { 0, 0 },     { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },    { 0, 0 },   { 0, 1 },   { 0, 2 } }, // Vertical Line Shape
        { { -1, 0 },    { 0, 0 },   { 1, 0 },   { 0, 1 } }, // T Shape
        { { 0, 0 },     { 1, 0 },   { 0, 1 },   { 1, 1 } }, // Square Shape
        { { -1, 0 },    { 0, 0 },   { 0, 1 },   { 1, 1 } }, // Z Shape
        { { -1, 1 },    { 0, 1 },   { 0, 0},   { 1, 0 } }, // S Shape - z mirrored
        { { 0, -1 },    { 0, 0 },   { 0, 1},   { 1, 1 } }, // L Shape
        { { 0, -1 },    { 0, 0 },   { 0, 1 },   {-1, 1 } }, // J Shape - l mirrored
    };

    for (int i = 0; i<4; i++){
        for (int j = 0; j<2; j++){
            coords_[i][j] = coords_table[shape][i][j];
        }
    }

    piece_shape_ = shape;
}


int TetrisPiece::minX() const
{
    int min = coords_[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords_[i][0]);
    return min;
}

int TetrisPiece::maxX() const
{
    int max = coords_[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords_[i][0]);
    return max;
}

int TetrisPiece::minY() const
{
    int min = coords_[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords_[i][1]);
    return min;
}

int TetrisPiece::maxY() const
{
    int max = coords_[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords_[i][1]);
    return max;
}
