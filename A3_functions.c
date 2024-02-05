#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "A3_functions.h"

// TODO create (at least) 5 these functions:
// reference: splitLinesIntoStrings function obtained from lecture W9L3 slide 23, and fileIO.zip 

void checkForFile(char filename[]) {
    FILE* fp;

    do {
        fp = fopen(filename, "r");

        if (fp == NULL) {
            printf("ERROR: File %s does not exist\n", filename);
            scanf("%s", filename);
        }
        else {
            fclose(fp);
            break;
        }
    } while (1);
}

int readLine(char line[], int size, FILE* fp) {
    if (feof(fp)) {
        return 0;
    }

    // read the next line of the file into the string line
    fgets(line, size, fp);

    // fgets leaves a '\n' at the end of the string, the next line will remove it.
    line[strlen(line) - 1] = '\0';

    return 1;
}

int splitLineIntoStrings(char line[], char words[][MAX_STRING_SIZE], char seperators[]) {
    int wordCount = 0;

    // use same tokenizer from before
    char* token = strtok(line, seperators);

    while (token != NULL) {
        strcpy(words[wordCount], token);

        token = strtok(NULL, seperators);
        wordCount++;
    }
    // use strcpy to copy token into array

    return wordCount;
}

float getAverage(float pollutants[24]){
    float average = 0;
    int count = 0;

    for (int i = 0; i < 24; i++) {
        if (pollutants[i] > 0 && pollutants[i] < 9000) {
            average += pollutants[i];
            count++;
        }
    }
    
    if (average == 0) {
        return 0;
    }
    else {
        average /= count;

        return average;
    }
}

int readAPollutantFile(char filename[], char location[], float pollutantAverages[], char dates[][MAX_STRING_SIZE]) {
    char line[200];
    char words[40][MAX_STRING_SIZE];
    int wordCount = 0;
    int days = 0;
    float pollutants[24];
    FILE* fp;

    fp = fopen(filename, "r");

    for (int i = 0; i < 4; i++) {
        readLine(line, MAX_STRING_SIZE, fp);
    }
    splitLineIntoStrings(line, words, ",");
    splitLineIntoStrings(words[0], words, " ");
    strcpy(location, words[0]);

    do {
        readLine(line, MAX_STRING_SIZE, fp);
        splitLineIntoStrings(line, words, ",");

        if (strcmp(words[0], "Station ID") == 0) {
            break;
        }
    } while (1);

    do {
        if (feof(fp)) {
            break;
        }
        else {
            readLine(line, 200, fp);
            wordCount = splitLineIntoStrings(line, words, ",");

            if (strlen(line) == 0 || days == MAX_DATES) {
                break;
            }
            else {
                strcpy(dates[days], words[2]);

                for (int i = 3; i < wordCount; i++) {
                    pollutants[i - 3] = atof(words[i]);
                }
                pollutantAverages[days] = getAverage(pollutants);

                days++;
            }
        }
    } while (1);

    fclose(fp);

    return days;
}

struct Location readData(){
    char filename[MAX_STRING_SIZE];
    char line[MAX_STRING_SIZE];
    char date[MAX_STRING_SIZE];
    char check[MAX_STRING_SIZE];
    char words[20][MAX_STRING_SIZE];
    int month;
    int wordCount = 0;
    Location newLocation;

    do {
        printf("Please enter the filename for the O3 csv file.\n");
        scanf("%s", filename);
        checkForFile(filename);
        strcpy(line, filename);
        wordCount = splitLineIntoStrings(line, words, "_");
        strcpy(check, words[0]);
        
        if (strcmp(words[wordCount - 1], "o3.csv") == 0) {
            newLocation.numDays = readAPollutantFile(filename, newLocation.locationName, newLocation.pollutantAverages[0], newLocation.dates);
            break;
        }
    } while (1);

    do {
        printf("Please enter the filename for the N02 csv file for %s.\n", newLocation.locationName);
        scanf("%s", filename);
        checkForFile(filename);
        strcpy(line, filename);
        wordCount = splitLineIntoStrings(line, words, "_");

        if (strcmp(words[0], check) == 0) {
            if (strcmp(words[wordCount - 1], "no2.csv") == 0) {
                newLocation.numDays = readAPollutantFile(filename, newLocation.locationName, newLocation.pollutantAverages[1], newLocation.dates);
                break;
            }
        }
    } while (1);


    do {
        printf("Please enter the filename for the PM2_5 csv file for %s.\n", newLocation.locationName);
        scanf("%s", filename);
        checkForFile(filename);
        strcpy(line, filename);
        wordCount = splitLineIntoStrings(line, words, "_");

        if (strcmp(words[0], check) == 0) {
            if (strcmp(words[wordCount - 1], "5.csv") == 0) {
                newLocation.numDays = readAPollutantFile(filename, newLocation.locationName, newLocation.pollutantAverages[2], newLocation.dates);
                break;
            }
        }
    } while (1);
    
    strcpy(date, newLocation.dates[0]);
    splitLineIntoStrings(date, words, "-");
    month = atoi(words[1]);

    for (int i = 0; i < newLocation.numDays; i++) {
        newLocation.standardAQHIs[i] = roundAQHI(standardAQHI(newLocation.pollutantAverages[0][i], newLocation.pollutantAverages[1][i], newLocation.pollutantAverages[2][i]));
        
        if (month < 4 || month > 9) {
            newLocation.seasonalAQHIs[i] = roundAQHI(coolAQHI(newLocation.pollutantAverages[1][i], newLocation.pollutantAverages[2][i]));
        }
        else {
            newLocation.seasonalAQHIs[i] = roundAQHI(warmAQHI(newLocation.pollutantAverages[0][i], newLocation.pollutantAverages[1][i], newLocation.pollutantAverages[2][i]));
        }
    }

    return newLocation;
}

void printTableByDate(Location locations[], int numLocations){
    if (numLocations != 0) {
        int choice;
        int check = 0;
        char date[MAX_STRING_SIZE];
        char location[MAX_STRING_SIZE] = "Location";

        do {
            printf("\nChoose your AQHI settings:\n");
            printf("\t1. Calculate Standard Conditions\n");
            printf("\t2. Calculate Seasonal Conditions\n");

            scanf("%d", &choice);

            if (choice < 1 || choice > 2) {
                printf("\nError: invalid option - input must be between 1 or 2.\n");
            }
        } while (choice < 1 || choice > 2);

        printf("Please enter a date.\n");
        scanf("%s", date);

        if (choice == 1) {
            char equation[MAX_STRING_SIZE] = "Standard";
            printTableHead(date, equation, location, numLocations);

            for (int i = 0; i < numLocations; i++) {
                for (int k = 0; k < locations[i].numDays; k++) {
                    if (strcmp(date, locations[i].dates[k]) == 0) {
                        printTableRow(locations[i].locationName, locations[i].standardAQHIs[k]);
                        check = 1;
                    }
                }
            }

            if (check == 0) {
                printf("%s not found.\n", date);
            }
        }
        else {
            char equation[MAX_STRING_SIZE] = "Seasonal";
            printTableHead(date, equation, location, numLocations);

            for (int i = 0; i < numLocations; i++) {
                for (int k = 0; k < locations[i].numDays; k++) {
                    if (strcmp(date, locations[i].dates[k]) == 0) {
                        printTableRow(locations[i].locationName, locations[i].seasonalAQHIs[k]);
                        check = 1;
                    }
                }
            }

            if (check == 0) {
                printf("%s not found.\n", date);
            }
        }
        printTableFoot();
    }
    else {
        printf("No location data has been entered. Please enter new location and data and try again.\n");
    }
}

void printTableByLocation(Location locations[], int numLocations) {
    if (numLocations != 0) {
        int choice;
        int check = 0;
        char location[MAX_STRING_SIZE];
        char date[MAX_STRING_SIZE] = "Date";

        do {
            printf("\nChoose your AQHI settings:\n");
            printf("\t1. Calculate Standard Conditions\n");
            printf("\t2. Calculate Seasonal Conditions\n");

            scanf("%d", &choice);

            if (choice < 1 || choice > 2) {
                printf("\nError: invalid option - input must be between 1 and 4.\n");
            }
        } while (choice < 1 || choice > 2);
        
        printf("Please enter the name of a location.\n");
        scanf("%s", location);

        if (choice == 1) {
            char equation[MAX_STRING_SIZE] = "Standard";
            printTableHead(location, equation, date, numLocations);

            for (int i = 0; i < numLocations; i++) {
                if (strcmp(location, locations[i].locationName) == 0) {
                    for (int j = 0; j < (sizeof(locations[i].dates) / sizeof(locations[i].dates[0])); j++) {
                        printTableRow(locations[i].dates[j], locations[i].standardAQHIs[j]);
                    }
                    check = 1;
                }
            }

            if (check == 0) {
                printf("%s not found.\n", location);
            }
        }
        else {
            char equation[MAX_STRING_SIZE] = "Seasonal";
            printTableHead(location, equation, date, numLocations);

            for (int i = 0; i < numLocations; i++) {
                if (strcmp(location, locations[i].locationName) == 0) {
                    for (int j = 0; j < (sizeof(locations[i].dates) / sizeof(locations[i].dates[0])); j++) {
                        printTableRow(locations[i].dates[j], locations[i].seasonalAQHIs[j]);
                    }
                    check = 1;
                }
            }

            if (check == 0) {
                printf("%s not found.\n", location);
            }
        }
        printTableFoot();
    }
    else {
        printf("No location data has been entered. Please enter new location and data and try again.\n");
    }
}

// ################# DO NOT MODIFY ANYTHING BELOW HERE  ####################
int printMenuGetSelection(){
    int choice=-1;
   
    do{
        printf("\nSelect an option from the Menu:\n");
        printf("\t1. Calculate AQHI for Location\n");
        printf("\t2. Calculate AQHI for Date\n");
        printf("\t3. Read Data From a File\n");
        printf("\t4. Exit\n");

        scanf("%d", &choice);

        if (choice < 1 ||  choice > 4) {
            printf("\nError: invalid option - input must be between 1 and 4.\n");
        }
    } while(choice < 1 ||  choice > 4);
    if (choice == 4){
        return -1;
    }
    
    return choice;
}

void printTableHead(char label[MAX_STRING_SIZE], char equationName[MAX_STRING_SIZE], char col1Name[MAX_STRING_SIZE], int numLocations){
    if(numLocations ==0){
        printf("\nNo location data has been entered. Please enter new location and data and try again. \n");
        return;
    }
    
    printf("\nAQHI for %s conditions at %s:", equationName, label);
    printf("\n---------------------------------------------------------------------------------------------------\n");
    printf("| %-10.10s| %-10s| %-12s| %-58s|\n", col1Name, "AQHI", "Health Risk", "Health Message");
   
    printf("---------------------------------------------------------------------------------------------------\n");
}

void printTableRow(char label[MAX_STRING_SIZE], float aqhi){
    char healthRisk;

    if (aqhi < 4) {
        healthRisk = 'L';
    }
    else if (aqhi < 7) {
        healthRisk = 'M';
    }
    else if (aqhi < 11) {
        healthRisk = 'H';
    }
    else {
        healthRisk = 'V';
    }

    if(aqhi <= 10){
        printf("| %-10.10s| %-10.2f| ", label,  aqhi);
    }
    else{
        printf("| %-10.10s| %-10s| ", label,  "Above 10");
    }
    
    switch (healthRisk) {
        case 'L':
            printf("%-12s|", "Low");
            printf(" %-58s|\n", "Ideal air quality for outdoor activities.");
            break;
        case 'M':
            printf("%-12s|", "Moderate");
            printf(" %-58s|\n", "No need to modify your outdoor activities.");
            break;
        case 'H':
            printf("%-12s|", "High");
            printf(" %-58s|\n", "Consider reducing or rescheduling activities outdoors.");
            break;
        case 'V':
            printf("%-12s|", "Very High");
            printf(" %-58s|\n", "Reduce or reschedule strenuous activities outdoors.");
            break;
    } 
}

void printTableFoot(){
    printf("---------------------------------------------------------------------------------------------------\n");
}

float roundAQHI(float aqhi){
    if (aqhi < 0.5) 
        return 1;
    return round(aqhi);
}

float standardAQHI(float O3, float NO2, float PM2_5){
    return (1000/10.4) * ( (exp(0.000537*O3)-1) + (exp(0.000871*NO2)-1) + (exp(0.000487*PM2_5)-1) );
}

float coolAQHI(float NO2, float PM2_5){
    return  (1000/6.43) * ( (exp(0.000457*NO2)-1) + (exp(0.000462*PM2_5)-1) );
}

float warmAQHI(float O3, float NO2, float PM2_5){
    return  (1000/12.8) * ( (exp(0.00104*O3)-1) + (exp(0.00101*NO2)-1) + (exp(0.000621*PM2_5)-1) );
}
