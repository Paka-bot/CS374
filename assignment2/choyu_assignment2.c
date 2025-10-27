#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <string.h> // Using strtok_r

// Assume that max language length is 21
#define MAX_LANGUAGE_LENGTH 21 

// Defines how data is stored in a CSV file, its format, and its structure.
struct movie {
    char *title;
    int year;
    char *languages;
    double rating;
    struct movie *next;
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
        printf("Error: Could not open file %s\n", filePath);
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


// Implement function 1
void showMoviesByYear(struct movie *list) {
    int year;
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);

    int found = 0;
    struct movie *current = list;

    //Trip to the end of the list
    while (current != NULL) {
        if (current->year == year) {
            printf("%s\n", current->title);
            found = 1;
        }
        current = current->next;
    }

    // If there is no corresponding movie
    if (found == 0) {
        printf("No data about movies released in the year %d\n", year);
    }
}

// Function2 Outputs top-rated movies by year
void showHighestRatedByYear(struct movie *list) {
    struct movie *highest_rated[2022] = {NULL}; 
    struct movie *current = list;

    // Browse the list to find the top-rated movies by year.
    while (current != NULL) {
        int year = current->year;
        
        if (highest_rated[year] == NULL || current->rating > highest_rated[year]->rating) {
            highest_rated[year] = current;
        }
        current = current->next;
    }

    // Iterate over the array from 1900 to 2010 and print the results.
    for (int year = 1900; year <= 2021; year++) {
        if (highest_rated[year] != NULL) {
            struct movie *m = highest_rated[year];
            printf("%d %.1f %s\n", m->year, m->rating, m->title); 
        }
    }
}


// Function3 Output the movie in the language provided
void showMoviesByLanguage(struct movie *list) {
    char userLang[MAX_LANGUAGE_LENGTH];
    printf("Enter the language for which you want to see movies: ");
    scanf("%20s", userLang);

    int found = 0;
    struct movie *current = list;
    
    while (current != NULL) {
        char temp_langs[1000]; 
        strcpy(temp_langs, current->languages);
        
        char *langSavePtr;
        char *langToken;
        
        // Separate language tokens using square brackets and semicolons
        langToken = strtok_r(temp_langs, "[];", &langSavePtr);
        
        while (langToken != NULL) {
            if (strcmp(langToken, userLang) == 0) { 
                printf("%d %s\n", current->year, current->title);
                found = 1;
                break;
            }
            langToken = strtok_r(NULL, "[];", &langSavePtr);
        }
        current = current->next;
    }

    if (found == 0) {
        printf("No data about movies released in %s\n", userLang);
    }
}

// Output user selection menu
void printMenu() {
    printf("\n1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n\n");
    printf("Enter a choice from 1 to 4: ");
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

//main function
int main ( int argc, char **argv ){
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies.csv\n");
        return EXIT_FAILURE;
    }

    int movie_count = 0;
    struct movie *list = processMovieFile(argv[1], &movie_count);
    
    if (list == NULL) return EXIT_FAILURE;

    printf("Processed file %s and parsed data for %d movies\n", argv[1], movie_count);

    int choice = 0;
    while (choice != 4) {
        printMenu();
        
        int num_scanned = scanf("%d", &choice);
        while (getchar() != '\n');
        
        if (num_scanned != 1) choice = -1;

        if (choice == 1) showMoviesByYear(list);
        else if (choice == 2) showHighestRatedByYear(list);
        else if (choice == 3) showMoviesByLanguage(list);
        else if (choice == 4) break;
        else printf("You entered an incorrect choice. Try again.\n");
    }

    freeMovieList(list);
    return EXIT_SUCCESS;
}