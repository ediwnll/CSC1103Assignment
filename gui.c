#include <gtk/gtk.h>
#include "tictactoe.h"

// game window
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

    // Connect the "destroy" signal to quit the application
    g_signal_connect(mainwindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all the widgets
    gtk_widget_show_all(mainwindow);
}

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

// When two players button is clicked
void two_players_button_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(mainwindow);
    gtk_widget_show(gamewindow);
    gtk_label_set_text(GTK_LABEL(gamemodelabel), "Two Players");
    int gamemode = 1;
}

// Back Button
void back_button_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(gamewindow);
    gtk_widget_show(mainwindow);
    reset_game();
    gtk_label_set_text(GTK_LABEL(gamemodelabel), "Game Label");
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

    // create 3x3 grid for tic tac toe
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

    GtkWidget *gameVBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(gamewindow), gameVBox);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // add back button to gamewindow
    // Add the "Back" button to the top-left corner of the screen
    GtkWidget *topLeftBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(topLeftBox), back_button, FALSE, FALSE, 0);

    // Add the top-left box to the game window
    gtk_box_pack_start(GTK_BOX(gameVBox), topLeftBox, FALSE, FALSE, 0);
    // add label to gamewindow
    gtk_box_pack_start(GTK_BOX(gameVBox), gamemodelabel, FALSE, FALSE, 0);

    // add grid to gameVbox
    gtk_box_pack_start(GTK_BOX(gameVBox), grid, TRUE, TRUE, 0);

    // Init game variables
    currentPlayer = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gameBoard[i][j] = 0;
        }
    }
    gtk_widget_show_all(gamewindow);
    gtk_widget_hide(gamewindow);
}

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
