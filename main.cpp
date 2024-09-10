#include <iostream>

#include "board.hpp"


int negamax(Board node, int depth, int &bestPosition)  // el uso & permite acceder a la variable original
{
    int maxDepth = 16;
    if(node.endGame() || depth == maxDepth)
        return node.evaluate(depth);

    int bestValue= -10000, dummy;  // dummy: variable para descartar las posiciones en las llamadas recursivas
    // Iteramos sobre todas las posibles jugadas legales
    for (int position : node.generateAllLegalMoves()) {
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -negamax(child, depth+1, dummy);
        if (value > bestValue) {
            bestValue = value;
            bestPosition = position;
        }
    }
    return bestValue;
}


int alphabeta(Board node, int depth, int alpha, int beta, int &bestPosition)
{
    int maxDepth = 16;
    if(node.endGame() || depth == maxDepth)
        return node.evaluate(depth);

    int bestValue= -10000, dummy;  // dummy: variable para descartar las posiciones en las llamadas recursivas
    // Iteramos sobre todas las posibles jugadas legales
    for (int position : node.generateAllLegalMoves()) {
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -alphabeta(child, depth+1, -beta, -alpha, dummy);
        if (value > bestValue) {
            bestValue = value;
            bestPosition = position;
            if (value > alpha) alpha = value;
            if (alpha >= beta) break;
        }
    }
    return bestValue;
}

int main()
{
    int depth = 0, bestPosition = -1;

    // Para realizar una jugada
    // se definen los siguientes valores para cada casilla
    // 0  | 1  | 2  | 3
    // 4  | 5  | 6  | 7
    // 8  | 9  | 10 | 11
    // 12 | 13 | 14 | 15

    // Pruebas de juego: humano vs negamax/alphabeta
    std::cout << "Que comience el juego!" << std::endl;
    Board newBoard;
    while (!newBoard.endGame()) {
        int position;
        if (newBoard.getActiveTurn() == X) {
            //negamax(newBoard, depth, bestPosition);
            alphabeta(newBoard, depth, -10000000, 10000000, bestPosition);
            position = bestPosition;
        } else {
            std::cout << "Ingresa una posición: ";
            std::cin >> position;
        }
        newBoard.makeMove(position);
        newBoard.print();
    }

    // Comprobar resultado
    if (newBoard.hasXWon())
        std::cout << "X ha ganado" << std::endl;
    else if (newBoard.hasOWon())
        std::cout << "O ha ganado" << std::endl;
    else if (newBoard.isFull())
        std::cout << "Empate" << std::endl;

    return 0;
}