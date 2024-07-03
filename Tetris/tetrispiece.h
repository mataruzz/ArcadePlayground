#ifndef TETRISPIECE_H
#define TETRISPIECE_H

#include <QRandomGenerator>

enum TetrisShape{
    NoShape,
    LineShape,
    TShape,
    SquareShape,
    ZShape,
    SShape,     // Z mirrored
    LShape,
    JShape      // L mirrored
};

class TetrisPiece
{
public:
    TetrisPiece();
    void setRandomShape();
    void setShape(TetrisShape shape);

    TetrisShape shape() const { return piece_shape_; }
    int x(int index) const { return coords_[index][0]; }
    int y(int index) const { return coords_[index][1]; }
    int minX() const;
    int maxX() const;
    int minY() const;
    int maxY() const;

private:
    void setX(int index, int x) { coords_[index][0] = x; }
    void setY(int index, int y) { coords_[index][1] = y; }

    int coords_[4][2];
    TetrisShape piece_shape_;
    int num_shapes_;
};

#endif // TETRISPIECE_H
