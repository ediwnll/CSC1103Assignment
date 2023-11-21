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
        strcpy(featureSets[i][0].position, "b");
        strcpy(featureSets[i][1].position, "x");
        strcpy(featureSets[i][2].position, "o");
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

void populateData(char *dataSet[], char *trainingSet[][num_feature], char *testSet[][num_feature], char *outcome[])
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

void learn(char *trainingSet[][num_feature], char *outcome[], int rowSize)
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
            // Skip if the cell is already occupied
            if (gameBoard[row][col] != 0)
            {
                continue;
            }
            // Make a temporary move for evaluation
            gameBoard[row][col] = COMPUTER;

            char *position_value = gameBoard_to_featureSet(gameBoard[row][col]);
            for (int feature = 0; feature < num_feature; ++feature)
            {
                for (int feature_value = 0; feature_value < 3; ++feature_value)
                {
                    if (strcasecmp(position_value, possibilityAtt[feature][0].position) == 0)
                    {
                        result.positive *= possibilityAtt[feature][feature_value].possibility->positive;
                        result.negative *= possibilityAtt[feature][feature_value].possibility->negative;
                        break;
                    }
                }
            }

            // Undo the temporary move
            gameBoard[row][col] = BLANK;
        }
    }

    printf("Intermediate Results - After Prediction:\n");
    printf("Positive Probability: %f\n", result.positive);
    printf("Negative Probability: %f\n", result.negative);

    return result;
}

PossibilityLabel* predictionValue(char* testData[][9]) {
    PossibilityLabel* result = allocatePossibility();
    result->positive=possibilityLabel->positive;
    result->negative=possibilityLabel->negative;

    for (int row=0; row<9; row++) {
        for (int col=0; col<3; col++) {
            if(stricmp(testData[row][col], possibilityAtt[row][col].position) == 0) {
                result->positive *= possibilityAtt[row][col].possibility->positive;
                result->negative *= possibilityAtt[row][col].possibility->negative;
                break;
            }

        }
    }
    return result;
}



void computeConfusionMatrix(char *testSet[][num_feature], char *actualOutcomes[], int testSetSize)
{
    int truePositive = 0, trueNegative = 0, falsePositive = 0, falseNegative = 0;

    for (int row = 0; row < testSetSize; ++row)
    {
        PossibilityLabel* prediction = predictionValue(testSet); // Assuming you have a predict function that takes a row of the test set

        // Compare the predicted probabilities and assign the predicted label
        char predictedLabel[9];
        if (prediction->positive > prediction->negative)
        {
            strcpy(predictedLabel, "positive");
        }
        else
        {
            strcpy(predictedLabel, "negative");
        }

        // Compare the predicted label with the actual label and update the confusion matrix
        if (strncmp(actualOutcomes[row], "positive", 8) == 0 && strncmp(predictedLabel, "positive", 8) == 0)
        {
            truePositive++;
        }
        else if (strncmp(actualOutcomes[row], "negative", 8) == 0 && strncmp(predictedLabel, "negative", 8) == 0)
        {
            trueNegative++;
        }
        else if (strncmp(actualOutcomes[row], "negative", 8) == 0 && strncmp(predictedLabel, "positive", 8) == 0)
        {
            falsePositive++;
        }
        else if (strncmp(actualOutcomes[row], "positive", 8) == 0 && strncmp(predictedLabel, "negative", 8) == 0)
        {
            falseNegative++;
        }
    }

    // Print the confusion matrix
    printf("\nConfusion Matrix:\n");
    printf("True Positive (TP): %d\n", truePositive);
    printf("True Negative (TN): %d\n", trueNegative);
    printf("False Positive (FP): %d\n", falsePositive);
    printf("False Negative (FN): %d\n", falseNegative);
}

double computeAccuracy(char *testSet[][num_feature], char *actualOutcomes[], int testSetSize)
{
    int correctPredictions = 0;

    for (int row = 0; row < testSetSize; ++row)
    {
        PossibilityLabel* prediction = predictionValue(testSet); // Assuming you have a predict function that takes a row of the test set

        // Compare the predicted probabilities and assign the predicted label
        char predictedLabel[9];
        if (prediction->positive > prediction->negative)
        {
            strcpy(predictedLabel, "positive");
        }
        else
        {
            strcpy(predictedLabel, "negative");
        }

        // Check if the predicted label matches the actual label
        if (strncmp(actualOutcomes[row], predictedLabel, 8) == 0)
        {
            correctPredictions++;
        }
    }

    // Calculate and return the accuracy
    double accuracy = (double)correctPredictions / (double)testSetSize;
    return accuracy;
}

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

char *gameBoard_to_featureSet(int value)
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
    int testSetSize = winStrategy - trainingSetSize;

    char *trainingSet[trainingSetSize][num_feature];
    char *testSet[testSetSize][num_feature];
    char *outcome[winStrategy];

    possibilityAtt = allocateFeatureSet();
    possibilityLabel = allocatePossibility();

    setup(dataSet);
    shuffle(dataSet);
    populateData(dataSet, trainingSet, testSet, outcome);

    // Learn and evaluate on both training and test sets
    learn(trainingSet, outcome, trainingSetSize);
    computeConfusionMatrix(testSet,outcome,testSetSize);
    double accuracy = computeAccuracy(testSet,outcome,testSetSize);
    printf("%f",accuracy * 100);    
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
                // Use the Naive Bayes model to predict the outcome of this move
                PossibilityLabel prediction = predict();

                double moveProbability = prediction.negative;
                // Compare the predicted probabilities
                if (moveProbability > bestProbability)
                {
                    bestProbability = moveProbability;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
    }

    if (bestProbability != -1)
    {
        // Make the best move according to the Naive Bayes model
        handle_player_move(bestRow, bestCol);
    }
}
