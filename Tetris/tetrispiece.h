#ifndef TETRISPIECE_H
#define TETRISPIECE_H

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
    void setShape(TetrisShape shape);

private:
    int coords_[4][2];
    TetrisShape piece_shape_;
    int num_shapes_;
};

#endif // TETRISPIECE_H
