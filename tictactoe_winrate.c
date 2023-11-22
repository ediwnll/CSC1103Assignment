#include <stdlib.h>
#include "tictactoe.h"

void play_game_minimax_winrate()
{
    int winner = 0;

    // Imperfect Minimax vs Perfect Minimax
    while (winner == 0)
    {
        gamemode = MEDIUM;
        minimax_move(TEST_X); // First player's move
        winner = check_winner();
        if (winner != 0)
            break;

        // Set the game mode to the desired mode for the second player (algorithm2)
        gamemode = HARD;
        minimax_move(TEST_O); // Second player's move
        winner = check_winner();
    }
    // Print the result of the game
}

void play_game_naive_winrate()
{
    int winner = 0;

    // Naive-Bayes Classifier vs Perfect Minimax
    while (winner == 0)
    {
        gamemode = EASY;
        naivebayes_move(); // First player's move
        winner = check_winner();
        if (winner != 0)
            break;
        gamemode = HARD;
        minimax_move(TEST_O);
        winner = check_winner();
    }
}

void play_multiple_games(int num_games)
{
    // For easy minimax vs hard minimax
    int contender = 0;
    int perfectminimax_win = 0;
    int draws = 0;
    float winrate = 0.0;

    for (int i = 0; i < num_games; ++i)
    {
        play_game_minimax_winrate();

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
        reset_game();
    }
    // Print win rate
    printf("\nPerfect Minimax wins: %d\n", perfectminimax_win);
    printf("Imperfect Minimax wins: %d\n", contender);
    printf("Draws: %d\n", draws);
    winrate = (((float)draws + (float)contender) / num_games) * 100;
    printf("Due to the fact that Perfect Minimax cannot lose, we consider draws to be the winrate\n");
    printf("Winrate: %.2f\n", winrate);

    contender = 0;
    perfectminimax_win = 0;
    draws = 0;
    winrate = 0;

    for (int i = 0; i < num_games; ++i)
    {
        play_game_naive_winrate();

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
        reset_game();
    }
    printf("\nPerfect Minimax wins: %d\n", perfectminimax_win);
    printf("Naive Bayes wins: %d\n", contender);
    printf("Draws: %d\n", draws);
    winrate = (((float)draws + (float)contender) / num_games) * 100;
    printf("Due to the fact that Perfect Minimax cannot lose, we consider draws to be the winrate\n");
    printf("Winrate: %.2f\n", winrate);
}