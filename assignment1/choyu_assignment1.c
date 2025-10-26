#include <stdio.h>
#include <math.h>

// declare pi as a constant
const double pi = 3.14159265359;

/*
This program first prompts the user to enter a natural number between 2 and 10. 
The entered natural number becomes the number of spherical segments. 
It then prompts the user to enter the sphere's radius, ha, and hb. 
The value of ha must always be greater than or equal to hb. 
The final height h of the spherical segments is calculated as h = ha - hb. 
If the user does not enter a valid value, the program repeats until a valid value is entered.
*/

int main() {
    
    // Variable counting the number of valid data sets
    int valid_sets = 0;

    // Declare variables to be input by the user
    int n = 0;
    double R, ha, hb;

    // Variables to store the calculated values of the variables entered by the user
    double a, b, h;

    // Variables to store the calculation results for each spherical segment
    double top_surface_area, bottom_surface_area, lateral_surface_area;
    double total_surface_area, volume;

    // Variables to display the average value at the end of the loop
    double total_area = 0;
    double total_volume = 0;

    // Requesting the user to input the number of sphere segments
    do {
        printf("How many spherical segments you want to evaluate [2-10]?\n");
        scanf("%d", &n);
    } while (n < 2 || n > 10);

    // Performed calculations n times
    while (valid_sets < n) {
        printf("Obtaining data for spherical segment number %d\n", valid_sets + 1);

        // Obtain values from the user to use in calculations
        printf("What is the radius of the sphere (R)?\n");
        scanf("%lf", &R);
        printf("What is the height of the top area of the spherical segment (ha)?\n");
        scanf("%lf", &ha);
        printf("What is the height of the bottom area of the spherical segment (hb)?\n");
        scanf("%lf", &hb);

        // Display the input value
        printf("Entered data: R=%.2f ha=%.2f hb=%.2f\n", R, ha, hb);

        // Validate the input value.If valid, proceed with the calculation; otherwise, prompt for re-entry.
        if (R > 0 && ha > 0 && hb > 0 && ha <= R && hb <= R && ha >= hb) {
            
            a = sqrt(R * R - ha * ha);
            b = sqrt(R * R - hb * hb);
            h = ha - hb; 

            // Calculate area and volume using formulas
            top_surface_area = pi * a * a;
            bottom_surface_area = pi * b * b;  
            lateral_surface_area = 2 * pi * R * h;   
            total_surface_area = top_surface_area + bottom_surface_area + lateral_surface_area;
            volume = (1.0 / 6.0) * pi * h * (3 * a * a + 3 * b * b + h * h);

            // Display calcualtion results
            printf("Total Surface Area = %.2f Volume = %.2f\n", total_surface_area, volume);

            // To display the average in the final loop, 
            // add the volume and surface area values of this sphere segment 
            //to the total volume and total surface area.
            total_area += total_surface_area;
            total_volume += volume;

            // Increment the valid_set value by 1 to count the number of calculations.
            valid_sets++;
        } else {
            // If an invalid value is entered, display an error message and proceed to re-entry. 
            printf("Invalid Input.\n");
        }
    }

    // After completing all calculations, 
    // display the average of the values calculated so far in the final loop. 
    if (n > 0) {
        double average_surface_area = total_area / n;
        double average_volume = total_volume / n;

        printf("Total average results:\n");
        printf("Average Surface Area = %.2f Average Volume = %.2f\n", average_surface_area, average_volume);
    }

    return 0;
}