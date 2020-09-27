/*
    Author: Jeremy Kimotho
    Date: 27/09/2020
    Description:
    An application that works like a simple search engine. It displays a table
    and you enter the index and number of results desired.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int randomNum(int m, int n){
    /* 
        The function takes an upper bound m and lower bound n and returns a random value between them. The rand function is called to generate a random number, the number is then checked to see if it fits between the bounds. If it does it's returned, if it doesn't another is generated and so on.
    */
    int random = rand();
    while(random<m || random>n){
        random = rand() % 10;
    }
    return random;
}

void initialize(int (*a)[20], int r, int c){
    /* 
        The function takes a pointer to an array as input as well as the row and column number and will populate the array with random integers. The randomNum function is called to produce an integer and it then assigns every value in the array a random integer.
    */
    srand(time(NULL));
    for (int i = 0; i < r; i++)
    {
        for (int j = 0;j<c;j++)
        {
            int random = randomNum(0,9);
            a[i][j] = random;
        }
    }
}

void copyTable(int (*a)[20], int (*b)[20], int r, int c){
    /*  
        The function copies table before it gets mutated by the sorting of the topDocument so it can get logged. The two tables takes as input are the 
        table intended to be copied first and the table to be copied into is second. They're both pointers to an array. The row and column numbers are used in the duplication loops.
    */
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            b[i][j] = a[i][j];
        }
    }
}

int display(int (*a)[20], int r, int c){
    /* 
        The function displays a 2d array given the row and column numbers. Anytime a table in the program needs to be printed to the console this is the function invoked. 
    */
    for (int i = 0; i < r; i++)
    {
        for (int j = 0;j<c;j++)
        {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    return 0;
}

void logToFile(int (*a)[20], int (*b)[20], int indexOfWord, int topDocNumber, FILE *logFile, int r, int c){
    /* 
        The function logs a table along with the index of the desired word and the number of search results requested. The pointers a and b represent the table initially and the search result in that order. The file logfile is the file to write to. The first for loop writes the initial table and the second for loop writes the search result table.  
    */
    fprintf(logFile, "The table:\n");
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            fprintf(logFile, "%d", a[i][j]);
        }
        fprintf(logFile, "\n");
    }
    fprintf(logFile, "\n");
    fprintf(logFile, "The user searched for index %d\n", indexOfWord);
    fprintf(logFile, "The user searched for %d top documents\n", topDocNumber);
    fprintf(logFile, "\n");
    fprintf(logFile, "The search result was:\n");
    for (int i = 0; i < topDocNumber; i++)
    {
        fprintf(logFile, "%d ", b[i][0]);
        fprintf(logFile, "\n");
    }
    fprintf(logFile, "\n");
}

void swap(int (*a)[20], int (*b)[20], int row_i, int row_ii, int column, int row){
    /*
        This function worked in tandem with the topRelevantDocs function to sort the table. What this function does is swap rows in a 2d array once the topRelevantDocs function has determined they need to be swapped.  The reason there are two arrays being swapped is because one is an array for the search results and the other is the original array. We determine what rows need to be swapped using the original array and then swap both because the search results array is what we will display to the user later. 
    */
    for(int i = 0;i<column;i++){
        int intermediate = a[row_i][i];
        a[row_i][i]=a[row_ii][i];
        a[row_ii][i]=intermediate;
        intermediate = b[row_i][0];
        b[row_i][0] = b[row_ii][0];
        b[row_ii][0] = intermediate;
    }
}

int topRelevantDocs(int (*a)[20], int  (*b)[20], int n, int r, int c,  int d, FILE* logFile){
    /*
        This function worked in tandem with the swap function to sort the table. What this function does is check if one values frequency is larger than another value and decide if the rows need to be swapped or not. If it determines they do it passes that task to the swap function. It determines the frequency using the rowTotals array. Before the comparison is made the double values are generated for comparison. It takes as input two 2d arrays as pointers which are the original array generated with random or file values and the other is the one we will log to the log file. The int n is the number of search results we want, the int d is the index we're searching for and logfile is the file we'll log to. int r and c are row and column. After every document search we log to the file. This is the only place logging is done. 
    */
    int rowTotals[20][20];
    int tableDuplicate[20][20];
    for (int i = 0; i < r; i++)
    {
        tableDuplicate[i][0] = i;
    }
    for (int i = 0; i < r; i++){
        rowTotals[i][0] = 0;
    }
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            rowTotals[i][0]+=a[i][j];
        }
    }

    for(int counter = 0;counter < r-1; counter++){
        for (int i = 0; i < r - 1; i++)
        {
            for (int j = 0; j < r - i - 1; j++)
            {
                double double_1 = (double)(a[i][d]) / (double)(rowTotals[i][0]);
                double double_2 = (double)(a[i+1][d]) / (double)(rowTotals[i+1][0]);
                if (double_1<double_2)
                {
                    swap(a, tableDuplicate, i, i + 1, c, r);
                }
            }
        }
    }
    printf("Sorting completed\n");

    copyTable(b, a, r, c);
    
    display(tableDuplicate, n, 1);
    logToFile(a, tableDuplicate, d, n, logFile, r, c);
    return 0;
}

void fileInitialize(int (*a)[20], int c, char file[100], int r){
    /*
        This function populates a 2d array if the user provided a file. It takes the table to be populated in the form of a pointer to a 2d array, the number of columns and rows and a filename. It whilst assigning values to the array will check if the column number is correct and if the individual values are correct. It then once populated will check if the row number is correct. 
    */
    FILE *fptr;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fptr = fopen(file, "r");
    if (fptr == NULL)
    {
        printf("Error opening file %s\n", file);
        exit(1);
    }

    int j = 0;
    while ((read = getline(&line, &len, fptr)) != -1)
    {
        int k = 0;
        for (int i = 0; i < (c * 2) - 1; i += 2)
        {
            if(line[i] - 48<0||line[i]-48>9){
                printf("There was a problem with the provided file. The two possibilities are the entered column value in the command line is not equal to the column value in the file you provided. Or the file you provided contains values smaller than 0 or larger than 9\n");
                exit(1);
            }else{
                a[j][k] = line[i] - 48;
                k++;
            }
        }
        j++;
    }
    fclose(fptr);
    if(r!=j){
        printf("There was a problem with the provided file. The row value provided in the command line arguments is not equal to the number of rows in the text document\n. Expected %d got %d\n", r, j);
        exit(1);
    }
    
}

int main(int argc, char *argv[]){

    if (argc == 3){
        int row = atoi(argv[1]);
        int column = atoi(argv[2]);

        if(row>20||column>20||row<1||column<1){
            printf("You have entered a row or column value smaller than 1 or larger than 20\n");
            exit(1);
        }

        printf("You have not provided a filename therefore a random table will be generated.\n");

        /* 
            The next few lines check if a log file already exists, if it does it deletes the contents if it doesn't it creates a log file
        */
        FILE *log;
        if((log = fopen("assign1.log", "r"))!=NULL){
            fclose(log);
            log = fopen("assign1.log", "w");
            fclose(log);
            log = fopen("assign1.log", "a");
        }else{
            log = fopen("assign1.log", "a+");
        }

        int indexOfWord;
        char searchAgain[100];
        int topDocumentsNumber;

        int table[20][20];
        int initialTable[20][20];

        initialize(table, row, column);

        copyTable(table, initialTable, row, column);

        do
        {
            display(table, row, column);
            printf("Enter the index of the word you are searching for\n");
            scanf("%d", &indexOfWord);
            if(indexOfWord>column-1){
                printf("You've entered an index outside of the range! Please enter a value between 0 and %d\n", column-1);
                exit(1);
            }
            printf("How many top documents do you want to retrieve?\n");
            scanf("%d", &topDocumentsNumber);
            if (topDocumentsNumber > row)
            {
                printf("You've entered an index outside of the range! Please enter a value between 0 and %d\n", row);
                exit(1);
            }

            topRelevantDocs(table, initialTable, topDocumentsNumber, row, column, indexOfWord, log);

            printf("Would you like to search again? (y/n)\n");
            scanf("%s", searchAgain);
        } while (strcmp(searchAgain, "y") == 0);

        fclose(log);
        printf("End\n");
    }
    else if (argc == 4)
    {

        int row = atoi(argv[1]);
        int column = atoi(argv[2]);

        if (row > 20 || column > 20 || row < 1 || column < 1)
        {
            printf("You have entered a row or column value smaller than 1 or larger than 20\n");
            exit(1);
        }

        printf("You have provided a filename %s therefore the table will be generated from the file\n", argv[3]);

        FILE *log;
        log = fopen("assign1.log", "a+");
        int table[20][20];
        int initialTable[20][20];
        char f[100];
        char searchAgain[100];
        strcpy(f, argv[3]);
        int indexOfWord;
        int topDocumentsNumber;

        fileInitialize(table, column, f, row);

        copyTable(table, initialTable, row, column);

        do{
            display(table, row, column);
            printf("Enter the index of the word you are searching for\n");
            scanf("%d", &indexOfWord);
            if (indexOfWord > column - 1)
            {
                printf("You've entered an index outside of the range! Please enter a value between 0 and %d\n", column - 1);
                exit(1);
            }
            printf("How many top documents do you want to retrieve?\n");
            scanf("%d", &topDocumentsNumber);
            if (topDocumentsNumber > row)
            {
                printf("You've entered a document value outside of the range! Please enter a value between 0 and %d\n", row);
                exit(1);
            }

            topRelevantDocs(table, initialTable, topDocumentsNumber, row, column, indexOfWord, log);

            printf("Would you like to search again? (y/n)\n");
            scanf("%s", searchAgain);
        } while (strcmp(searchAgain, "y") == 0 || strcmp(searchAgain, "Y") == 0);

        fclose(log);
        printf("End\n");
    }else{
        printf("You must enter input to the command line in the format:\nprogram_name M N filename(optional)\n");
    }
    return 0;
}