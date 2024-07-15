#include "tetrispiece.h"

/**
 * @brief Sets the Tetris piece to a random shape.
 *
 * This method assigns a random shape to the Tetris piece.
 *
 * The range of shapes is defined to be between 1 and 7 (inclusive), corresponding to the
 * different possible Tetris shapes.
 */
void TetrisPiece::setRandomShape()
{
    setShape(TetrisShape(QRandomGenerator::global()->bounded(1, 8)));
}

/**
 * @brief Sets the Tetris piece to the specified shape.
 *
 * This method updates the Tetris piece's coordinates based on the given shape.
 * The coordinates are determined using a predefined table that maps each shape
 * to its corresponding coordinates.
 *
 * @param shape The shape to assign to the Tetris piece.
 */
void TetrisPiece::setShape(TetrisShape shape){

    static constexpr int coords_table[8][4][2]{
        { { 0, 0 },     { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },    { 0, 0 },   { 0, 1 },   { 0, 2 } }, // Vertical Line Shape
        { { -1, 0 },    { 0, 0 },   { 1, 0 },   { 0, 1 } }, // T Shape
        { { 0, 0 },     { 1, 0 },   { 0, 1 },   { 1, 1 } }, // Square Shape
        { { -1, 0 },    { 0, 0 },   { 0, 1 },   { 1, 1 } }, // Z Shape
        { { -1, 1 },    { 0, 1 },   { 0, 0 },   { 1, 0 } }, // S Shape - z mirrored
        { { 0, -1 },    { 0, 0 },   { 0, 1 },   { 1, 1 } }, // L Shape
        { { 0, -1 },    { 0, 0 },   { 0, 1 },   {-1, 1 } }, // J Shape - l mirrored
    };

    for (int i = 0; i<4; i++){
        for (int j = 0; j<2; j++){
            coords_[i][j] = coords_table[shape][i][j];
        }
    }

    piece_shape_ = shape;
}

/**
 * @brief Returns the minimum x-coordinate of the Tetris piece.
 *
 * This method calculates the minimum x-coordinate among the four blocks
 * of the Tetris piece.
 *
 * @return The minimum x-coordinate.
 */
int TetrisPiece::minX() const
{
    int min = coords_[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords_[i][0]);
    return min;
}

/**
 * @brief Returns the maximum x-coordinate of the Tetris piece.
 *
 * This method calculates the maximum x-coordinate among the four blocks
 * of the Tetris piece.
 *
 * @return The maximum x-coordinate.
 */
int TetrisPiece::maxX() const
{
    int max = coords_[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords_[i][0]);
    return max;
}

/**
 * @brief Returns the minimum y-coordinate of the Tetris piece.
 *
 * This method calculates the minimum y-coordinate among the four blocks
 * of the Tetris piece.
 *
 * @return The minimum y-coordinate.
 */
int TetrisPiece::minY() const
{
    int min = coords_[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords_[i][1]);
    return min;
}

/**
 * @brief Returns the maximum y-coordinate of the Tetris piece.
 *
 * This method calculates the maximum y-coordinate among the four blocks
 * of the Tetris piece.
 *
 * @return The maximum y-coordinate.
 */
int TetrisPiece::maxY() const
{
    int max = coords_[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords_[i][1]);
    return max;
}

/**
 * @brief Returns a TetrisPiece rotated 90 degrees to the right.
 *
 * This method creates a new TetrisPiece object that represents the current piece
 * rotated 90 degrees clockwise. If the current piece is a square shape, the method
 * returns a copy of the original piece without rotation.
 *
 * @return A new TetrisPiece object rotated 90 degrees to the right.
 */
TetrisPiece TetrisPiece::rotatedRight(){
    if(piece_shape_ == TetrisShape::SquareShape)
        return *this;

    TetrisPiece rotated_piece = *this;
    // Invert x, and y positions
    for(int i = 0; i < 4; i++){
        rotated_piece.setX(i, y(i));
        rotated_piece.setY(i, -x(i));
    }

    return rotated_piece;
}

/**
 * @brief Returns a TetrisPiece rotated 90 degrees to the left.
 *
 * This method creates a new TetrisPiece object that represents the current piece
 * rotated 90 degrees counterclockwise. If the current piece is a square shape, the method
 * returns a copy of the original piece without rotation.
 *
 * @return A new TetrisPiece object rotated 90 degrees to the left.
 */
TetrisPiece TetrisPiece::rotatedLeft(){
    if(piece_shape_ == TetrisShape::SquareShape)
        return *this;

    TetrisPiece rotated_piece = *this;
    // Invert x, and y positions
    for(int i = 0; i < 4; i++){
        rotated_piece.setX(i, -y(i));
        rotated_piece.setY(i, x(i));
    }

    return rotated_piece;
}

