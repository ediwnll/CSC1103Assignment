#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

// Board Definitions
#define SIZE 3
#define PLAYER 1
#define COMPUTER 2
#define BLANK 0

// Difficulty definitions
#define TWOPLAYERS 1
#define EASY 2
#define MEDIUM 3
#define HARD 4

// ML Definitions
#define winStrategy 958
#define maxCol 10
#define num_feature_sets 9
#define num_feature 9

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
extern GtkWidget *gameGrid[SIZE][SIZE]; // the buttons for tic tac toe
extern int gamemode;
extern int currentPlayer;
extern int gameBoard[SIZE][SIZE];
void load_css(const char *path); // css

// ML Variables
typedef struct
{
    double positive;
    double negative;
} PossibilityLabel;
typedef struct
{
    char position[2];
    PossibilityLabel *possibility;
} Feature;
extern Feature **possibilityAtt;
extern PossibilityLabel *possibilityLabel;

// function prototypes
void create_mainwindow();                                               // creates the main window
void create_gamewindow();                                               // creates the game window for tictactoe
void two_players_button_clicked(GtkButton *button, gpointer user_data); // main window two player button clicked
void one_player_button_clicked(GtkButton *button, gpointer user_data);  // main window singleplayer button clicked
void difficulty_button_clicked(GtkWidget *button, gpointer user_data);  // singleplayer window choose difficulty
void reset_game();                                                      // set gameBoard to empty and current player back to X
void handle_grid_button(GtkButton *button, gpointer user_data);         // allows the grid button to change label to X or O base on current player
void update_game_grid();
void back_button_clicked(GtkButton *button, gpointer user_data);
int check_winner();
void disable_game_buttons(); // disable all game buttons when game is over
void create_difficultywindow();
int minimax(int depth, int isMax, int alpha, int beta);
void handle_player_move(int row, int col);
void minimax_move();

// ML function prototypes
PossibilityLabel *allocatePossibility();
Feature **allocateFeatureSet();
void setup(char *array[]);
void shuffle(char *array[]);
void populateData(char *dataSet[], char *trainingSet[][num_feature], char *testSet[][num_feature], char *outcome[]);
void learn(char *trainingSet[][num_feature], char *outcome[], int rowSize);
PossibilityLabel predict();
PossibilityLabel* predictionValue(char* testData[][9]);
char *gameBoard_to_featureSet(int value);
void free_memory();
void naivebayes();
void naivebayes_move();
void computeConfusionMatrix(char *testSet[][num_feature], char *actualOutcomes[], int testSetSize);
double computeAccuracy(char *testSet[][num_feature], char *actualOutcomes[], int testSetSize);
/*
gui.c functions
create_mainwindow
create_gamewindow
create_difficultywindow
difficulty_button_clicked
oneplayer_button_clicked
two player button click
backbutton clicked

gameoperation functions
resetgame
handlegridbutton
updategamegrid
checkwinner
disablegamebutton

tictactoetwoplayer function
handleplayermove

tictactoe singleplayer
handlecomputermove
minimax

*/