#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <vector>

enum MARK {X, O};

class Board
{
    uint64_t board[2]{};
    MARK turn;

    uint64_t oneMask = 0b1;
    uint64_t fullMask = 0xFFFFFFFFFFFF;

    bool isLegalMove(int position) const;  // Verificar si la posición es legal
    static bool checkWin(uint64_t board);  //

    public:
        Board();  // Constructor prototype
        Board(uint64_t x, uint64_t o, MARK turn);
        ~Board();  // Destructor prototype

        int evaluate(int depth);  // Función de evaluación
        int countAlmostWinningLines(uint64_t playerBoard);
        std::vector<int> generateAllLegalMoves() const;

        uint64_t getXBoard() const;
        uint64_t getOBoard() const;
        MARK getActiveTurn() const;

        void print() const;  // Función para imprimir el tablero en una forma legible
        bool makeMove(int position);  // Función para hacer una jugada

        bool hasXWon() const;  // Verificar si X ha ganado
        bool hasOWon() const;  // Verificar si O ha ganado
        bool isFull() const;  // Verificar si el tablero está lleno
        bool endGame() const;

        bool operator==(const Board &other) const {
            return turn==other.turn && board[X]==other.board[X] && board[O]==other.board[O];
        }
};

struct BoardHash
{
    std::size_t operator()(Board const& board) const noexcept
    {
        std::size_t h1 = std::hash<uint64_t>{}(board.getXBoard());
        std::size_t h2 = std::hash<uint64_t>{}(board.getOBoard());
        std::size_t h3 = std::hash<bool>{}(board.getActiveTurn() == X);

        // Mezclar usando XOR y desplazamientos
        return h1 ^ (h2 << 1) ^ (h2 >> 1) ^ (h3 << 2) ^ (h3 >> 2);
    }
};

#endif //BOARD_HPP
