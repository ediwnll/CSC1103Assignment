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

    gtk_init(&argc, &argv); // Initialise GTK for the GUI
    // g_print("Current working directory: %s\n", g_get_current_dir());
    load_css("style.css"); // Load the style sheet (design for the GUI)

    naivebayes();                 // Initialise learning of Naive-Bayes model
    create_mainwindow();          // Create Main Window
    create_gamewindow();          // Create Game Window
    create_difficultywindow();    // Create Difficulty Window
    play_multiple_games(NUMGAME); // Test the win rate of the different difficulties by a set number of games
    reset_game();                 // Always run to reset the game
    gtk_main();                   // Start the GTK main loop

    return 0;
}
