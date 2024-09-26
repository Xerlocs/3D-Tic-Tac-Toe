#include <iostream>

#include "board.hpp"

Board::Board()
{
    // Bitboards para cada jugador.
    // Usando valores binarios, inicialmente vacíos.
    board[X] = 0; // Bitboard para X
    board[O] = 0; // Bitboard para O
    turn = X;  // Comienza jugando X
}

Board::Board(const uint64_t x, const uint64_t o, const MARK turn) : board{x, o}, turn(turn) {}

Board::~Board() = default;

uint64_t Board::getXBoard() const { return board[X]; }
uint64_t Board::getOBoard() const { return board[O]; }
MARK Board::getActiveTurn() const { return turn; }

int Board::evaluate(const int depth)
{
    /* Si ha ganado, devolvemos un puntaje positivo
     * Se resta la profundidad para penalizar las victorias que ocurren en niveles más profundos.
     * Esto hace que el algoritmo prefiera ganar más rápidamente si es posible.
     */
    if (hasXWon())
     std::cout << "X won, evaluation at depth " << depth << std::endl;
        return (turn == X) ? (10 - depth) : (depth - 10);  // Si 'X' ha ganado, es bueno para el maximizador (X)
    if (hasOWon())
     std::cout << "O won, evaluation at depth " << depth << std::endl;
        return (turn == O) ? (10 - depth) : (depth - 10); // Si 'O' ha ganado, es bueno para el minimizador (O)
    if (isFull())
     std::cout << "Board is full, evaluation: 0 " << std::endl;
        return 0;  // Empate
}

std::vector<int> Board::generateAllLegalMoves() const
{
    std::vector<int> legalMoves;
    for (int i = 0; i < 64; i++)
    {
        if (isLegalMove(i))
        {
            legalMoves.push_back(i);
            std::cout << "Legal move found at position " << i << std::endl;
        }
    }
    if (legalMoves.empty()) {
    std::cout << "No legal moves found!" << std::endl;
    }
    return legalMoves;
}

bool Board::isLegalMove(const int position) const
{
    if (position < 0 || position >= 64)  // posicion dentro del rango?
        return false;
    if ((board[X] | board[O]) & (oneMask << position))  // posicion es vacia?
        return false;
    return true;
}

bool Board::makeMove(const int position)
{
    if (isLegalMove(position)) {
        std::cout << "Player " << (turn == X ? "X" : "O") << " makes move at position " << position << std::endl;
        board[turn] |= (oneMask << position);
        turn = 1-turn == 0 ? X : O;  // cambio de turno
        return true;
    }
     std::cout << "Illegal move attempted at position " << position << std::endl;
    return false;
}

bool Board::checkWin(uint64_t board)
{
    uint64_t winningBoards[56] = {
        //Filas por nivel
        0b0000000000000000000000000000000000000000000000000000000000001111ULL,  // Fila 1, Nivel 0 (posiciones 0, 1, 2, 3)
        0b0000000000000000000000000000000000000000000000000000000011110000ULL,  // Fila 2, Nivel 0 (posiciones 4, 5, 6, 7)
        0b0000000000000000000000000000000000000000000000000000111100000000ULL,  // Fila 3, Nivel 0 (posiciones 8, 9, 10, 11)
        0b0000000000000000000000000000000000000000000000001111000000000000ULL,  // Fila 4, Nivel 0 (posiciones 12, 13, 14, 15)
        0b0000000000000000000000000000000000000000000011110000000000000000ULL,  // Fila 1, Nivel 1 (posiciones 16, 17, 18, 19)
        0b0000000000000000000000000000000000000000111100000000000000000000ULL,  // Fila 2, Nivel 1 (posiciones 20, 21, 22, 23)
        0b0000000000000000000000000000000000001111000000000000000000000000ULL,  // Fila 3, Nivel 1 (posiciones 24, 25, 26, 27)
        0b0000000000000000000000000000000011110000000000000000000000000000ULL,  // Fila 4, Nivel 1 (posiciones 28, 29, 30, 31)
        0b0000000000000000000000000000111100000000000000000000000000000000ULL,  // Fila 1, Nivel 2 (posiciones 32, 33, 34, 35)
        0b0000000000000000000000001111000000000000000000000000000000000000ULL,  // Fila 2, Nivel 2 (posiciones 36, 37, 38, 39)
        0b0000000000000000000011110000000000000000000000000000000000000000ULL,  // Fila 3, Nivel 2 (posiciones 40, 41, 42, 43)
        0b0000000000000000111100000000000000000000000000000000000000000000ULL,  // Fila 4, Nivel 2 (posiciones 44, 45, 46, 47)
        0b0000000000001111000000000000000000000000000000000000000000000000ULL,  // Fila 1, Nivel 3 (posiciones 48, 49, 50, 51)
        0b0000000011110000000000000000000000000000000000000000000000000000ULL,  // Fila 2, Nivel 3 (posiciones 52, 53, 54, 55)
        0b0000111100000000000000000000000000000000000000000000000000000000ULL,  // Fila 3, Nivel 3 (posiciones 56, 57, 58, 59)
        0b1111000000000000000000000000000000000000000000000000000000000000ULL,  // Fila 4, Nivel 3 (posiciones 60, 61, 62, 63)
        //Columnas por nivel
        0b0000000000000000000000000000000000000000000000000001000100010001ULL,  // Columna 1, Nivel 0 (posiciones 0, 4, 8, 12)
        0b0000000000000000000000000000000000000000000000000010001000100010ULL,  // Columna 2, Nivel 0 (posiciones 1, 5, 9, 13)
        0b0000000000000000000000000000000000000000000000000100010001000100ULL,  // Columna 3, Nivel 0 (posiciones 2, 6, 10, 14)
        0b0000000000000000000000000000000000000000000000001000100010001000ULL,  // Columna 4, Nivel 0 (posiciones 3, 7, 11, 15)
        0b0000000000000000000000000000000000010001000100010000000000000000ULL,  // Columna 1, Nivel 1 (posiciones 16, 20, 24, 28)
        0b0000000000000000000000000000000000100010001000100000000000000000ULL,  // Columna 2, Nivel 1 (posiciones 17, 21, 25, 29)
        0b0000000000000000000000000000000001000100010001000000000000000000ULL,  // Columna 3, Nivel 1 (posiciones 18, 22, 26, 30)
        0b0000000000000000000000000000000010001000100010000000000000000000ULL,  // Columna 4, Nivel 1 (posiciones 19, 23, 27, 31)
        0b0000000000000000000100010001000100000000000000000000000000000000ULL,  // Columna 1, Nivel 2 (posiciones 32, 36, 40, 44)
        0b0000000000000000001000100010001000000000000000000000000000000000ULL,  // Columna 2, Nivel 2 (posiciones 33, 37, 41, 45)
        0b0000000000000000010001000100010000000000000000000000000000000000ULL,  // Columna 3, Nivel 2 (posiciones 34, 38, 42, 46)
        0b0000000000000000100010001000100000000000000000000000000000000000ULL,  // Columna 4, Nivel 2 (posiciones 35, 39, 43, 47)
        0b0001000100010001000000000000000000000000000000000000000000000000ULL,  // Columna 1, Nivel 3 (posiciones 48, 52, 56, 60)
        0b0010001000100010000000000000000000000000000000000000000000000000ULL,  // Columna 2, Nivel 3 (posiciones 49, 53, 57, 61)
        0b0100010001000100000000000000000000000000000000000000000000000000ULL,  // Columna 3, Nivel 3 (posiciones 50, 54, 58, 62)
        0b1000100010001000000000000000000000000000000000000000000000000000ULL,  // Columna 4, Nivel 3 (posiciones 51, 55, 59, 63)
        //Diagonales dentro de cada nivel
        0b0000000000000000000000000000000000000000000000001000010000100001ULL, // Nivel 0, Diagonal 1 (posiciones 0, 5, 10, 15)
        0b0000000000000000000000000000000000000000000000000001001001001000ULL, // Nivel 0, Diagonal 2 (posiciones 3, 6, 9, 12)
        0b0000000000000000000000000000000010000100001000010000000000000000ULL, // Nivel 1, Diagonal 1 (posiciones 16, 21, 26, 31)
        0b0000000000000000000000000000000000010010010010000000000000000000ULL, // Nivel 1, Diagonal 2 (posiciones 19, 22, 25, 28)
        0b0000000000000000100001000010000100000000000000000000000000000000ULL, // Nivel 2, Diagonal 1 (posiciones 32, 37, 42, 47)
        0b0000000000000000000100100100100000000000000000000000000000000000ULL, // Nivel 2, Diagonal 2 (posiciones 35, 38, 41, 44)
        0b1000010000100001000000000000000000000000000000000000000000000000ULL, // Nivel 3, Diagonal 1 (posiciones 48, 53, 58, 63)
        0b0001001001001000000000000000000000000000000000000000000000000000ULL, // Nivel 3, Diagonal 2 (posiciones 51, 54, 57, 60)
        //Pilares
        0b0000000000000001000000000000000100000000000000010000000000000001ULL, // Posiciones 0, 16, 32, 48
        0b0000000000000010000000000000001000000000000000100000000000000010ULL, // Posiciones 1, 17, 33, 49
        0b0000000000000100000000000000010000000000000001000000000000000100ULL, // Posiciones 2, 18, 34, 50
        0b0000000000001000000000000000100000000000000010000000000000001000ULL, // Posiciones 3, 19, 35, 51
        0b0000000000010000000000000001000000000000000100000000000000010000ULL, // Posiciones 4, 20, 36, 52
        0b0000000000100000000000000010000000000000001000000000000000100000ULL, // Posiciones 5, 21, 37, 53
        0b0000000001000000000000000100000000000000010000000000000001000000ULL, // Posiciones 6, 22, 38, 54
        0b0000000010000000000000001000000000000000100000000000000010000000ULL, // Posiciones 7, 23, 39, 55
        0b0000000100000000000000010000000000000001000000000000000100000000ULL, // Posiciones 8, 24, 40, 56
        0b0000001000000000000000100000000000000010000000000000001000000000ULL, // Posiciones 9, 25, 41, 57
        0b0000010000000000000001000000000000000100000000000000010000000000ULL, // Posiciones 10, 26, 42, 58
        0b0000100000000000000010000000000000001000000000000000100000000000ULL, // Posiciones 11, 27, 43, 59
        0b0001000000000000000100000000000000010000000000000001000000000000ULL, // Posiciones 12, 28, 44, 60
        0b0010000000000000001000000000000000100000000000000010000000000000ULL, // Posiciones 13, 29, 45, 61
        0b0100000000000000010000000000000001000000000000000100000000000000ULL, // Posiciones 14, 30, 46, 62
        0b1000000000000000100000000000000010000000000000001000000000000000ULL // Posiciones 15, 31, 47, 63
    };
    for (uint64_t wp : winningBoards)
        if ((board & wp) == wp) return true;
    return false;
}

bool Board::hasXWon() const { return checkWin(board[X]); }
bool Board::hasOWon() const { return checkWin(board[O]); }
bool Board::isFull() const { return (board[X] | board[O]) == fullMask; }
bool Board::endGame() const { 
    std::cout << "Checking endGame: hasXWon() = " << hasXWon() 
              << ", hasOWon() = " << hasOWon() 
              << ", isFull() = " << isFull() << std::endl;
    return hasXWon() || hasOWon() || isFull(); 
    }

void Board::print() const {
    for (int level = 0; level < 4; ++level) {
        std::cout << "Nivel " << level + 1 << ":\n";
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                int pos = level * 16 + row * 4 + col;
                if (board[X] & (oneMask << pos)) {
                    std::cout << "X ";
                } else if (board[O] & (oneMask << pos)) {
                    std::cout << "O ";
                } else {
                    std::cout << "- ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
