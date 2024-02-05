#include <stdio.h>
#include <math.h>
#include <string.h>
#include "A3_functions.h"

int main(){
    int choice = 0;
    int numLocations = 0;
    Location locations[MAX_LOCATIONS];
    
    printf("Welcome to the Air Quality Index Generator!\n");

    do {
        choice = printMenuGetSelection();

        if (choice == 1) {
            printTableByLocation(locations, numLocations);
        }
        if (choice == 2) {
            printTableByDate(locations, numLocations);
        }
        if (choice == 3) {
            if (numLocations <= 10) {
                locations[numLocations] = readData();
                numLocations++;
            }
            else {
                printf("Maximum locations have been added\n");
            }
        }
        if (choice == -1) {
            break;
        }
    } while (1);
        
    return 0;
}