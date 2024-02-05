/*
 * This file contains the headers for required functions, structure, and 
 * some useful constants.
 *
 * Look at the section with the comment "FUNCTIONS YOU MUST CREATE", 
 * you will need to create the function defintions for those functions.
 *
 * You may also use othe functions that you can add to the section with 
 * the comment "ADD OPTIONAL EXTRA FUNCTIONS HERE"
 *
 * If you modify the existing function protoypes, arguments, structure, 
 * defines, etc found below the comment "DO NOT MODIFY ANYTHING BELOW HERE"
 * you will lose marks.
*/

#ifndef A3_FUNCTIONS_H_
#define A3_FUNCTIONS_H_

// #################  ADD OPTIONAL EXTRA FUNCTIONS HERE  ####################

void checkForFile(char filename[]);


// ################# DO NOT MODIFY ANYTHING BELOW HERE  ####################

// Use these symbolic constants for array sizes
#define MAX_LOCATIONS 10 
#define MAX_STRING_SIZE 128
#define MAX_DATES 31
#define HOURS 24 

#define O3_INDEX 0
#define NO2_INDEX 1
#define PM2_5_INDEX 2

typedef struct Location{ 
    char locationName[MAX_STRING_SIZE]; 
    char dates[MAX_DATES][MAX_STRING_SIZE];
    int numDays; 
    
    float pollutantAverages[3][MAX_DATES];  

    float standardAQHIs[MAX_DATES]; 
    float seasonalAQHIs[MAX_DATES]; 

} Location; 


// ################# FUNCTIONS YOU MUST CREATE  ####################

float getAverage(float pollutants[24]); 
int readAPollutantFile(char filename[], char location[], float pollutantAverages[], char dates[][MAX_STRING_SIZE]); 
struct Location readData(); 

void printTableByDate(Location locations[], int numLocations);
void printTableByLocation(Location locations[], int numLocations);


// ################# FUNCTIONS ALREADY PROVIDED  ####################

// FILES
int readLine(char line[], int size, FILE* fp);
int splitLineIntoStrings(char line[], char words[][MAX_STRING_SIZE], char seperators[]);

// Input/Output Functions
int printMenuGetSelection();
void printTableHead(char label[MAX_STRING_SIZE], char equationName[MAX_STRING_SIZE], char col1Name[MAX_STRING_SIZE], int numLocations);
void printTableRow(char label[MAX_STRING_SIZE], float aqhi);
void printTableFoot();

// Math Functions
float roundAQHI(float aqhi);
float standardAQHI(float O3, float NO2, float PM2_5);
float coolAQHI(float NO2, float PM2_5);
float warmAQHI(float O3, float NO2, float PM2);

#endif