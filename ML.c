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
        for(int ttrow=0; ttrow<9; ttrow++) {
            if(stricmp(trainingSet[trow][ttrow], possibilityAtt[ttrow]->position) == 0) {
                if(strncmp(outcome[trow],"positive",8) == 0) {
                    possibilityAtt[ttrow]->possibility->positive += 1;
                    possibilityLabel->positive += 1;
                } else if(strncmp(outcome[trow], "negative",8) == 0) {
                    possibilityAtt[ttrow]->possibility->negative += 1;
                    possibilityLabel->negative += 1;
                }
                break;
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

    possibilityLabel->positive /= trainingRow;
    possibilityLabel->negative /= trainingRow;

}

void predict() {

}

void cross_validate() {
    
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

void shuffle(char *array[]){    
    for (int i = 0; i < winStrategy - 1; i++){
        int j = rand() % (i + 1);
        char *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void populateData(char* dataSet[], char* trainingSet[][9],char* testSet[][9], char* outcome[]){
    int trainingRow = winStrategy * 0.8;
   
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
                testSet[row][col] = value;
            } else{
                outcome[row] = value;
            }
            value = strtok(NULL, ",");
            
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
    char* testSet[winStrategy-trainingSetSize][9];
    char* outcome[winStrategy];    

    Feature** possibilityAtt = allocateFeatureSet();
    PossibilityLabel* possibilityLabel = allocatePossibility();

    setup(dataSet);
    shuffle(dataSet);
    populateData(dataSet, trainingSet, testSet, outcome);

    // Perform learning and prediction
    learn(possibilityAtt, possibilityLabel, trainingSet, outcome);
    //predict (parameters );

    //computeConfusionMatrix(outcome);
    free_memory(possibilityAtt, possibilityLabel);
}