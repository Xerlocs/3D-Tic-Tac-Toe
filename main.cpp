#include <iostream>
#include <chrono>  // Para medir el tiempo

#include "board.hpp"
#include "TTEntry.h"

TranspositionTable TT;

int nodesEvaluated = 0; // Contador para nodos revisados

int negamax(Board node, int depth, int &bestPosition) {
    nodesEvaluated++;  // Incrementar contador de nodos
    int maxDepth = 9;
    if(node.endGame() || depth == maxDepth)
        return node.evaluate(depth);

    int bestValue = -10000, dummy;  
    for (int position : node.generateAllLegalMoves()) {
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -negamax(child, depth + 1, dummy);
        if (value > bestValue) {
            bestValue = value;
            bestPosition = position;
        }
    }
    return bestValue;
}

int alphabeta(Board node, int maxDepth, int depth, int alpha, int beta, int &bestPosition) {
    nodesEvaluated++;  // Incrementar contador de nodos
    if(node.endGame() || depth == maxDepth)
        return node.evaluate(depth);

    int bestValue = -10000, dummy;  
    for (int position : node.generateAllLegalMoves()) {
        //std::cout << "z";
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -alphabeta(child, maxDepth, depth + 1, -beta, -alpha, dummy);
        if (value > bestValue) {
            bestValue = value;
            bestPosition = position;
            if (value > alpha) alpha = value;
            if (alpha >= beta) break;
        }
    }
    return bestValue;
}

int iterativeDeepening(Board node, int maxDepth, int &bestPosition) {
    int value;
    for(int depth = 0; depth < maxDepth; depth++) {
        nodesEvaluated = 0;  // Resetear contador de nodos por cada profundidad
        auto start = std::chrono::high_resolution_clock::now(); // Comenzar tiempo

        value = alphabeta(node, depth, 0, -10000000, 10000000, bestPosition);
        
        auto end = std::chrono::high_resolution_clock::now(); // Finalizar tiempo
        std::chrono::duration<double> elapsed = end - start; // Calcular tiempo transcurrido
        
        // Imprimir estadísticas
        std::cout << "Iteración a profundidad " << depth << ":\n";
        std::cout << "Tiempo de ejecución: " << elapsed.count() << " segundos\n";
        std::cout << "Nodos revisados: " << nodesEvaluated << "\n";
        std::cout << "Nodos por segundo: " << (nodesEvaluated / elapsed.count()) << "\n\n";
    }
    return value;
}

/*int alphabetaTT(Board node, int maxDepth, int depth, int alpha, int beta, int &bestPosition) {
    std::cout << "Entering alphabetaTT at depth " << depth << std::endl;
    nodesEvaluated++;  // Incrementar contador de nodos
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
        std::cout << "Game ended at depth " << depth << std::endl;
        return node.evaluate(depth);

    int bestValue = -10000, dummy;  
    int al = alpha;
    for (int position : node.generateAllLegalMoves()) {
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -alphabetaTT(child, maxDepth, depth + 1, -beta, -al, dummy);
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
}*/

int alphabetaTT(Board node, int maxDepth, int depth, int alpha, int beta, int &bestPosition) {
    std::cout << "Entering alphabetaTT at depth " << depth << std::endl;

    if (node.endGame()) {
        std::cout << "Game ended at depth " << depth << std::endl;
        return node.evaluate(depth);
    }

    int bestValue = -10000;
    std::vector<int> legalMoves = node.generateAllLegalMoves();
    if (legalMoves.empty()) {
        std::cout << "No legal moves at depth " << depth << std::endl;
    }

    for (int position : legalMoves) {
        std::cout << "Trying move at position " << position << std::endl;
        Board child(node.getXBoard(), node.getOBoard(), node.getActiveTurn());
        child.makeMove(position);

        int value = -alphabetaTT(child, maxDepth, depth + 1, -beta, -alpha, bestPosition);
        std::cout << "Evaluated move at position " << position << " with value " << value << std::endl;

        if (value > bestValue) {
            bestValue = value;
            bestPosition = position;
            std::cout << "New best move at position " << position << " with value " << value << std::endl;
            alpha = std::max(alpha, value);
            if (alpha >= beta) {
                std::cout << "Alpha-beta cutoff at position " << position << std::endl;
                break;
            }
        }
    }

    if (bestPosition == -1) {
        std::cout << "Warning: bestPosition was not updated, fallback to first legal move" << std::endl;
        if (!legalMoves.empty()) {
            bestPosition = legalMoves[0];  // Fallback to first legal move
        }
    }

    return bestValue;
}


int main() {
    Board board;
    board.print();  // Mostrar el tablero inicial
    int depth = 0, bestPosition = -1;

    std::cout << "¡Que comience el juego 4x4x4!" << std::endl;
    Board newBoard;
    while (!newBoard.endGame()) {
        int position;
        if (newBoard.getActiveTurn() == X) {
            int maxDepth = 9;  // Ajustar según el rendimiento
            // Descomentar la función que deseas medir
            //negamax(newBoard, depth, bestPosition);
            //alphabeta(newBoard, maxDepth, depth, -10000000, 10000000, bestPosition);
            iterativeDeepening(newBoard, maxDepth, bestPosition);
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
