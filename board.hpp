#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <vector>

enum MARK {X, O};

class Board
{
    uint32_t board[2];
    MARK turn;

    uint32_t oneMask = 1;
    uint32_t fullMask = 0xFFFF;

    bool isLegalMove(int position);  // Verificar si la posición es legal
    bool checkWin(uint32_t board);  //

    public:
        Board();  // Constructor prototype
        Board(uint32_t x, uint32_t o, MARK turn);
        ~Board();  // Destructor prototype

        int evaluate(int depth);  // Función de evaluación
        std::vector<int> generateAllLegalMoves();

        uint32_t getXBoard() const;
        uint32_t getOBoard() const;
        MARK getActiveTurn() const;

        void setActiveBitboard(uint32_t bitboard);

        void print();  // Función para imprimir el tablero en una forma legible
        bool makeMove(int position);  // Función para hacer una jugada
        void undoMove(int position);  // Función para deshacer una jugada

        bool hasXWon();  // Verificar si X ha ganado
        bool hasOWon();  // Verificar si O ha ganado
        bool isFull();  // Verificar si el tablero está lleno
        bool endGame();
};

#endif //BOARD_HPP
