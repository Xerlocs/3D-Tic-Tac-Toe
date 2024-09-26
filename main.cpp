#include <iostream>

#include "board.hpp"
#include "TTEntry.h"

TranspositionTable TT;

int negamax(Board node, int depth, int &bestPosition)  // el uso & permite acceder a la variable original
{
    int maxDepth = 9;
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


int alphabeta(Board node, int maxDepth, int depth, int alpha, int beta, int &bestPosition)
{
    if(node.endGame() || depth == maxDepth)
        return node.evaluate(depth);

    int bestValue= -10000, dummy;  // dummy: variable para descartar las posiciones en las llamadas recursivas
    // Iteramos sobre todas las posibles jugadas legales
    for (int position : node.generateAllLegalMoves()) {
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -alphabeta(child, maxDepth, depth+1, -beta, -alpha, dummy);
        if (value > bestValue) {
            bestValue = value;
            bestPosition = position;
            if (value > alpha) alpha = value;
            if (alpha >= beta) break;
        }
    }
    return bestValue;
}

int iterativeDeepening(Board node, int maxDepth, int &bestPosition){
    int value;
    for(int depth = 0; depth < maxDepth; depth++) {
        value = alphabeta(node, depth, 0, -10000000, 10000000, bestPosition);
    }
    return value;
}

int alphabetaTT(Board node, int maxDepth, int depth, int alpha, int beta, int &bestPosition)
{
    TTEntry ttEntry = TT.get(node);
    if (ttEntry.isValid()){
        if(ttEntry.getDepth() >= depth) {
            if (ttEntry.isExact()){
                return ttEntry.getValue();
            }
            if (ttEntry.isLower()) {
                alpha = std::max(alpha, ttEntry.getValue());
            } else if (ttEntry.isUpper()){
                beta = std::min(beta, ttEntry.getValue());
            }
            if (alpha >= beta){
                return ttEntry.getValue();
            }
        }
    }

    if(node.endGame() || depth == maxDepth)
        return node.evaluate(depth);

    int bestValue= -10000, dummy;  // dummy: variable para descartar las posiciones en las llamadas recursivas
    int al = alpha;
    // Iteramos sobre todas las posibles jugadas legales
    for (int position : node.generateAllLegalMoves()) {
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -alphabeta(child, maxDepth, depth+1, -beta, -al, dummy);
        if (value > bestValue) {
            bestValue = value;
            bestPosition = position;
            if (value > al) al = value;
            if (al >= beta) break;
        }
    }
    bool lower = false, exact = false, upper = false;
    if (bestValue <= alpha)
        upper = true;
    else if (bestValue >= beta)
        lower = true;
    else
        exact = true;
    TT.store(TTEntry(bestValue, depth, lower, exact, upper, bestPosition), node);
    return bestValue;
}

int main()
{
    Board board;
    board.print();  // Mostrar el tablero inicial
    int depth = 0, bestPosition = -1;

    std::cout << "Que comience el juego 4x4x4!" << std::endl;
    Board newBoard;
    while (!newBoard.endGame()) {
        int position;
        if (newBoard.getActiveTurn() == X) {
            int maxDepth = 9;  // Ajustar según el rendimiento
            alphabeta(newBoard, maxDepth, depth, -10000000, 10000000, bestPosition);
            //alphabetaTT(newBoard, maxDepth, depth, -10000000, 10000000, bestPosition);
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