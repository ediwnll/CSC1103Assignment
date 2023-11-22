#include <gtk/gtk.h>
#include "tictactoe.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

// The minimax algorithm evaluates all possible moves and selects the best move
// based on the current game state and the maximizing or minimizing player.
// It uses alpha-beta pruning to optimize the search tree.

// Minimax Algorithm Function
int minimax(int depth, int isMax, int alpha, int beta, int isTesting)
{
    // Evaluate the score at the current depth
    int score = check_winner();
    if (score == -1)
    {
        return 0;
    }
    else if (score != 0)
    {
        if (score == 1)
        {
            return -1; // Player wins
        }
        if (score == 2)
        {
            return 1; // Computer wins
        }
    }

    if (isMax)
    {
        // Maximizer
        int max_val = -1000;

        // loop through every cell on board
        for (int row = 0; row < SIZE; row++)
        {
            for (int col = 0; col < SIZE; col++)
            {
                if (gameBoard[row][col] == 0)
                {
                    if (isTesting)
                    {
                        gameBoard[row][col] = PLAYER; // Testing: Player will be the maximizer
                    }
                    else
                    {
                        gameBoard[row][col] = COMPUTER; // Assume AI makes a move
                    }

                    // Recursively evaluate the move
                    max_val = fmax(max_val, minimax(depth + 1, !isMax, alpha, beta, isTesting));
                    alpha = fmax(alpha, max_val);

                    gameBoard[row][col] = BLANK; // Undo move

                    // Alpha-beta pruning to optimize the search
                    if (beta <= alpha)
                    {
                        break; // Prune the remaining branches
                    }
                }
            }
        }
        return max_val;
    }
    else
    {
        // Minimizer
        int min_val = 1000;

        // loop through every cell on board
        for (int row = 0; row < SIZE; row++)
        {
            for (int col = 0; col < SIZE; col++)
            {
                if (gameBoard[row][col] == 0)
                {
                    if (isTesting)
                    {
                        gameBoard[row][col] = COMPUTER; // Testing: Computer will be the minimizer
                    }
                    else
                    {
                        gameBoard[row][col] = PLAYER; // Assume Player makes a move
                    }

                    // Recursively evaluate the move
                    min_val = fmin(min_val, minimax(depth + 1, !isMax, alpha, beta, isTesting));
                    beta = fmin(beta, min_val);

                    gameBoard[row][col] = BLANK; // Undo move

                    // Alpha-beta pruning to optimize the search
                    if (beta <= alpha)
                    {
                        break; // Prune remaining branches
                    }
                }
            }
        }
        return min_val;
    }
}

// The minimax_move function uses the minimax algorithm to determine the best move
// for the computer player. It considers all possible moves, assigns a score to each move,
// and selects the move with the highest score.

// Minimax Move Function
void minimax_move(int isTesting)
{
    int bestMove = -1;
    int bestVal = -1000;

    // loop through all cells on the board
    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (gameBoard[row][col] == BLANK)
            {
                if (isTesting)
                {
                    gameBoard[row][col] = PLAYER; // Testing: Player will be the maximizer
                }
                else
                {
                    gameBoard[row][col] = COMPUTER; // Assume AI makes a move
                }

                // Evaluate the move using the minimax algorithm
                int moveVal = minimax(0, 0, -1000, 1000, isTesting);
                gameBoard[row][col] = BLANK; // Clear the cell

                // Select the best move based on the highest evaluation score
                if (moveVal > bestVal)
                {
                    bestVal = moveVal;
                    bestMove = row * SIZE + col;
                }
            }
        }
    }

    if (bestMove != -1)
    {
        // Convert best move value to row and column
        int bestRow = bestMove / SIZE;
        int bestCol = bestMove % SIZE;

        // If game mode is medium
        if (gamemode == MEDIUM)
        {
            // Introduce randomness to make an imperfect move
            if (rand() % 3 == 0)
            {
                int randomMove;

                // Find a random empty cell
                do
                {
                    randomMove = rand() % (SIZE * SIZE);
                } while (gameBoard[randomMove / SIZE][randomMove % SIZE] != 0);

                // Convert the random move index to row and column
                bestRow = randomMove / SIZE;
                bestCol = randomMove % SIZE;
            }
        }

        // Make the best move according to the minimax algorithm
        handle_player_move(bestRow, bestCol);
    }
}
