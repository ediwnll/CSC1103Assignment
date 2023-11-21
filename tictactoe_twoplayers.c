#include <gtk/gtk.h>
#include "tictactoe.h"

void handle_player_move(int row, int col)
{
    if (gameBoard[row][col] == 0)
    {
        gameBoard[row][col] = currentPlayer + 1;
        // printf("gameBoard[row][col] = %d\n", gameBoard[row][col]);
        update_game_grid();
        int result = check_winner();
        if (result == 1)
        {
            gtk_label_set_text(GTK_LABEL(gamemodelabel), "Player X wins!");
            disable_game_buttons();
        }
        else if (result == 2)
        {
            gtk_label_set_text(GTK_LABEL(gamemodelabel), "Player O wins!");
            disable_game_buttons();
        }
        else if (result == -1)
        {
            gtk_label_set_text(GTK_LABEL(gamemodelabel), "It's a Tie!");
        }
        else
        {
            // Toggle current player
            currentPlayer = 1 - currentPlayer;
            gtk_label_set_text(GTK_LABEL(gamemodelabel), (currentPlayer == 0) ? "Player X turn" : "Player O turn");
        }
    }
}
