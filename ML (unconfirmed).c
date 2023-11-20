#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <time.h>

#define winStrategy 958
#define maxCol 10

#define num_feature_sets 9
#define num_feature 3

typedef struct {
    float positive;
    float negative;
} PossibilityLabel;

typedef struct {
    char position[2];
    PossibilityLabel* possibility;
} Feature;

typedef char* DataRow[9];

PossibilityLabel* allocatePossibility() {
    PossibilityLabel* possibility =  (PossibilityLabel *)malloc(sizeof(PossibilityLabel));
    if(possibility == NULL) {
        return NULL;
    }

    possibility->positive = 0.0;
    possibility->negative = 0.0;

    return possibility;
}

Feature** allocateFeatureSet() {
    Feature** featureSets = (Feature **) malloc(num_feature_sets * sizeof(Feature*));
    for (int i = 0; i < num_feature_sets; i++) {
        featureSets[i] = (Feature *)malloc(num_feature * sizeof(Feature));
        for (int j = 0; j < num_feature; j++) {
            featureSets[i][j].possibility = allocatePossibility();
        }
        strcpy(featureSets[i][0].position, "o");
        strcpy(featureSets[i][1].position, "x");
        strcpy(featureSets[i][2].position, "b");
    }
    return featureSets;
}

void setup(char *array[]){
    char data[winStrategy];
    FILE *MLfile = fopen("tic-tac-toe.data","r");
    if(MLfile==NULL){
        printf("ERROR: Could not open");
    }

    int rows = 0;
    while(fgets(data,sizeof(data),MLfile)){
        array[rows] = strdup(data);
        rows++;   
    }

    fclose(MLfile);
}

void learn(Feature** possibilityAtt, PossibilityLabel* possibilityLabel, char* trainingSet[][9], char* outcome[]) {
    int trainingRow = winStrategy * 0.8;

    //training data row
    for(int trow=0; trow<trainingRow; trow++) {
         if(strncmp(outcome[trow],"positive",8) == 0) {
            possibilityLabel->positive += 1;
        } else if(strncmp(outcome[trow], "negative",8) == 0) {
            possibilityLabel->negative += 1;
        }

        for(int ttrow=0; ttrow<9; ttrow++) {
            for(int col=0; col<3; col++) {
                if(stricmp(trainingSet[trow][ttrow], possibilityAtt[ttrow][col].position) == 0) {
                    if(strncmp(outcome[trow],"positive",8) == 0) {
                        possibilityAtt[ttrow][col].possibility->positive += 1;
                    } else if(strncmp(outcome[trow], "negative",8) == 0) {
                        possibilityAtt[ttrow][col].possibility->negative += 1;
                    }
                    break;
                }
            }
        }
    }

    for (int tts=0 ; tts<9; tts++) {
        Feature* feature = possibilityAtt[tts];
        for (int col=0; col <3; col++) {
            feature[col].possibility->positive /= possibilityLabel->positive;
            feature[col].possibility->negative /= possibilityLabel->negative;
        }
    }

    for (int tts=0 ; tts<9; tts++) {
        Feature* feature = possibilityAtt[tts];
        for (int col=0; col <3; col++) {
            printf("%s -> %f, %f...", feature[col].position, feature[col].possibility->positive, feature[col].possibility->negative);
        }
        printf("\n");
    }

    possibilityLabel->positive /= trainingRow;
    possibilityLabel->negative /= trainingRow;
}

PossibilityLabel* predict(Feature** possibilityAtt, PossibilityLabel* possibilityLabel, char** randomData) {
    PossibilityLabel* result = allocatePossibility();
    result->positive=possibilityLabel->positive;
    result->negative=possibilityLabel->negative;

    for (int row=0; row<9; row++) {
        for (int col=0; col<3; col++) {
            if(stricmp(randomData[row], possibilityAtt[row][col].position) == 0) {
                result->positive *= possibilityAtt[row][col].possibility->positive;
                result->negative *= possibilityAtt[row][col].possibility->negative;
                break;
            }

        }
    }

    return result;
}

void free_memory(Feature** possibilityAtt, PossibilityLabel* possibilityLabel) {
    if (possibilityAtt == NULL) {
        return;
    }

    for (int i = 0; i < num_feature_sets; i++) {
        if (possibilityAtt[i] == NULL) {
            continue;
        }

        for (int j = 0; j < num_feature; j++) {
            free(possibilityAtt[i][j].possibility);
        }
        free(possibilityAtt[i]);
    }

    free(possibilityAtt);
    free(possibilityLabel);
}

void cross_validate(char* dataSet[], char* outcomes[], int dataSetSize, int folds) {
   int partSize = dataSetSize / folds;
    float totalAccuracy = 0.0;

    for (int i = 0; i < folds; i++) {
        int testStart = i * partSize;
        int testEnd = (i + 1) * partSize;

        DataRow* trainingSet = (DataRow*)malloc((dataSetSize - partSize) * sizeof(DataRow));
        DataRow* testSet = (DataRow*)malloc(partSize * sizeof(DataRow));
        char** trainingOutcomes = (char**)malloc((dataSetSize - partSize) * sizeof(char*));
        char** testOutcomes = (char**)malloc(partSize * sizeof(char*));

        int trainingIndex = 0, testIndex = 0;
        for (int j = 0; j < dataSetSize; j++) {
            DataRow row;
            char *token = strtok(dataSet[j], ",");
            for (int k = 0; k < 9 && token != NULL; k++) {
                row[k] = strdup(token);
                token = strtok(NULL, ",");
            }

            if (j >= testStart && j < testEnd) {
                memcpy(testSet[testIndex], row, sizeof(DataRow));
                testIndex++;
            } else {
                memcpy(trainingSet[trainingIndex], row, sizeof(DataRow));
                trainingIndex++;
            }

            for (int k = 0; k < 9; k++) {
                free(row[k]);
            }
        }

        Feature** possibilityAtt = allocateFeatureSet();
        PossibilityLabel* possibilityLabel = allocatePossibility();
        learn(possibilityAtt, possibilityLabel, trainingSet, trainingOutcomes);

        int correctPredictions = 0;
        for (int j = 0; j < testIndex; j++) {
            PossibilityLabel* prediction = predict(possibilityAtt, possibilityLabel, testSet[j]);
            if ((prediction->positive > prediction->negative && strcmp(testOutcomes[j], "positive") == 0) ||
                (prediction->negative > prediction->positive && strcmp(testOutcomes[j], "negative") == 0)) {
                correctPredictions++;
            }
            free(prediction);
        }

        float accuracy = (float)correctPredictions / testIndex;
        totalAccuracy += accuracy;

        free(trainingSet);
        free(testSet);
        free(trainingOutcomes);
        free(testOutcomes);
        free_memory(possibilityAtt, possibilityLabel);
    }
}

void shuffle(char *array[]){    
    for (int i = 0; i < winStrategy - 1; i++){
        int j = rand() % (i + 1);
        char *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

char** randomizeData(char *array[][9]){
    int random = rand() % (winStrategy * (20 / 100) - 1);
    return array[random];
}

void populateData(char* dataSet[], char* trainingSet[][9], char* testSet[][9], char* outcome[]){
    int trainingRow = winStrategy * 0.8;
    int testRow = 0;
    for (int row = 0; row < winStrategy; row ++){
        char *dataRow = dataSet[row];
        char *value = strtok(dataRow, ",");

        if(value == NULL){
            printf("Row %d is empty or does not contain delimiters.\n", row);
            continue; // Skip to the next row if the current one is empty or has no delimiters
        }
        
        for (int col = 0; col < maxCol; col ++){
            if (row < trainingRow && col < maxCol - 1){
                trainingSet[row][col] = value;
            } else if (row > trainingRow && col < maxCol - 1){
                testSet[testRow][col] = value;
            } else{
                outcome[row] = value;
            }
            value = strtok(NULL, ",");
            
        }

        if (row > trainingRow) {
            testRow++;
        }
    }
}

void computeConfusionMatrix(char* outcome){
    int error = 1 / winStrategy;
    int TP,TN,FP,FN;


    printf("True Positive: %d ,\n True Negative: %d ,\n False Positive: %d,\n False Negative :%d");
}

void main(){
    srand(time(NULL));   
    char* dataSet[winStrategy];
    int trainingSetSize = (winStrategy * 80 / 100);
    
    char* trainingSet[trainingSetSize][9];
    char* testSet[192][9];
    char* outcome[winStrategy];
    int folds, dataSetSize;    

    Feature** possibilityAtt = allocateFeatureSet();
    PossibilityLabel* possibilityLabel = allocatePossibility();

    setup(dataSet);
    shuffle(dataSet);
    populateData(dataSet, trainingSet, testSet, outcome);
    char** randomData = trainingSet[0]; //randomizeData(testSet);

    // Perform learning and prediction
    learn(possibilityAtt, possibilityLabel, trainingSet, outcome);
    PossibilityLabel* result = predict(possibilityAtt, possibilityLabel, randomData);

    cross_validate(dataSet,outcome, dataSetSize, folds);
    //computeConfusionMatrix(outcome);
    free_memory(possibilityAtt, possibilityLabel);
}