#include "tetrispiece.h"

TetrisPiece::TetrisPiece()
{}

void TetrisPiece::setShape(TetrisShape shape){

    static constexpr int coords_table[2][4][2]{
        { { 0, 0 },     { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },    { 0, 0 },   { 0, 1 },   { 0, 2 } }, // Vertical Line Shape
    /*    { { -1, 0 },    { 0, 0 },   { 1, 0 },   { 0, 1 } }, // T Shape
        { { 0, 0 },     { 1, 0 },   { 0, 1 },   { 1, 1 } }, // Square Shape
        { { -1, 0 },    { 0, 0 },   { 0, -1 },   { -1, 1 } }, // Z Shape
        { { -1, -1 },    { 0, -1 },   { 0, 0},   { 1, 1 } }, // S Shape - z mirrored

        { { 0, -2 },    { 0, -1 },   { 0, 0},   { 1, 0 } }, // L Shape
        { { -1, 0 },    { 0, 0 },   { 0, -1 },   { -1, 1 } },*/ // Square Shape
    };

    for (int i = 0; i<4; i++){
        for (int j = 0; j<2; j++){
            coords_[i][j] = coords_table[shape][i][j];
        }
    }

    piece_shape_ = shape;
}
