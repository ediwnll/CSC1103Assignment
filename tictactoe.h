#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

// Global Variables
extern GtkWidget *mainwindow;
extern GtkWidget *gamewindow;
extern GtkWidget *difficultywindow;
extern GtkWidget *singlePlayerButton;
extern GtkWidget *twoPlayersButton;
extern GtkWidget *easybutton;
extern GtkWidget *mediumbutton;
extern GtkWidget *hardbutton;
extern GtkWidget *gamelabel;
extern GtkWidget *gamemodelabel;
extern GtkWidget *difficultylabel;
extern GtkWidget *gameGrid[3][3]; // the buttons for tic tac toe
extern int gamemode;              // twoplayer = 1, singleplayer easy = 2, medium = 3,  hard = 4
extern int currentPlayer;
extern int gameBoard[3][3];

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
int minimax(int depth, int isMax, int alpha, int beta);
void handle_computer_move();
void handle_player_move(int row, int col);