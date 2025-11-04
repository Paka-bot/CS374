#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE, system()
#include <string.h> // Using strtok_r
#include <dirent.h> // For directory operations
#include <time.h> // using random seed based on time

// Assume that max filename length is 256
#define MAX_FILENAME_LENGTH 256
// For the creation name of the directory
#define ONID "choyu" 

// Defines how data is stored in a CSV file, its format, and its structure.
struct movie {
    char *title;
    int year;
    char *languages;
    double rating;
    struct movie *next;
};

// Linked list structure for movie titles to be stored in files by year
struct titleNode {
    char *title;
    struct titleNode *next;
};

// Receive each line of the file in csv, create a movie structure, 
// and return a pointer to the next line.
struct movie *createMovie(char *currLine) {
    //Dynamically allocating memory for movie structures
    struct movie *currMovie = malloc(sizeof(struct movie));
    if (currMovie == NULL) {
        perror("Failed to allocate memory for movie struct");
        exit(EXIT_FAILURE);
    }

    char *saveptr;

    // Title parsing
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = malloc(strlen(token) + 1);
    strcpy(currMovie->title, token);

    // Year Parsing
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    // Language parsing
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = malloc(strlen(token) + 1);
    strcpy(currMovie->languages, token);

    // Rating parsing
    token = strtok_r(NULL, "\n", &saveptr); 
    currMovie->rating = strtod(token, NULL);

    // Initialize pointer
    currMovie->next = NULL;

    return currMovie;
}

// Open the file, read each line, convert it to a structure, 
// and create a linked list with the structures.
struct movie *processMovieFile(char* filePath, int *movie_count) {
    // Internal functions for getline
    char *currLine = NULL;
    size_t len = 0;

    // pointers to track the head and tail of a linked list
    struct movie *head = NULL;
    struct movie *tail = NULL;

    FILE *movieFile = fopen(filePath, "r");
    if (movieFile == NULL) { // If an invalid file name is provided, an error message is displayed.
        printf("Error: Could not open files %s\n", filePath); 
        return NULL; 
    }

    // The header of the CSV is read and not saved as data.
    getline(&currLine, &len, movieFile);

    // Read the file line by line
    while (getline(&currLine, &len, movieFile) != -1)
    {
        // Create a new movie node in the linked list using data from currLine.
        struct movie *newNode = createMovie(currLine);

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        (*movie_count)++;
    }

    // Free the memory allocated by getline for currLine
    free(currLine);
    // Close the file
    fclose(movieFile);
    return head;
}


// Free dynamic memory of list when program ends
void freeMovieList(struct movie *list) {
    struct movie *temp;
    while (list != NULL) {
        temp = list;
        list = list->next;

        // Free the dynamically allocated memory inside the 
        // structure first, then free the structure memory.
        free(temp->title);
        free(temp->languages);
        free(temp);
    }
}

// Find the largest or smallest file whose name starts with movies and ends with .csv.
int find_target_files(char *largest_file, char *smallest_file) {
    DIR *d;
    struct dirent *dir;
    long max_size = -1;
    long min_size = -1;
    int found = 0;

    largest_file[0] = '\0';
    smallest_file[0] = '\0';

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // Make sure the file name starts with movies and ends with .csv
            if (strncmp(dir->d_name, "movies_", 7) == 0 && 
                strlen(dir->d_name) > 4 && 
                strcmp(dir->d_name + strlen(dir->d_name) - 4, ".csv") == 0) {
                
                FILE *f = fopen(dir->d_name, "r");
                if (f == NULL) {
                    continue;
                }
                // Move pointer to the end of file
                fseek(f, 0, SEEK_END);
                long current_size = ftell(f);
                fclose(f);
                
                // Check for largest
                if (current_size > max_size) {
                    max_size = current_size;
                    strcpy(largest_file, dir->d_name);
                }
                // Check for smallest
                // If min_size is -1 or smaller than the current size, update
                if (min_size == -1 || current_size < min_size) {
                    min_size = current_size;
                    strcpy(smallest_file, dir->d_name);
                }
                found = 1;
            }
        }
        closedir(d);
    }
    return found;
}

// Process the found function and create a new directory
void process_and_write_files(struct movie *list, const char *onid) {
    // create new directory
    int randNum = random() % 100000;
    char dirName[256];
    sprintf(dirName, "%s.movies.%d", onid, randNum);

    char command[512];
    sprintf(command, "mkdir %s", dirName);
    if (system(command) != 0) {
        perror("Failed to create directory");
        return;
    }
    printf("Created directory with name %s\n", dirName);

    // Data organization by year
    struct titleNode *yearLists[2022] = {NULL};
    struct movie *current = list;

    while (current != NULL) {
        int year = current->year;
        if (year >= 1900 && year <= 2021) {
            struct titleNode *newTitle = malloc(sizeof(struct titleNode));
            newTitle->title = malloc(strlen(current->title) + 1);
            strcpy(newTitle->title, current->title);
            
            newTitle->next = yearLists[year];
            yearLists[year] = newTitle;
        }
        current = current->next;
    }

    // Create files by year and insert data
    for (int year = 1900; year <= 2021; year++) {
        if (yearLists[year] != NULL) {
            char filePath[512];
            sprintf(filePath, "%s/%d.txt", dirName, year);

            FILE *outFile = fopen(filePath, "w");
            if (outFile == NULL) {
                perror("Failed to create year file");
                continue;
            }

            // Loop through the title list and put it into a file
            struct titleNode *currentTitle = yearLists[year];
            while (currentTitle != NULL) {
                fprintf(outFile, "%s\n", currentTitle->title);
                currentTitle = currentTitle->next;
            }
            fclose(outFile);
        }
    }

    // Release memory for title list by year
    for (int year = 1900; year <= 2021; year++) {
        struct titleNode *currentTitle = yearLists[year];
        while (currentTitle != NULL) {
            struct titleNode *temp = currentTitle;
            currentTitle = currentTitle->next;
            free(temp->title);
            free(temp);
        }
    }
}

// Handle the menu shown to the user
int handle_file_selection(char *selected_filename) {
    char largest_file[MAX_FILENAME_LENGTH];
    char smallest_file[MAX_FILENAME_LENGTH];
    int file_choice = 0;
    
    int files_found; 

    while (1) {
        files_found = find_target_files(largest_file, smallest_file);

        printf("\nWhich file you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");
        printf("Enter a choice from 1 to 3: ");

        if (scanf("%d", &file_choice) != 1) {
            while (getchar() != '\n');
            printf("You entered an incorrect choice. Try again.\n");
            continue;
        }
        while (getchar() != '\n');

        if (file_choice == 1) {
            if (files_found) {
                strcpy(selected_filename, largest_file);
                return 1;
            } else {
                printf("No file with prefix 'movies_' and extension '.csv' found. Try again\n");
            }
        } else if (file_choice == 2) {
            if (files_found) {
                strcpy(selected_filename, smallest_file);
                return 1;
            } else {
                printf("No file with prefix 'movies_' and extension '.csv' found. Try again\n");
            }
        } else if (file_choice == 3) {
            printf("Enter the complete file name: ");
            scanf("%255s", selected_filename);
            while (getchar() != '\n');

            FILE *f = fopen(selected_filename, "r");
            if (f != NULL) {
                fclose(f);
                return 1;
            } else {
                // display error message if there is no such file, and back to menu
                printf("The file %s was not found. Try again\n", selected_filename);
            }
        } else {
            printf("You entered an incorrect choice. Try again.\n");
        }
    }
}


//main function
int main ( int argc, char **argv ){
    int top_level_choice = 0;
    
    // Set the random seed only once when running the program
    srandom(time(NULL)); 

    while (top_level_choice != 2) {
        printf("\n1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2: ");

        if (scanf("%d", &top_level_choice) != 1) {
            while (getchar() != '\n');
            printf("You entered an incorrect choice. Try again.\n");
            continue;
        }
        while (getchar() != '\n');

        if (top_level_choice == 1) {
            char selected_filename[MAX_FILENAME_LENGTH] = "";
            
            // Handling the file selection menu
            if (handle_file_selection(selected_filename)) {
                
                int movie_count = 0;
                // Try to process the file
                struct movie *list = processMovieFile(selected_filename, &movie_count);

                if (list != NULL) {
                    // Success: Print required messages
                    printf("Now processing the chosen file named %s\n", selected_filename);
                    
                    process_and_write_files(list, ONID); 
                    
                    freeMovieList(list);

                } 
            }
        } else if (top_level_choice == 2) {
            break;
        } else {
            printf("You entered an incorrect choice. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}