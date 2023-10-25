#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

// Global Variables
GtkWidget *mainwindow;
GtkWidget *gamewindow;
GtkWidget *difficultywindow;
GtkWidget *singlePlayerButton;
GtkWidget *twoPlayersButton;
GtkWidget *easybutton;
GtkWidget *mediumbutton;
GtkWidget *hardbutton;
GtkWidget *gamelabel;
GtkWidget *gamemodelabel;
GtkWidget *difficultylabel;
GtkWidget *gameGrid[3][3]; // the buttons for tic tac toe
int gamemode = 0;          // twoplayer = 1, singleplayer easy = 2, medium = 3,  hard = 4
int currentPlayer;
int gameBoard[3][3];

// function prototypes
void create_mainwindow();                                               // creates the main window
void create_gamewindow();                                               // creates the game window for tictactoe
void two_players_button_clicked(GtkButton *button, gpointer user_data); // main window two player button clicked
void one_player_button_clicked(GtkButton *button, gpointer user_data);  // main window singleplayer button clicked
void difficulty_button_clicked(GtkWidget *button, gpointer user_data);  // singelplayer window choose difficulty
void reset_game();                                                      // set gameBoard to empty and current player back to X
void handle_grid_button(GtkButton *button, gpointer user_data);         // allows the grid button to change label to X or O base on current player
void update_game_grid();
void back_button_clicked(GtkButton *button, gpointer user_data);
int check_winner();
void disable_game_buttons(); // disable all game buttons when game is over
void create_difficultywindow();

void create_difficultywindow()
{
    difficultywindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(difficultywindow), "Select Difficulty");
    gtk_window_set_default_size(GTK_WINDOW(difficultywindow), 500, 500);
    gtk_container_set_border_width(GTK_CONTAINER(difficultywindow), 30);

    // make window non resizeable
    gtk_window_set_resizable(GTK_WINDOW(difficultywindow), FALSE);

    // Create back window
    GtkWidget *difficulty_back_button = gtk_button_new_with_label("Back");
    g_signal_connect(difficulty_back_button, "clicked", G_CALLBACK(back_button_clicked), NULL);
    gtk_widget_set_size_request(difficulty_back_button, 60, 30);

    // Create label for difficulty
    difficultylabel = gtk_label_new("Choose your difficulty");
    gtk_widget_set_name(difficultylabel, "difficultylabel");
    // Add padding to top and bottom of label
    gtk_widget_set_margin_top(difficultylabel, 10);
    gtk_widget_set_margin_bottom(difficultylabel, 20);

    // vertical box for difficultywindow
    GtkWidget *gameVBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(difficultywindow), gameVBox);

    // Add the "Back" button to the top-left corner of the screen
    GtkWidget *topLeftBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(topLeftBox), difficulty_back_button, FALSE, FALSE, 0);
    // Add back button into gameVBox
    gtk_box_pack_start(GTK_BOX(gameVBox), topLeftBox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(gameVBox), difficultylabel, TRUE, TRUE, 0);

    // Create easy button
    easybutton = gtk_button_new_with_label("EASY");
    gtk_widget_set_size_request(easybutton, 100, 60); // Set button size
    g_signal_connect(easybutton, "clicked", G_CALLBACK(difficulty_button_clicked), NULL);
    // Create medium button
    mediumbutton = gtk_button_new_with_label("MEDIUM");
    gtk_widget_set_size_request(mediumbutton, 100, 60); // Set button size
    g_signal_connect(mediumbutton, "clicked", G_CALLBACK(difficulty_button_clicked), NULL);
    // Create hard button
    hardbutton = gtk_button_new_with_label("IMPOSSIBLE!");
    gtk_widget_set_size_request(hardbutton, 100, 60); // Set button size
    g_signal_connect(hardbutton, "clicked", G_CALLBACK(difficulty_button_clicked), NULL);

    GtkWidget *gameHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(gameVBox), gameHBox, FALSE, FALSE, 0);

    gtk_box_pack_end(GTK_BOX(gameHBox), hardbutton, FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(gameHBox), mediumbutton, FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(gameHBox), easybutton, FALSE, FALSE, 20);

    gtk_widget_set_margin_bottom(gameHBox, 60);

    gtk_widget_show_all(difficultywindow);
    gtk_widget_hide(difficultywindow);
}

void create_gamewindow()
{
    gamewindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(gamewindow), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(gamewindow), 500, 500);
    gtk_container_set_border_width(GTK_CONTAINER(gamewindow), 30);

    // make window non resizeable
    gtk_window_set_resizable(GTK_WINDOW(gamewindow), FALSE);

    // label for gamemode
    gamemodelabel = gtk_label_new("Game Label");
    gtk_widget_set_name(gamemodelabel, "gamemodelabel");
    // Add padding to the top and bottom of gamemodelabel
    gtk_widget_set_margin_top(gamemodelabel, 10);
    gtk_widget_set_margin_bottom(gamemodelabel, 20);

    // Create a "Back" button
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    g_signal_connect(back_button, "clicked", G_CALLBACK(back_button_clicked), NULL);
    gtk_widget_set_size_request(back_button, 60, 30);

    GtkWidget *grid = gtk_grid_new();

    GtkWidget *gameVBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(gamewindow), gameVBox);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // add back button to gamewindow
    // add back button to the top-left corner of the screen
    GtkWidget *topLeftBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(topLeftBox), back_button, FALSE, FALSE, 0);
    // add the top-left box to the game window
    gtk_box_pack_start(GTK_BOX(gameVBox), topLeftBox, FALSE, FALSE, 0);
    // add label to gamewindow
    gtk_box_pack_start(GTK_BOX(gameVBox), gamemodelabel, FALSE, FALSE, 0);
    // add grid to gameVbox
    gtk_box_pack_start(GTK_BOX(gameVBox), grid, TRUE, TRUE, 0);

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            gameGrid[row][col] = gtk_button_new();
            gtk_widget_set_size_request(gameGrid[row][col], 60, 60); // set button size
            gtk_grid_attach(GTK_GRID(grid), gameGrid[row][col], col, row, 1, 1);
            g_signal_connect(gameGrid[row][col], "clicked", G_CALLBACK(handle_grid_button), GINT_TO_POINTER(row * 3 + col));
        }
    }

    gtk_widget_show_all(gamewindow);
    gtk_widget_hide(gamewindow);
}

// Back Button
void back_button_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(gamewindow);
    gtk_widget_show(mainwindow);
    gamemode = 0;
    reset_game();
    gtk_label_set_text(GTK_LABEL(gamemodelabel), "Game Label");
}

// Single Player GameMode
// Minimax Algorithm
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

// Two Player GameMode
// reset the game
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

// Check for winner
int check_winner()
{
    for (int i = 0; i < 3; i++)
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
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
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

// Update the game grid
void update_game_grid()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
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

// Handle player moves
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

// Computer Move
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

// Handle grid
void handle_grid_button(GtkButton *button, gpointer user_data)
{
    int position = GPOINTER_TO_INT(user_data);
    // printf("position: %d\n", position);
    int row = position / 3;
    int col = position % 3;
    // printf("row: %d, col: %d\n", row, col);

    if (gamemode == 1)
    {
        handle_player_move(row, col);
    }
    else if (gamemode == 4)
    {
        handle_player_move(row, col);
        if (check_winner() == 0)
        {
            handle_computer_move();
        }
    }
}

// Disable game buttons
void disable_game_buttons()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gtk_widget_set_sensitive(gameGrid[i][j], FALSE);
        }
    }
}

// Create Main Window
void create_mainwindow()
{
    // Create the main window
    mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mainwindow), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(mainwindow), 500, 500);
    gtk_container_set_border_width(GTK_CONTAINER(mainwindow), 10);

    // make window non resizeable
    gtk_window_set_resizable(GTK_WINDOW(mainwindow), FALSE);

    // Create a label
    gamelabel = gtk_label_new("Tic Tac Toe");
    gtk_widget_set_name(gamelabel, "gamelabel_id");

    // Create Single Player button
    singlePlayerButton = gtk_button_new_with_label("Single Player");
    gtk_widget_set_size_request(singlePlayerButton, 200, 100); // Set button size

    // Create Two Players button
    twoPlayersButton = gtk_button_new_with_label("Two Players");
    gtk_widget_set_size_request(twoPlayersButton, 200, 100); // Set button size

    // Create a vertical box to arrange the widgets
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(mainwindow), vbox);

    // Create a horizontal box to arrange the buttons
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Add the label and buttons to the vbox
    gtk_box_pack_start(GTK_BOX(vbox), gamelabel, TRUE, TRUE, 0);

    // Add horizontal box to the vertical box
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 80);

    // Add buttons to horizontal box
    gtk_box_pack_end(GTK_BOX(hbox), singlePlayerButton, FALSE, FALSE, 30);
    gtk_box_pack_end(GTK_BOX(hbox), twoPlayersButton, FALSE, FALSE, 30);

    // gsignal connection of the two buttons
    g_signal_connect(twoPlayersButton, "clicked", G_CALLBACK(two_players_button_clicked), NULL);
    g_signal_connect(singlePlayerButton, "clicked", G_CALLBACK(one_player_button_clicked), NULL);

    // Connect the "destroy" signal to quit the application
    g_signal_connect(mainwindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all the widgets
    gtk_widget_show_all(mainwindow);
}

// When two players button is clicked
void two_players_button_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(mainwindow);
    gtk_widget_show(gamewindow);
    gtk_label_set_text(GTK_LABEL(gamemodelabel), "Two Players");
    gamemode = 1;
}
// When one player button is clicked
void one_player_button_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(mainwindow);
    gtk_widget_show(difficultywindow);
}

// Difficulty chosen button
void difficulty_button_clicked(GtkWidget *button, gpointer user_data)
{
    gtk_widget_hide(difficultywindow);
    gtk_widget_show(gamewindow);
    // Set the global gamemode variable based on the button clicked
    if (button == easybutton)
    {
        gamemode = 2;
        // printf("gamemode = %d\n", gamemode);
    }
    else if (button == mediumbutton)
    {
        gamemode = 3;
    }
    else if (button == hardbutton)
    {
        gamemode = 4;
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    create_mainwindow();
    create_gamewindow();
    create_difficultywindow();
    reset_game(); // Always run to reset the game
    // Start the GTK main loop
    gtk_main();

    return 0;
}
