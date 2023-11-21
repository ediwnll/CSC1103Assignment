#include <gtk/gtk.h>
#include "tictactoe.h"

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
GtkWidget *gameGrid[SIZE][SIZE]; // the buttons for tic tac toe
int gamemode = 0;                // twoplayer = 1, singleplayer easy = 2, medium = 3,  hard = 4
int currentPlayer;
int gameBoard[SIZE][SIZE];

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    g_print("Current working directory: %s\n", g_get_current_dir());
    load_css("style.css");

    // Initialise Naive-Bayes model
    naivebayes();

    create_mainwindow();
    create_gamewindow();
    create_difficultywindow();
    reset_game(); // Always run to reset the game
    // Start the GTK main loop
    gtk_main();

    return 0;
}
