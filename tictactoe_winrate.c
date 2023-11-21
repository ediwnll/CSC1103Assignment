#include <stdlib.h>
#include "tictactoe.h"

void play_game_minimax_winrate(int algorithm1(), int algorithm2())
{
    reset_game();
    int winner = 0;

    // Imperfect Minimax vs Perfect Minimax
    while (winner == 0)
    {
        gamemode = EASY;
        algorithm1(); // First player's move
        winner = check_winner();
        if (winner != 0)
            break;

        // Set the game mode to the desired mode for the second player (algorithm2)
        gamemode = HARD;
        algorithm2(); // Second player's move
        winner = check_winner();
    }
    // Print the result of the game
}

void play_game_naive_winrate(int algorithm1(), int algorithm2())
{
    reset_game();
    int winner = 0;
    while (winner == 0)
    {
        gamemode = MEDIUM;
        algorithm1(); // First player's move
        winner = check_winner();
        if (winner != 0)
            break;
        gamemode = HARD;
        algorithm2();
        winner = check_winner();
    }
}

void play_multiple_games(int num_games)
{
    // For easy minimax vs hard minimax
    int contender = 0;
    int perfectminimax_win = 0;
    int draws = 0;

    for (int i = 0; i < num_games; ++i)
    {
        play_game(minimax_move, minimax_move);

        // Update win counts based on the result
        int winner = check_winner();
        if (winner == 1)
        {
            contender++;
        }
        else if (winner == 2)
        {
            perfectminimax_win++;
        }
        else
        {
            draws++;
        }
    }
    // Print win rate
    printf("Perfect Minimax wins: %d\n", perfectminimax_win);
    printf("Imperfect Minimax wins: %d\n", contender);
    printf("Draws: %d\n", draws);
    int winrate = draws / num_games;
    printf("Due to the fact that Perfect Minimax cannot lose, we consider draws to be the winrate\n");
    printf("Winrate: %d\n", winrate);

    contender = 0;
    perfectminimax_win = 0;
    draws = 0;
    winrate = 0;

    for (int i = 0; i < num_games; ++i)
    {
        play_game(naivebayes_move, minimax_move);

        // Update win counts based on the result
        int winner = check_winner();
        if (winner == 1)
        {
            contender++;
        }
        else if (winner == 2)
        {
            perfectminimax_win++;
        }
        else
        {
            draws++;
        }
    }
    printf("Perfect Minimax wins: %d\n", perfectminimax_win);
    printf("Naive Bayers wins: %d\n", contender);
    printf("Draws: %d\n", draws);
    int winrate = draws / num_games;
    printf("Due to the fact that Perfect Minimax cannot lose, we consider draws to be the winrate\n");
    printf("Winrate: %d\n", winrate);
}