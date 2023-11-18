#include <gtk/gtk.h>
#include "tictactoe.h"
#include <stdio.h>
#include <math.h>

void reset_game()
{
    currentPlayer = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gameBoard[i][j] = 0;
        }
    }
    update_game_grid();
    gtk_label_set_text(GTK_LABEL(gamemodelabel), "Player X turn");
}

void handle_grid_button(GtkButton *button, gpointer user_data)
{
    int position = GPOINTER_TO_INT(user_data);
    // printf("position: %d\n", position);
    int row = position / SIZE;
    int col = position % SIZE;
    // printf("row: %d, col: %d\n", row, col);

    if (gamemode == TWOPLAYERS)
    {
        handle_player_move(row, col);
    }
    else if (gamemode == EASY || gamemode == HARD)
    {
        handle_player_move(row, col);
        if (check_winner() == 0)
        {
            minimax_move();
        }
    }
}

void update_game_grid()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            GtkWidget *button = gameGrid[i][j];
            if (gameBoard[i][j] == 1)
            {
                gtk_button_set_label(GTK_BUTTON(button), "X");
                gtk_widget_set_sensitive(button, FALSE);
            }
            else if (gameBoard[i][j] == 2)
            {
                gtk_button_set_label(GTK_BUTTON(button), "O");
                gtk_widget_set_sensitive(button, FALSE);
            }
            else
            {
                gtk_button_set_label(GTK_BUTTON(button), "");
                gtk_widget_set_sensitive(button, TRUE);
            }
        }
    }
}

int check_winner()
{
    for (int i = 0; i < SIZE; i++)
    {
        // check row
        if (gameBoard[i][0] != 0 && gameBoard[i][0] == gameBoard[i][1] && gameBoard[i][1] == gameBoard[i][2])
        {
            return gameBoard[i][0];
        }
        // check columns
        if (gameBoard[0][i] != 0 && gameBoard[0][i] == gameBoard[1][i] && gameBoard[1][i] == gameBoard[2][i])
        {
            return gameBoard[0][i];
        }
    }
    // check for diagonal wins
    if (gameBoard[0][0] != 0 && gameBoard[0][0] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][2])
    {
        return gameBoard[0][0];
    }
    if (gameBoard[0][2] != 0 && gameBoard[0][2] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][0])
    {
        return gameBoard[0][2];
    }
    // Check for a draw
    int is_draw = 1;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (gameBoard[i][j] == 0)
            {
                is_draw = 0; // There are empty cells, not a draw, break out of the loop and continue to return.
                break;
            }
        }
    }
    if (is_draw)
    {
        return -1; // Draw
    }

    return 0; // no winner yet
}

void disable_game_buttons()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            gtk_widget_set_sensitive(gameGrid[i][j], FALSE);
        }
    }
}
