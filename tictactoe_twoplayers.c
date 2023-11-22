#include <gtk/gtk.h>
#include "tictactoe.h"

// Function to handle the player move on the board
void handle_player_move(int row, int col)
{
    // Check if the selected cell is empty
    if (gameBoard[row][col] == 0)
    {
        // Update the game board with the current player symbol
        gameBoard[row][col] = currentPlayer + 1;

        // Update the graphical representation of the game board
        update_game_grid();

        // Check if there is a winner after the current move
        int result = check_winner();
        if (result == 1)
        {
            // Display a text if Player X wins
            gtk_label_set_text(GTK_LABEL(gamemodelabel), "Player X wins!");
            disable_game_buttons(); // Disable the game buttons since the game is over
        }
        else if (result == 2)
        {
            // Display a text if Player O wins
            gtk_label_set_text(GTK_LABEL(gamemodelabel), "Player O wins!");
            disable_game_buttons(); // Disable the game buttons since the game is over
        }
        else if (result == -1)
        {
            // Display a text if the game is a tie
            gtk_label_set_text(GTK_LABEL(gamemodelabel), "It's a Tie!");
            disable_game_buttons(); // Disable the game buttons since the game is over
        }
        else
        {
            // Toggle to the next player turn if the game is still ongoing
            currentPlayer = 1 - currentPlayer;
            gtk_label_set_text(GTK_LABEL(gamemodelabel), (currentPlayer == 0) ? "Player X turn" : "Player O turn");
        }
    }
}
