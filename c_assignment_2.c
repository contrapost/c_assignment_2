#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

#define LENGTH_OF_FILENAME 256
#define DEFAUL_SIZE_OF_PART 30

int main(int argc, char* argv[]) {
    
    // Checks if there is no arguments (dirname isn't mentioned)
    if(argc < 2)
    {
        printf("%s\n%s\n", "You forgot to write a name of directory as argument.",
               "NB! Please enter the hole path to the directory.");
        return -1;
    }
    
    // ================= Store file names from directory in the array ======================
    
    DIR *dir;
    struct dirent *ent;
    char **fileNames;
    int sizeOfListWithFileNames = 6;
    int numberOfFileNames = 0;
    
    // Allocate memory for array of file names
    fileNames = malloc(sizeOfListWithFileNames * sizeof(char*));
    
    // Allocate memory for file names
    for(int i = 0; i < sizeOfListWithFileNames; i++)
    {
        fileNames[i]=malloc(LENGTH_OF_FILENAME * sizeof(char));
    }
    
    if ((dir = opendir (argv[1])) != NULL)
    {
        // print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL)
        {
            // get only .txt files
            if(strstr(ent->d_name, ".txt") != NULL)
            {
                if (sizeOfListWithFileNames > numberOfFileNames)
                {
                    strcpy(fileNames[numberOfFileNames++], ent->d_name);
                }
                else
                {
                    int sizeOfPrevious = sizeOfListWithFileNames;
                    
                    // If the number of strings is equal to the length of the array,
                    // the program doubles the size of the array.
                    sizeOfListWithFileNames *= 2;
                    
                    // Reallocates memory for the enlarged array.
                    fileNames = realloc(fileNames, sizeof(char*)*(sizeOfListWithFileNames));
                    if (fileNames == NULL) {
                        printf("Can't allocate memory\n");
                        return -1;
                    }
                    
                    // (Re)allocates memory for strings.
                    for(int i = 0; i < sizeOfListWithFileNames; i++)
                    {
                        // Reallocates memory for the existing strings.
                        if(i < sizeOfPrevious)
                        {
                            fileNames[i]=realloc(fileNames[i], LENGTH_OF_FILENAME * sizeof(char));
                            if (fileNames[i] == NULL)
                            {
                                printf("Can't allocate memory\n");
                                return -1;
                            }
                        }
                        //Allocates memory for new strings.
                        else
                        {
                            fileNames[i]=malloc(LENGTH_OF_FILENAME * sizeof(char));
                        }
                    }
                    // Copies the last line that was read from file to the first empty
                    // index in the array.
                    strcpy(fileNames[numberOfFileNames++], ent->d_name);
                }
            }
        }
        closedir (dir);
    }
    else
    {
        perror ("Couldn't open the directory. Please enter the hole path to the directory.");
        return EXIT_FAILURE;
    }
    
    // ============ Get number of columns and rows =====================
    
    int numberOfRows, numberOfColumns;
    char* p = fileNames[numberOfFileNames - 1];
    
    int first = 0;
    
    while (*p)
    { // While there are more characters to process...
        
        if (isdigit(*p))
        { // Upon finding a digit, ...
            long val = strtol(p, &p, 10); // Read a number, ...
            if(first == 0)
            {
                numberOfColumns = val;
                first++;
            }
            else if (first == 1)
            {
                numberOfRows = val;
                first++;
            }
        }
        else
        { // Otherwise, move on to the next character.
            p++;
        }
    }
    
    // Check if the file name contains numbers of both columns and rows.
    if (first <= 1)
    {
        printf("Wrong format of file name. Cannot detect the position of the fragment\n");
        return -1;
    }
    
    printf("Number of col: %d, number of rows: %d\n", numberOfColumns, numberOfRows);
    
    //char** part;
    //part = malloc(sizeof(char*) * DEFAUL_SIZE_OF_PART);
    
    //for(int i = 0; i < DEFAUL_SIZE_OF_PART; i++)
    //{
    //    part[i] = malloc(sizeof(char) * DEFAUL_SIZE_OF_PART);
    //}
    
    // =============== reading files from the array =======================
    
    for(int i = 0; i < numberOfFileNames; i++) {
        printf("%d. %s\n", i + 1, fileNames[i]);
    }

    // =================== freeing the memory ============================
    for(int i = 0; i < numberOfFileNames; i++) {
        free(fileNames[i]);
    }
    
    free(fileNames);
    
    return 0;
}
