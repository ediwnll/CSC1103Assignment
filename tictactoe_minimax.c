#include <gtk/gtk.h>
#include "tictactoe.h"
#include <stdio.h>
#include <math.h>

void handle_computer_move()
{
    int bestMove = -1;
    int bestVal = -1000;
    // loop through all cells
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (gameBoard[row][col] == 0)
            {
                gameBoard[row][col] = 2; // Computer move
                int moveVal = minimax(0, 0, -1000, 1000);
                gameBoard[row][col] = 0; // remove
                if (moveVal > bestVal)
                {
                    bestVal = moveVal;
                    bestMove = row * 3 + col;
                }
            }
        }
    }
    if (bestMove != -1)
    {
        int computer_row = bestMove / 3;
        int computer_col = bestMove % 3;
        printf("computer_row = %d, computer_col = %d\n", computer_row, computer_col);
        handle_player_move(computer_row, computer_col);
    }
}

int minimax(int depth, int isMax, int alpha, int beta)
{
    int score = check_winner();
    if (score == -1)
    {
        return 0;
    }
    else if (score != 0)
    {
        if (score == 1)
        {
            return -1;
        }
        if (score == 2)
        {
            return 1;
        }
    }
    if (isMax)
    {
        int max_val = -1000;
        // run through every cell on board
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                if (gameBoard[row][col] == 0)
                {
                    gameBoard[row][col] = 2; // Assume AI made a move
                    max_val = fmax(max_val, minimax(depth + 1, !isMax, alpha, beta));
                    alpha = fmax(alpha, max_val);
                    gameBoard[row][col] = 0; // Undo move

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
        int min_val = 1000;
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                if (gameBoard[row][col] == 0)
                {
                    gameBoard[row][col] = 1; // Assume player made a move
                    min_val = fmin(min_val, minimax(depth + 1, !isMax, alpha, beta));
                    beta = fmin(beta, min_val);
                    gameBoard[row][col] = 0; // Undo move
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
