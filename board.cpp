#include <iostream>

#include "board.hpp"

Board::Board()
{
    // Bitboards para cada jugador.
    // Usando valores binarios, inicialmente vacíos.
    board[X] = 0b0000000000000000; // Bitboard para X (16 posiciones)
    board[O] = 0b0000000000000000; // Bitboard para O
    turn = X;  // Comienza jugando X
}

Board::Board(uint32_t x, uint32_t o, MARK turn): board{x, o}, turn(turn) {}

Board::~Board() = default;

uint32_t Board::getXBoard() const { return board[X]; }
uint32_t Board::getOBoard() const { return board[O]; }
MARK Board::getActiveTurn() const { return turn; }

int Board::evaluate(int depth)
{
    /* Si ha ganado, devolvemos un puntaje positivo
     * Se resta la profundidad para penalizar las victorias que ocurren en niveles más profundos.
     * Esto hace que el algoritmo prefiera ganar más rápidamente si es posible.
     */
    if (hasXWon())
        return (turn == X) ? (10 - depth) : (depth - 10);  // Si 'X' ha ganado, es bueno para el maximizador (X)
    if (hasOWon())
        return (turn == O) ? (10 - depth) : (depth - 10); // Si 'O' ha ganado, es bueno para el minimizador (O)
    if (isFull())
        return 0;  // Empate
}

std::vector<int> Board::generateAllLegalMoves() {
    std::vector<int> legalMoves;
    for (int i = 0; i < 16; i++)  // Cambiado de 9 a 16
        if (isLegalMove(i))
            legalMoves.push_back(i);
    return legalMoves;
}

bool Board::isLegalMove(int position)
{
    if (position < 0 || position > 15)  // posicion dentro del rango?
        return false;
    if ((board[X] | board[O]) & (oneMask << position))  // posicion es vacia?
        return false;
    return true;
}

bool Board::makeMove(int position)
{
    if (isLegalMove(position)) {
        board[turn] |= (oneMask << position);
        turn = 1-turn == 0 ? X : O;  // cambio de turno
        return true;
    }
    return false;
}

void Board::undoMove(int position)
{
    board[turn] ^= (oneMask << position);
}

bool Board::checkWin(uint32_t board) {
    uint32_t winningBoards[10] = {
        0b1111000000000000,  // fila superior
        0b0000111100000000,  // fila segunda
        0b0000000011110000,  // fila tercera
        0b0000000000001111,  // fila inferior
        0b1000100010001000,  // columna izquierda
        0b0100010001000100,  // columna segunda
        0b0010001000100010,  // columna tercera
        0b0001000100010001,  // columna derecha
        0b1000010000100001,  // diagonal principal
        0b0001001001001000   // diagonal secundaria
    };
    for (uint32_t wp : winningBoards)
        if ((board & wp) == wp) return true;
    return false;
}

bool Board::hasXWon() { return checkWin(board[X]); }
bool Board::hasOWon() { return checkWin(board[O]); }

bool Board::isFull()
{
    return (board[X] | board[O]) == fullMask;
}

bool Board::endGame()
{
    return hasXWon() || hasOWon() || isFull();
}

void Board::print() {
    for (int i = 0; i < 16; i++) {  // 16 casillas ahora
        if (board[X] & (oneMask << i))
            std::cout << "X ";
        else if (board[O] & (oneMask << i))
            std::cout << "O ";
        else
            std::cout << "- ";
        if (i % 4 == 3) std::cout << std::endl;  // Cambio de fila después de cada 4 posiciones
    }
    std::cout << std::endl;
}
