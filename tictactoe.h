#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

/* DEFINITIONS */
// Board Definitions
#define SIZE 3
#define PLAYER 1
#define COMPUTER 2
#define BLANK 0
// Difficulty
#define TWOPLAYERS 1
#define EASY 2
#define MEDIUM 3
#define HARD 4
// Machine Learning
#define winStrategy 958
#define maxCol 9
#define label_size 9
#define num_feature_sets 9
#define num_feature 9
// Win Rate Testing
#define NUMGAME 20
#define TEST_X 1
#define TEST_O 0

/*GLOBAL VARIABLES */
// GUI
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
// Game Board
extern int gamemode;
extern int currentPlayer;
extern int gameBoard[SIZE][SIZE];
// Naive-Bayes Classifier
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
typedef struct
{
    char features[num_feature][maxCol - 1]; // Features (excluding label)
    char label[label_size];                 // Label
} DataInstance;
extern Feature **possibilityAtt;
extern PossibilityLabel *possibilityLabel;
extern DataInstance *dataSet;
extern DataInstance *trainingSet;
extern DataInstance *testSet;

/* FUNCTION PROTOTYPES */
// GUI function prototypes
void create_mainwindow();                                               // creates the main window
void create_gamewindow();                                               // creates the game window for tictactoe
void two_players_button_clicked(GtkButton *button, gpointer user_data); // main window two player button clicked
void one_player_button_clicked(GtkButton *button, gpointer user_data);  // main window singleplayer button clicked
void difficulty_button_clicked(GtkWidget *button, gpointer user_data);  // singleplayer window choose difficulty
void reset_game();                                                      // set gameBoard to empty and current player back to X
void handle_grid_button(GtkButton *button, gpointer user_data);         // allows the grid button to change label to X or O base on current player
void update_game_grid();
void back_button_clicked(GtkButton *button, gpointer user_data);
void disable_game_buttons(); // disable all game buttons when game is over
void create_difficultywindow();
void load_css(const char *path); // css
// Game Operations
int check_winner();
void handle_player_move(int row, int col);
// Minimax Algorithm
int minimax(int depth, int isMax, int alpha, int beta, int isTesting);
void minimax_move(int isTesting);
// Win Rate Testing
void play_game_minimax_winrate();
void play_game_naive_winrate();
void play_multiple_games(int num_games);
// Naive-Bayes Classifier
PossibilityLabel *allocatePossibility();
Feature **allocateFeatureSet();
void setup(DataInstance *array);
void shuffle(DataInstance *array);
void populateData(DataInstance *dataSet, DataInstance *trainingSet, DataInstance *testSet, int dataSize, int trainingSize);
void learn(DataInstance *trainingSet, int rowSize);
PossibilityLabel predict(DataInstance *testSet);
void fill_features_from_board(DataInstance *dataInstance);
void free_memory();
void naivebayes();
void naivebayes_move(int isTesting);
void computeConfusionMatrix(DataInstance *testSet, int testSetSize);
double computeAccuracy(DataInstance *testSet, int testSetSize);