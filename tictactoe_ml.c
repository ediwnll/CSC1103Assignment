#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "tictactoe.h"

Feature **possibilityAtt;
PossibilityLabel *possibilityLabel;

PossibilityLabel *allocatePossibility()
{
    PossibilityLabel *possibility = (PossibilityLabel *)malloc(sizeof(PossibilityLabel));
    if (possibility == NULL)
    {
        return NULL;
    }

    possibility->positive = 0.0;
    possibility->negative = 0.0;

    return possibility;
}

Feature **allocateFeatureSet()
{
    Feature **featureSets = (Feature **)malloc(num_feature_sets * sizeof(Feature *));
    for (int i = 0; i < num_feature_sets; i++)
    {
        featureSets[i] = (Feature *)malloc(num_feature * sizeof(Feature));
        for (int j = 0; j < num_feature; j++)
        {
            featureSets[i][j].possibility = allocatePossibility();
        }
        strcpy(featureSets[i][0].position, "o");
        strcpy(featureSets[i][1].position, "x");
        strcpy(featureSets[i][2].position, "b");
    }
    return featureSets;
}

void setup(char *array[])
{
    char data[winStrategy];
    FILE *MLfile = fopen("tic-tac-toe.data", "r");
    if (MLfile == NULL)
    {
        printf("ERROR: Could not open");
    }

    int rows = 0;
    while (fgets(data, sizeof(data), MLfile))
    {
        array[rows] = strdup(data);
        rows++;
    }

    fclose(MLfile);
}

void shuffle(char *array[])
{
    for (int i = 0; i < winStrategy - 1; i++)
    {
        int j = rand() % (i + 1);
        char *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// char **randomizeData(char *array[][9])
// {
//     int random = rand() % (winStrategy * (20 / 100) - 1);
//     return array[random];
// }

void populateData(char *dataSet[], char *trainingSet[][9], char *testSet[][9], char *outcome[])
{
    int trainingRow = winStrategy * 0.8;
    int testRow = 0;
    for (int row = 0; row < winStrategy; row++)
    {
        char *dataRow = dataSet[row];
        char *value = strtok(dataRow, ",");

        if (value == NULL)
        {
            printf("Row %d is empty or does not contain delimiters.\n", row);
            continue; // Skip to the next row if the current one is empty or has no delimiters
        }

        for (int col = 0; col < maxCol; col++)
        {
            if (row < trainingRow && col < maxCol - 1)
            {
                trainingSet[row][col] = value;
            }
            else if (row > trainingRow && col < maxCol - 1)
            {
                testSet[testRow][col] = value;
            }
            else
            {
                outcome[row] = value;
            }
            value = strtok(NULL, ",");
        }

        if (row > trainingRow)
        {
            testRow++;
        }
    }
}

void learn(char *trainingSet[][9], char *outcome[], int rowSize)
{
    // training data row
    for (int trow = 0; trow < rowSize; trow++)
    {
        if (strncmp(outcome[trow], "positive", 8) == 0)
        {
            possibilityLabel->positive += 1;
        }
        else if (strncmp(outcome[trow], "negative", 8) == 0)
        {
            possibilityLabel->negative += 1;
        }

        for (int ttrow = 0; ttrow < num_feature; ttrow++)
        {
            for (int col = 0; col < 3; col++)
            {

                if (strcasecmp(trainingSet[trow][ttrow], possibilityAtt[ttrow][col].position) == 0)
                {
                    if (strncmp(outcome[trow], "positive", 8) == 0)
                    {
                        possibilityAtt[ttrow][col].possibility->positive += 1;
                    }
                    else if (strncmp(outcome[trow], "negative", 8) == 0)
                    {
                        possibilityAtt[ttrow][col].possibility->negative += 1;
                    }
                    break;
                }
            }
        }
    }

    for (int tts = 0; tts < 9; tts++)
    {
        Feature *feature = possibilityAtt[tts];
        for (int col = 0; col < 3; col++)
        {
            feature[col].possibility->positive /= possibilityLabel->positive;
            feature[col].possibility->negative /= possibilityLabel->negative;
        }
    }

    for (int tts = 0; tts < 9; tts++)
    {
        Feature *feature = possibilityAtt[tts];
        for (int col = 0; col < 3; col++)
        {
            printf("%s -> %f, %f...", feature[col].position, feature[col].possibility->positive, feature[col].possibility->negative);
        }
        printf("\n");
    }

    possibilityLabel->positive /= rowSize;
    possibilityLabel->negative /= rowSize;
}

PossibilityLabel predict()
{
    PossibilityLabel result;
    result.positive = possibilityLabel->positive;
    result.negative = possibilityLabel->negative;

    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            const char *position_value = gameBoard_to_featureSet(gameBoard[row][col]);
            for (int feature = 0; feature < num_feature; ++feature)
            {
                if (strcasecmp(position_value, possibilityAtt[feature][0].position) == 0)
                {
                    result.positive *= possibilityAtt[feature][0].possibility->positive;
                    result.negative *= possibilityAtt[feature][0].possibility->negative;
                    break;
                }
                else if (strcasecmp(position_value, possibilityAtt[feature][1].position) == 0)
                {
                    result.positive *= possibilityAtt[feature][1].possibility->positive;
                    result.negative *= possibilityAtt[feature][1].possibility->negative;
                    break;
                }
                else if (strcasecmp(position_value, possibilityAtt[feature][2].position) == 0)
                {
                    result.positive *= possibilityAtt[feature][2].possibility->positive;
                    result.negative *= possibilityAtt[feature][2].possibility->negative;
                    break;
                }
            }
        }
    }

    return result;
}

// void cross_validate(char *dataSet[], int dataSetSize, float *accuracy, int folds)
// {
// }

void free_memory()
{
    if (possibilityAtt == NULL)
    {
        return;
    }

    for (int i = 0; i < num_feature_sets; i++)
    {
        if (possibilityAtt[i] == NULL)
        {
            continue;
        }

        for (int j = 0; j < num_feature; j++)
        {
            free(possibilityAtt[i][j].possibility);
        }
        free(possibilityAtt[i]);
    }

    free(possibilityAtt);
    free(possibilityLabel);
}

// void computeConfusionMatrix(char *outcome)
// {
//     int error = 1 / winStrategy;
//     int TP, TN, FP, FN;

//     // printf("True Positive: %d ,\n True Negative: %d ,\n False Positive: %d,\n False Negative :%d");
// }

const char *gameBoard_to_featureSet(int value)
{
    switch (value)
    {
    case 0:
        return "b";
    case 1:
        return "x";
    case 2:
        return "o";
    default:
        return "";
    }
}
void naivebayes()
{
    srand(time(NULL));
    char *dataSet[winStrategy];
    int trainingSetSize = (winStrategy * 80 / 100);

    char *trainingSet[trainingSetSize][9];
    char *testSet[winStrategy - trainingSetSize][9];
    char *outcome[winStrategy];

    possibilityAtt = allocateFeatureSet();
    possibilityLabel = allocatePossibility();

    setup(dataSet);
    shuffle(dataSet);
    populateData(dataSet, trainingSet, testSet, outcome);
    // char **randomData = randomizeData(trainingSet);

    // Perform learning and prediction
    learn(trainingSet, outcome, trainingSetSize);

    // cross_validate();
    // computeConfusionMatrix(outcome);
    // free_memory(possibilityAtt, possibilityLabel);
}

void naivebayes_move()
{
    int bestRow = -1, bestCol = -1;
    double bestProbability = -1.0;

    // Iterate through empty spaces on the board and evaluate each move
    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            if (gameBoard[row][col] == BLANK)
            {
                // Make a temporary move for evaluation
                gameBoard[row][col] = COMPUTER;

                // Use the Naive Bayes model to predict the outcome of this move
                PossibilityLabel prediction = predict();

                // Compare the predicted probabilities
                float winProbability = prediction.negative;
                printf("%f\n", winProbability);
                printf("%f\n", prediction.positive);
                if (winProbability > bestProbability)
                {
                    bestProbability = winProbability;
                    bestRow = row;
                    bestCol = col;
                }

                // Undo the temporary move
                gameBoard[row][col] = BLANK;
            }
        }
    }

    if (bestProbability != -1)
    {
        // Make the best move according to the Naive Bayes model
        handle_player_move(bestRow, bestCol);
    }
}
