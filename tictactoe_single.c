// CSC1103 Tic Tac Toe Single Player
#include <stdio.h>
#include <math.h>
// #include <gtk/gtk.h>

// Global Variables
char board[3][3];    // TicTacToe board
char player = 'X';   // player sign
char computer = 'O'; // computer sign

// function prototypes
void instruction();
void reset_board();
void print_board();
int isMovesLeft();
int check_winner();
int minimax(int depth, int isMax, int alpha, int beta);
void computerMove();
int playerMove();
void play_game();
int restart_game();

// instruction for the game
void instruction()
{
    // print out instructions and clear the board
    printf("\nTic Tac Toe Player vs Player\n\n");
    printf("\n\tInstructions\n");
    printf("\tPlayer sign =  X\n");
    printf("\tComputer sign =  O\n\n");
    printf("\tTo quit the game, Enter -1\n");
    printf("\n\nEnter the cell numbers on the board to place your marks\n");

    reset_board();
}

// reset board till it's empty and only shows the cell number
void reset_board()
{
    // initialise count as the cell number
    int count = 1;
    // Ensure that the cell is all empty
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }
    // create the board using print and by using for loops, we add in the count 1 by 1 into each array slots
    printf("\n\n\t  ");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d", count++);
            if (j < 2)
            {
                printf("  |  ");
            }
        }
        if (i < 2)
            printf("\n\t----------------\n\t  ");
    }
    printf("\n\n\n");
}

// print out the board with the player signs
void print_board()
{
    printf("\n\n\t  ");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // board[i][j] will be populated with player sign respectively
            printf("%c", board[i][j]);
            if (j < 2)
            {
                printf("  |  ");
            }
        }
        if (i < 2)
            printf("\n\t----------------\n\t  ");
    }
    printf("\n\n\n");
}

// check if there are still possible moves on the board
int isMovesLeft()
{
    // Run through every cell in the board
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // if board is still empty, return true to tell program to continue
            if (board[i][j] == ' ')
            {
                return 1;
            }
        }
    }
    // return false to tell program to stop
    return 0;
}

// to check if there are any winning row/columns/diagonals
int check_winner()
{
    // check for a winner in rows
    for (int row = 0; row < 3; row++)
    {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2])
        {
            // player wins
            if (board[row][0] == player)
                return -10;
            // computer wins
            else if (board[row][0] == computer)
                return 10;
        }
    }
    // check for a winner in columns
    for (int col = 0; col < 3; col++)
    {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col])
        {
            // player wins
            if (board[0][col] == player)
                return -10;
            // computer wins
            else if (board[0][col] == computer)
                return 10;
        }
    }
    // check for a winner in diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        // player wins
        if (board[0][0] == player)
            return -10;
        // computer wins
        else if (board[0][0] == computer)
            return 10;
    }
    else if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        // player wins
        if (board[0][2] == player)
            return -10;
        // computer wins
        else if (board[0][2] == computer)
            return 10;
    }

    // no winner
    return 0;
}

// Minimax algorithm with alpha-beta pruning
// Alpha is the best value that the maximizer can guarantee at that depth or above
// Beta is the best value that the minimizer can guarantee at that depth or below
int minimax(int depth, int isMax, int alpha, int beta)
{
    // Check current situation on board
    int score = check_winner();

    // Computer wins
    if (score == 10)
    {
        return score - depth;
    }
    // Player wins
    else if (score == -10)
    {
        return score + depth;
    }
    // If there are no more moves and there is no winner, it is a tie
    else if (!isMovesLeft())
    {
        return 0;
    }

    // Maximizer
    if (isMax)
    {
        int max_val = -1000;

        // run through every cell on the board
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                // check if cell is empty
                if (board[i][j] == ' ')
                {
                    // make the move
                    board[i][j] = computer;

                    // call minimax recursively and choose the maximum value
                    max_val = fmax(max_val, minimax(depth + 1, !isMax, alpha, beta));
                    alpha = fmax(alpha, max_val); // replace alpha with the best possible value

                    // undo the move
                    board[i][j] = ' ';

                    // alpha-beta pruning
                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }
        return max_val;
    }

    // Minimizer
    else
    {
        int min_val = 1000;

        // run through every cell on the board
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                // check if cell is empty
                if (board[i][j] == ' ')
                {
                    // make the move
                    board[i][j] = player;

                    // call minimax recursively and choose the minimum value
                    min_val = fmin(min_val, minimax(depth + 1, !isMax, alpha, beta));
                    beta = fmin(beta, min_val); // replace beta with the best possible value

                    // undo the move
                    board[i][j] = ' ';

                    // alpha-beta pruning
                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }
        return min_val;
    }
}

// make best possible move for Computer
void computerMove()
{
    int bestVal = -1000;
    int bestRow = -1;
    int bestCol = -1;

    // run through every cell on the board
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // check if cell is empty
            if (board[i][j] == ' ')
            {
                // make the move for computer
                board[i][j] = computer;

                // compute evaluation function for this move
                int moveVal = minimax(0, 0, -1000, 1000);

                // undo this move
                board[i][j] = ' ';

                // if the value of the current move is better than best value,
                // then update the best value
                if (moveVal > bestVal)
                {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    // make best move for computer
    board[bestRow][bestCol] = computer;
}

// prompt player to input move and return cell value

int playerMove()
{
    int move = 0;

    printf("\nInput move ([1..9]): ");
    scanf("%d", &move);
    fflush(stdin);
    printf("\n");

    return move;
}

// function to start the game
void play_game()
{
    int cell = 0;
    int user = 0;
    int turn = 0;

    // Start tic tac toe game
    do
    {
        // Allow player to choose to start first or second
        printf("To play first choose (1), to play second choose (2)");
        scanf("%d", &user);
        printf("\n");

        // if user input is not 1 or 2
        if (user != 1 && user != 2)
        {
            printf("\nPlease input a valid choice\n");
        }
        else
        {
            // program executes 9 times or while a winner has not been found
            while (turn < 9 && check_winner() == 0)
            {
                // computer's turn
                if ((user + turn) % 2 == 0)
                {
                    // print the current situation on the board
                    print_board();
                    printf("\nPlease wait, I am making my move...");
                    // make the best move for computer
                    computerMove();
                    // print board to display current status
                    turn++;
                }
                // player's turn
                else
                {
                    // print the current situation on the board
                    print_board();
                    // prompt player to input move
                    do
                    {
                        cell = playerMove();
                        // if user inputs 1-9
                        if ((cell >= 1 && cell <= 9))
                        {
                            int row = (cell - 1) / 3;
                            int column = (cell - 1) % 3;

                            // check that the cell is filled
                            if (board[row][column] != ' ')
                            {
                                printf("\nCell is already filled. Choose another cell.\n");
                            }
                            // if cell is not filled, fill it with player sign
                            else
                            {
                                board[row][column] = player;
                                turn++;
                            }
                        }
                        // if user inputs -1
                        else if (cell == -1)
                        {
                            // exit the game
                            return;
                        }
                        // if user input is not 1-9 or -1
                        else
                        {
                            // prompt user to input choice again
                            printf("\nPlease enter a valid choice (1-9)\nAlternatively, enter (-1) to exit");
                        }

                    } while ((cell > 9 || cell < 1) && cell != -1); // continue prompting for user input if the choice is not 1-9 or -1
                }
            }
        }

    } while (user != 1 && user != 2); // if user choice is not 1 or 2, prompt again
}

// restart game function or quit game
int restart_game()
{
    int choice;
    // Prompt user input
    do
    {
        printf("\nPress 1 to Restart Game");
        printf("\nPress 0 to Exit Game");
        printf("\n\n Your Choice: ");
        scanf("%d", &choice);
        fflush(stdin);

        // if user input is not 1 or 0
        if (choice != 1 && choice != 0)
        {
            // inform the user that the input is invalid
            printf("\nPlease input a valid choice (1 or 0)\n");
        }
    } while (choice != 1 && choice != 0); // if user choice is not 1 or 0, prompt user again

    return choice;
}

// main function
int main()
{
    instruction();
    char start = ' ';
    printf("\n press Enter to start");
    // if value is equal to char start ascii 32
    scanf("%c", &start);
    if (start)
    {
        int user = 1;
        // restart game
        while (user)
        {
            play_game();

            // print final board results
            print_board();

            // switch case to print out who is the winner / if it is a draw
            switch (check_winner())
            {
            //  if draw
            case 0:
                printf("\n\tGame ended on a draw\n");
                break;
            // if computer wins
            case 10:
                printf("\n\tYou lose! HAHAHAHA\n");
                break;
            // if player wins
            case -10:
                printf("\n\tYou win!\n");
                break;
            }
            // restart game prompt
            user = restart_game();
            if (user == 1)
            {
                reset_board();
            }
            printf("\n");
        }
    }
    printf("\nThanks for playing\n");
    return 0;
}