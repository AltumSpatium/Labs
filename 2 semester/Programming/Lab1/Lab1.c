/**
 * The input is some set of numbers.
 * Program will display the average,
 * maximum and minimum values of the set after each number.
 */

#include "stdio.h"

int main() {
    double current_number;
    double max;
    double min;
    double average;
    double counter = 1;
    double sum = 0;

    printf("Enter some numbers, any other symbol to exit\n");
    while(scanf("%lf", &current_number)) {
        if (counter == 1) {
            max = current_number;
            min = current_number;
        }
        if (current_number < min){
            min = current_number;
        } 
        if (current_number > max) {
            max = current_number;
        }
        sum += current_number;
        average = sum / counter;
        printf("max = %.3f, min = %.3f, average = %.3f \n", max, min, average);
        counter++;
    }
    return 0;
}
