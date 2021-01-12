// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for movie information */
struct movie
{
    char* title;
    char* year;
    char* language;
    char* rating;
    struct movie* next;
};

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie* createMovie(char* currLine)
{
    struct movie* currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char* saveptr;

    // The first token is the Title
    char* token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->year, token);

    // The next token is the language
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->language = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->language, token);

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->rating, token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie* processFile(char* filePath)
{
    // Open the specified file for reading only
    FILE* movieFile = fopen(filePath, "r");

    char* currLine = NULL;
    size_t len = 0;
    size_t nread;
    char* token;
    int tally = 0;

    // The head of the linked list
    struct movie* head = NULL;
    // The tail of the linked list
    struct movie* tail = NULL;

    // Ignore the first line
    getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get the number of lines in file
        tally++;

        // Get a new movie node corresponding to the current line
        struct movie* newNode = createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    printf("Processed file %s and parsed data for %d movies\n", filePath, tally);
    return head;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* aMovie) {
    printf("%s, %s %s, %s\n", aMovie->title,
        aMovie->language,
        aMovie->year,
        aMovie->rating);
}
/*
* Print the linked list of movies
*/
void printMovieList(struct movie* list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/* Present Choices to User*/
void choices(void)
{
    printf("\n");
    printf("1. Show Movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n");
    printf("\n");
}

/* Process user's choice input */
int processChoiceInput(int userChoice)
{
    // when user's input is invalid
    if (userChoice < 1 || userChoice > 4)
    {
        printf("You entered an incorrect choice. Try again\n");
        return 0;
    }
    return userChoice;
}

/* Get user's Input*/
int getChoiceInput(void)
{
    int numInput = 0;
    printf("Enter a choice from 1 to 4: ");
    scanf("%d", &numInput);
    return processChoiceInput(numInput);
}

int prompt(void)
{
    // to repeat choice prompt until valid input
    int cycle = 0;

    // Get a valid input from user
    do
    {
        // Display a menu of interactive choices to user
        choices();
        // Get and Process user's input
        cycle = getChoiceInput();
    } while (cycle == 0);

    return cycle;
}

void processYearInput(struct movie* list)
{
    //Get Year From User
    int yearInput = 0;
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &yearInput);

    // to track at least one found result
    int atLeastOne = 0;

    //Process Year Entered
    while (list != NULL)
    {
        // compare user's year input to movie's year in database
        int newYear = atoi(list->year);

        if (newYear == yearInput)
        {
            printf("%s\n", list->title);
            atLeastOne = 1;
        }

        // on to next movie in list
        list = list->next;
    }

    if (list == NULL && atLeastOne == 0)
    {
        printf("No data about movies released in the year %d\n", yearInput);
    }
}


/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info1.txt\n");
        return EXIT_FAILURE;
    }
    // Process file and print message
    struct movie* list = processFile(argv[1]);

    // Display Prompt and get input

    int choice = prompt();

    // Continue Until User wants to Exit
    while (choice != 4)
    {
        // Process Choice (1-3)
        if (choice == 1)
        {
            processYearInput(list);

        } else if (choice == 2)
        {
            printf("%d", choice);
        
        } else
        {
            printf("%d", choice);

            // Free Up Heap Memory
            
        }
        
        // Repeat Prompt After Processing
        choice = prompt();
    }

    
    //printMovieList(list);
    return EXIT_SUCCESS;
}