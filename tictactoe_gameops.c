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
    // gets the number from the button state
    int position = GPOINTER_TO_INT(user_data);
    int row = position / SIZE;
    int col = position % SIZE;

    // if it is two players, we only need to let players do the move
    if (gamemode == TWOPLAYERS)
    {
        // calls the function where it will update the board base on how the player moves
        handle_player_move(row, col);
    }
    // else if game mode is medium or hard, we let player do the move and computer reply back with their algorithm function
    else if (gamemode == MEDIUM || gamemode == HARD)
    {
        handle_player_move(row, col);
        if (check_winner() == 0)
        {
            // minimax to make the computer move
            minimax_move(TEST_O);
        }
    }
    // else if game mode is easy, we let player do the move and computer will reply back with a algorith move from naive bayes
    else if (gamemode == EASY)
    {
        handle_player_move(row, col);
        if (check_winner() == 0)
        {
            // naive bayes classifier to make the computer move
            naivebayes_move(TEST_O);
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

            char button_id[20];
            snprintf(button_id, sizeof(button_id), "button_%d_%d", i, j);
            gtk_widget_set_name(button, button_id);
            GtkStyleContext *context = gtk_widget_get_style_context(button);
            gtk_style_context_remove_class(context, "button");
            gtk_style_context_remove_class(context, "x_button");
            gtk_style_context_remove_class(context, "o_button");

            if (gameBoard[i][j] == PLAYER)
            {
                gtk_button_set_label(GTK_BUTTON(button), "X");
                gtk_widget_set_sensitive(button, FALSE);
                // Set a class for X buttons
                GtkStyleContext *context = gtk_widget_get_style_context(button);
                gtk_style_context_add_class(context, "x_button");
            }
            else if (gameBoard[i][j] == COMPUTER)
            {
                gtk_button_set_label(GTK_BUTTON(button), "O");
                gtk_widget_set_sensitive(button, FALSE);
                // Set a class for O buttons
                GtkStyleContext *context = gtk_widget_get_style_context(button);
                gtk_style_context_add_class(context, "o_button");
            }
            else
            {
                gtk_button_set_label(GTK_BUTTON(button), "");
                gtk_widget_set_sensitive(button, TRUE);

                // Remove any classes for empty buttons
                GtkStyleContext *context = gtk_widget_get_style_context(button);
                gtk_style_context_add_class(context, "button");
            }
            // Debugging print statement
            // g_print("Button at row %d, col %d has ID: %s, Label: %s\n", i, j, button, gtk_button_get_label(GTK_BUTTON(button)));
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
