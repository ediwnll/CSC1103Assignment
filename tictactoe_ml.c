#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "tictactoe.h"

Feature **possibilityAtt;
PossibilityLabel *possibilityLabel;
DataInstance *dataSet;
DataInstance *trainingSet;
DataInstance *testSet;

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

void setup(DataInstance *array)
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
        char *value = strtok(data, ",");
        int col = 0;

        while (value != NULL && col < num_feature + 1) // +1 for label
        {
            if (col < num_feature)
            {
                strcpy(array[rows].features[col], value);
            }
            else
            {
                strcpy(array[rows].label, value);
            }

            value = strtok(NULL, ",");
            col++;
        }

        rows++;
    }

    fclose(MLfile);
}

// function to shuffle the dataset
void shuffle(DataInstance *array)
{
    for (int i = 0; i < winStrategy - 1; i++)
    {
        // generate a random index within current range
        int j = rand() % (i + 1);

        // swap the current element with a randomly chosen element
        DataInstance temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// function to split the dataset into train and test data
void populateData(DataInstance *dataSet, DataInstance *trainingSet, DataInstance *testSet, int dataSize, int trainingSize)
{
    // iterate through all rows of data
    for (int row = 0; row < dataSize; row++)
    {

        if (row < trainingSize)
        {
            memcpy(&trainingSet[row], &dataSet[row], sizeof(DataInstance));
        }
        else
        {
            memcpy(&testSet[row - trainingSize], &dataSet[row], sizeof(DataInstance));
        }
    }
}

void learn(DataInstance *trainingSet, int rowSize)
{
    // training data row
    for (int trow = 0; trow < rowSize; trow++)
    {
        // update counts for positive and negative labels
        if (strncmp(trainingSet[trow].label, "positive", 8) == 0)
        {
            possibilityLabel->positive += 1;
        }
        else if (strncmp(trainingSet[trow].label, "negative", 8) == 0)
        {
            possibilityLabel->negative += 1;
        }

        // update feature probabilities based on the training set
        for (int ttrow = 0; ttrow < num_feature; ttrow++)
        {
            for (int col = 0; col < 3; col++)
            {
                // check if the current feature value matches the current position in possibilityAtt
                if (strcasecmp(trainingSet[trow].features[ttrow], possibilityAtt[ttrow][col].position) == 0)
                {
                    // update counts based on the label for current feature value
                    if (strncmp(trainingSet[trow].label, "positive", 8) == 0)
                    {
                        possibilityAtt[ttrow][col].possibility->positive += 1;
                    }
                    else if (strncmp(trainingSet[trow].label, "negative", 8) == 0)
                    {
                        possibilityAtt[ttrow][col].possibility->negative += 1;
                    }
                    break;
                }
            }
        }
    }

    // normalize feature probabilities by dividing the total counts for each feature
    for (int tts = 0; tts < 9; tts++)
    {
        Feature *feature = possibilityAtt[tts];
        for (int col = 0; col < 3; col++)
        {
            feature[col].possibility->positive /= possibilityLabel->positive;
            feature[col].possibility->negative /= possibilityLabel->negative;
        }
    }

    // Normalize label probabilities by dividing by the total number of rows in the training set
    possibilityLabel->positive /= rowSize;
    possibilityLabel->negative /= rowSize;
}

PossibilityLabel predict(DataInstance *testData)
{
    PossibilityLabel result;
    result.positive = possibilityLabel->positive;
    result.negative = possibilityLabel->negative;

    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (gameBoard[row][col] == BLANK)
            {
                // Use the Naive Bayes model to update probabilities
                for (int feature = 0; feature < num_feature; ++feature)
                {
                    for (int feature_value = 0; feature_value < 3; ++feature_value)
                    {
                        if (strcasecmp(testData->features[feature], possibilityAtt[feature][feature_value].position) == 0)
                        {
                            result.positive *= possibilityAtt[feature][feature_value].possibility->positive;
                            result.negative *= possibilityAtt[feature][feature_value].possibility->negative;
                            break;
                        }
                    }
                }
            }
        }
    }
    return result;
}

void computeConfusionMatrix(DataInstance *testSet, int testSetSize)
{
    int truePositive = 0, trueNegative = 0, falsePositive = 0, falseNegative = 0;

    for (int row = 0; row < testSetSize; ++row)
    {
        PossibilityLabel prediction = predict(&testSet[row]);

        // Compare the predicted probabilities and assign the predicted label
        char predictedLabel[9];
        if (prediction.positive > prediction.negative)
        {
            strcpy(predictedLabel, "positive");
        }
        else
        {
            strcpy(predictedLabel, "negative");
        }

        // Compare the predicted label with the actual label and update the confusion matrix
        if (strncmp(testSet[row].label, "positive", 8) == 0 && strncmp(predictedLabel, "positive", 8) == 0)
        {
            truePositive++;
        }
        else if (strncmp(testSet[row].label, "negative", 8) == 0 && strncmp(predictedLabel, "negative", 8) == 0)
        {
            trueNegative++;
        }
        else if (strncmp(testSet[row].label, "negative", 8) == 0 && strncmp(predictedLabel, "positive", 8) == 0)
        {
            falsePositive++;
        }
        else if (strncmp(testSet[row].label, "positive", 8) == 0 && strncmp(predictedLabel, "negative", 8) == 0)
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

double computeAccuracy(DataInstance *testSet, int testSetSize)
{
    int correctPredictions = 0;

    for (int row = 0; row < testSetSize; ++row)
    {
        PossibilityLabel prediction = predict(&testSet[row]);

        // Compare the predicted probabilities and assign the predicted label
        char predictedLabel[9];
        if (prediction.positive > prediction.negative)
        {
            strcpy(predictedLabel, "positive");
        }
        else
        {
            strcpy(predictedLabel, "negative");
        }

        // Check if the predicted label matches the actual label
        if (strncmp(testSet[row].label, predictedLabel, 8) == 0)
        {
            correctPredictions++;
        }
    }

    // Calculate and return the accuracy
    double accuracy = (double)correctPredictions / (double)testSetSize;
    return accuracy;
}

void fill_features_from_board(DataInstance *dataInstance)
{
    // Iterate through each cell in the game board
    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            // Switch based on the content of the current cell in the game board
            switch (gameBoard[row][col])
            {
            case BLANK:
                // If the cell is blank, set the corresponding feature to "b"
                strcpy(dataInstance->features[row * SIZE + col], "b");
                break;
            case PLAYER:
                // If the cell is occupied by the player, set the corresponding feature to "x"
                strcpy(dataInstance->features[row * SIZE + col], "x");
                break;
            case COMPUTER:
                // If the cell is occupied by the computer, set the corresponding feature to "o"
                strcpy(dataInstance->features[row * SIZE + col], "o");
                break;
            default:
                break;
            }
        }
    }
}

void naivebayes()
{
    srand(time(NULL));
    dataSet = (DataInstance *)malloc(winStrategy * sizeof(DataInstance));
    int trainingSetSize = (winStrategy * 80 / 100);  // training set 80% of dataset
    int testSetSize = winStrategy - trainingSetSize; // test set is the remaining 20%

    trainingSet = (DataInstance *)malloc(trainingSetSize * sizeof(DataInstance));
    testSet = (DataInstance *)malloc(testSetSize * sizeof(DataInstance));

    possibilityAtt = allocateFeatureSet();
    possibilityLabel = allocatePossibility();

    setup(dataSet);
    shuffle(dataSet);
    populateData(dataSet, trainingSet, testSet, winStrategy, trainingSetSize);

    // Learn and evaluate on both training and test sets
    learn(trainingSet, trainingSetSize);

    // Print confusion matrix for training set
    printf("Train Set ");
    computeConfusionMatrix(trainingSet, trainingSetSize);
    // Print confusion matrix for test set
    printf("Test Set ");
    computeConfusionMatrix(testSet, testSetSize);
    double accuracy = computeAccuracy(testSet, testSetSize);
    printf("Model Accuracy: %.2f\n", accuracy * 100);

    // free(dataSet);
    // free(trainingSet);
    // free(testSet);
}

void naivebayes_move(int isTesting)
{
    int bestRow = -1, bestCol = -1;
    double bestProbability = -1.0;
    double moveProbability = 0.0;

    // Iterate through empty spaces on the board and evaluate each move
    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            if (gameBoard[row][col] == BLANK)
            {
                // Make a temporary move for evaluation
                if (isTesting)
                {
                    gameBoard[row][col] = PLAYER; // TESTING: Make player move
                }
                else
                {
                    gameBoard[row][col] = COMPUTER;
                }

                // Create a data instance representing the current board state
                DataInstance currentBoardState;
                // Fill Features based on current board state
                fill_features_from_board(&currentBoardState);

                // Use the Naive Bayes model to predict the outcome of this move
                PossibilityLabel prediction = predict(&currentBoardState);

                // Undo the temporary move
                gameBoard[row][col] = BLANK;

                if (isTesting)
                {
                    moveProbability = prediction.positive;
                }
                else
                {
                    moveProbability = prediction.negative;
                }
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
