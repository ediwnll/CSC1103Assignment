#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define winStrategy 958
#define maxCol 10

void shuffle(char *array[]){
    int size = winStrategy - 1;
    
    for (int i = size; i > 0 ; i--){
        int j = rand() % (i + 1);
        char *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
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

void populateData(char* dataSet[],char* trainingSet[][8],char* testSet[][8], char* outcome[]){

    int trainingRow = winStrategy * 0.8;
   
    for (int row = 0; row < winStrategy; row ++){
        char *dataRow = dataSet[row];
        char *value = strtok(dataRow, ",");

        if(value == NULL){
            printf("Row %d is empty or does not contain delimiters.\n", row);
            continue; // Skip to the next row if the current one is empty or has no delimiters
        }
        
        for (int col = 0; col < maxCol; col ++){
            if (row < trainingRow && col < maxCol - 2){
                trainingSet[row][col] = value;
            }else if (row > trainingRow && col > maxCol - 2){
                testSet[row][col] = value;
            }else{
                outcome[row] = value;
            }
            value = strtok(NULL, ",");
        }        
    }
    free(dataSet);
}

void main(){

    srand(time(NULL));   
    char* dataSet[winStrategy];
    int trainingSetSize = (winStrategy * 80 / 100);
    
    char* trainingSet[trainingSetSize][8];
    char* testSet[winStrategy-trainingSetSize][8];
    char* outcome[winStrategy];    

    setup(dataSet);
    shuffle(dataSet);
    populateData(dataSet,trainingSet,testSet,outcome);
}