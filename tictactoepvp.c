// CSC1103 Tic Tac Toe Human vs Human
#include <stdio.h>

// declare global tictactoe board
char board[3][3];

// function prototypes

// instruction for the game
void instruction();
// reset board till it's empty and only shows the cell number
void reset_board();
// print out the board with the player signs
void print_board();
// update the board with the respective player sign on the board
int update_board(int cell, char player_sign);
// to check if there are any winning row/columns/diagonals
int check_winner(char player_sign);
// to play the tictactoe game
void play_game();
// restart game function or quit game
int restart_game();

void instruction()
{
    // print out instructions and clear the board
    printf("\nTic Tac Toe Player vs Player\n\n");
    printf("\n\tInstructions\n");
    printf("\tPlayer 1 sign =  X\n");
    printf("\tPlayer 2 sign =  O\n\n");
    printf("\tTo quit the game, Enter -1\n");
    printf("\n\nEnter the cell numbers on the board to place your marks\n");

    reset_board();
}

// to reset the board to the cell numbers
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

// print the game board after each move or game starts
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

int update_board(int cell, char player_sign)
{
    int row = (cell - 1) / 3;
    int column = (cell - 1) % 3;
    int is_valid = 1;

    // check that the space is filled
    if (board[row][column] != ' ')
    {
        printf("\nCell is already filled. Choose another cell.\n");
        is_valid = 0;
    }
    // else cell is not filled, fill it with player sign
    else
    {
        board[row][column] = player_sign;
    }
    // print out the board with the player sign
    print_board();
    return is_valid;
}

int check_winner(char player_sign)
{
    // check for a winner in rows
    if (board[0][0] == player_sign && board[0][1] == player_sign && board[0][2] == player_sign ||
        board[1][0] == player_sign && board[1][1] == player_sign && board[1][2] == player_sign ||
        board[2][0] == player_sign && board[2][1] == player_sign && board[2][2] == player_sign)
    {
        return 1;
    }
    // check for a winner in column
    else if (board[0][0] == player_sign && board[1][0] == player_sign && board[2][0] == player_sign ||
             board[0][1] == player_sign && board[1][1] == player_sign && board[2][1] == player_sign ||
             board[0][2] == player_sign && board[1][2] == player_sign && board[2][2] == player_sign)
    {
        return 1;
    }
    // check for a winner in diagonal
    else if (board[0][0] == player_sign && board[1][1] == player_sign && board[2][2] == player_sign ||
             board[0][2] == player_sign && board[1][1] == player_sign && board[2][0] == player_sign)
    {
        return 1;
    }
    // no winner
    return 0;
}

void play_game()
{
    int game_result = 0;
    int cell = 0;
    int play_count = 0;
    int update_result = 1;

    char player_sign = ' ';
    // they can only play until till 9 times because there are only 9 cells. play_cout will only increment on successful turns
    while (!game_result && play_count < 9)
    {
        if (play_count % 2 == 0)
        {
            // player 1
            printf("\nPlayer 1 uses [X]: ");
            player_sign = 'X';
        }
        else
        {
            // player 2
            printf("\nPlayer 2 uses [O]: ");
            player_sign = 'O';
        }
        scanf("%d", &cell);
        // check to ensure input is between 1 to 0
        if (cell > 0 && cell < 10)
        {
            // update result will return as 1 or 0 due to is_valid on update_board()
            update_result = update_board(cell, player_sign);
            // if update_result = 1
            if (update_result)
            {
                // check for winner and game_result wil return as 1 or 0 due to check_winner()
                game_result = check_winner(player_sign);
                // if game_result = 1
                if (game_result)
                {
                    // if player_sign is X, it will print out as player 1 wins, else it prints player 2 wins
                    printf("\t Player %d wins!\n", player_sign == 'X' ? 1 : 2);
                }
                // play count increases if no winner
                play_count++;
            }
        }
        // if they choose -1, game ends immediately
        else if (cell == -1)
        {
            printf("\n\t Game has ended\n");
            return;
        }
        // if they choose other value than 1-10 or -1, tell them to enter the value again. all this happens in a while loop.
        else
        {
            printf("\n Please enter a valid value\n");
        }
    }
    // if no one wins the game, resulting in draw
    if (!game_result && play_count == 9)
    {
        printf("\n\tGame ended on a draw\n");
    }
    printf("\n\t Game Over\n");
}

int restart_game()
{
    int choice;
    printf("\nPress 1 to Restart Game");
    printf("\nPress 0 to Exit Game");
    printf("\n\n Your Choice: ");
    scanf("%d", &choice);
    return choice;
}

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
            user = restart_game();
            if (user)
            {
                reset_board();
            }
            printf("\n");
        }
    }
    printf("\nThanks for playing\n");
    return 0;
}