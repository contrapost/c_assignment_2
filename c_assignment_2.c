#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

#define LENGTH_OF_FILENAME 256
#define DEFAUL_SIZE_OF_PART 30

char** getPart(char* fileName, char** part);

char** sort(char **array, int counter);

char** mergeParts(char** mergedPicture, char*** parts, int numberOfRows, int numberOfColumns);

int main(int argc, char* argv[]) {
    
    // Checks if there is no arguments (dirname isn't mentioned)
    if(argc < 2)
    {
        printf("%s\n%s\n", 
        		"You forgot to write a name of directory as argument.",
           	    "NB! Please enter the hole path to the directory.");
        return -1;
    }
    
    // ========== Store file names from directory in the array ================
    
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
                	strcpy(fileNames[numberOfFileNames], argv[1]);
                    strcat(fileNames[numberOfFileNames++], ent->d_name);
                }
                else
                {
                    int sizeOfPrevious = sizeOfListWithFileNames;
                    
                    // If the number of strings is equal to the length of 
                    // the array, the program doubles the size of the array.
                    sizeOfListWithFileNames *= 2;
                    
                    // Reallocates memory for the enlarged array.
                    fileNames = realloc(fileNames, 
                    			sizeof(char*)*(sizeOfListWithFileNames));
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
                            fileNames[i]=realloc(fileNames[i], 
                            			LENGTH_OF_FILENAME * sizeof(char));
                            if (fileNames[i] == NULL)
                            {
                                printf("Can't allocate memory\n");
                                return -1;
                            }
                        }
                        //Allocates memory for new strings.
                        else
                        {
                            fileNames[i] = 
                            	malloc(LENGTH_OF_FILENAME * sizeof(char));
                        }
                    }
                    // Copies the last line that was read from file to the 
                    // first empty index in the array.
                    strcpy(fileNames[numberOfFileNames], argv[1]);
                    strcat(fileNames[numberOfFileNames++], ent->d_name);
                }
            }
        }
        closedir (dir);
    }
    else
    {
        perror ("Couldn't open the directory. Please enter the hole path. ");
        return EXIT_FAILURE;
    }
    
    // ============ Get number of columns and rows =====================
    
    fileNames = sort(fileNames, numberOfFileNames);
    
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
                numberOfColumns = val + 1;
                first++;
            }
            else if (first == 1)
            {
                numberOfRows = val + 1;
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
        printf("%s %s\n", "Wrong format of file name.", 
        					"Cannot detect the position of the fragment");
        
        for(int i = 0; i < sizeOfListWithFileNames; i++) {
            free(fileNames[i]);
        }
        
        free(fileNames);
        
        return -1;
    }
    
    printf("Number of col: %d, number of rows: %d\n", 
    					numberOfColumns, numberOfRows);
    
    char*** parts;
    parts = malloc(numberOfFileNames * sizeof(char**));
    for(int i = 0; i < numberOfFileNames; i++) {
        parts[i] = malloc(DEFAUL_SIZE_OF_PART * sizeof(char*));
        for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++) {
            parts[i][j] = malloc((DEFAUL_SIZE_OF_PART + 1) * sizeof(char));
        }
    }
    
    // =============== reading files from the array =======================
    
    for(int i = 0; i < numberOfFileNames; i++)
    {
        parts[i] = getPart(fileNames[i], parts[i]);
        if (parts[i] == NULL)
        {
            for(int i = 0; i < numberOfFileNames; i++) {
                for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++) {
                    free(parts[i][j]);
                }
            }
            
            for(int i = 0; i < numberOfFileNames; i++) {
                free(parts[i]);
            }
            
            free(parts);
            
        	for(int i = 0; i < numberOfFileNames; i++) 
        	{
        		free(fileNames[i]);
    		}
    
   			free(fileNames);
        	return -1;
        }
    }
    
   
    
    char** mergedPicture;
    mergedPicture = malloc(numberOfRows * DEFAUL_SIZE_OF_PART * sizeof(char*));
    for(int i = 0; i < DEFAUL_SIZE_OF_PART * numberOfRows; i++)
    {
        mergedPicture[i] = malloc(DEFAUL_SIZE_OF_PART * numberOfColumns * sizeof(char));
    }
    
    mergedPicture = mergeParts(mergedPicture, parts, numberOfRows, numberOfColumns);
    
    // =============== DEBUGGING =======================
    
    for(int i = 0; i < numberOfFileNames; i++) {
        printf("%d. %s\n", i + 1, fileNames[i]);
    }
    
   /* int counter = 0;
    for(int i = 0; i < numberOfFileNames; i++){
        for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++){
            for(int k = 0; k < DEFAUL_SIZE_OF_PART; k++) {
                printf("%c", parts[i][j][k]);
                counter++;
                if(counter % 31 == 0) printf("\n");
            }
        }
    } */
    
    for(int i = 0; i < DEFAUL_SIZE_OF_PART * numberOfRows; i++)
    {
        printf("%s\n", mergedPicture[i]);
    }
    
    // =================== freeing the memory ============================
    
    
    for(int i = 0; i < DEFAUL_SIZE_OF_PART * numberOfRows; i++)
    {
        free(mergedPicture[i]);
    }
    
    free(mergedPicture);
    
    for(int i = 0; i < numberOfFileNames; i++) {
        for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++) {
            free(parts[i][j]);
        }
    }
    
    for(int i = 0; i < numberOfFileNames; i++) {
    	free(parts[i]);
    }
    
    free(parts);
    
    for(int i = 0; i < sizeOfListWithFileNames; i++) {
        free(fileNames[i]);
    }
    
    free(fileNames);
    
    return 0;
}

char** getPart(char* fileName, char** part) {

	FILE *file = fopen(fileName, "r");
	
	if (file == NULL)
	{
		return NULL;
	}
    
    for(int i = 0; i < DEFAUL_SIZE_OF_PART; i++)
    {
    	for(int j  = 0; j < DEFAUL_SIZE_OF_PART; j++)
    	{
    		int c;
    		if ((c = fgetc(file)) != EOF)
    		{
    			part[i][j] = (char) c;
    		} 
    		else 
    		{
    			printf("The fragment doesn't contain enough characters");
    			return NULL;
    		}
            part[i][DEFAUL_SIZE_OF_PART] = '\0';
    	}
    }
    
    // Closes the file.
	fclose(file);
	
    return part;
}

char** sort(char **array, int length){
	int comparisionResult;
    char tmpLine[LENGTH_OF_FILENAME];

	// Checks if there is only one element in the array so it's unnecessary
	// to sort it.
    if (length <= 1)
    {
        return array;
	}
	
	// Iterates through the array until it's sorted.
    for (int i = 0; i < length - 1; i++)
    {
    	int swapped = 0;
    	// Compares each neighboring pair of elements.
        for (int j = 0; j < length - i - 1; j++)
        {
            comparisionResult = strcmp(array[j], array[j+1]);
			
			// Swaps the elements if they are in wrong order.
            if (comparisionResult > 0)
            {
                strcpy(tmpLine, array[j+1]);
                strcpy(array[j+1], array[j]);
                strcpy(array[j], tmpLine);
                swapped = 1;
            }
        }
        
        // If swapped hasn't been change, it means that array is already sorted.
        if(swapped == 0)
        {
        	break;
        }
    }

	return array;
}

char** mergeParts(char** mergedPicture, char*** parts, int numberOfRows, int numberOfColumns) {
    
    for(int i = 0; i < numberOfRows; i++)
    {
        for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++)
        {
            strcpy(mergedPicture[j + i * DEFAUL_SIZE_OF_PART], parts[i][j]);
            for(int k = 1; k < numberOfColumns; k++)
            {
                strcat(mergedPicture[j + i * DEFAUL_SIZE_OF_PART], parts[i + k * numberOfRows][j]);
            }
        }
    }
    return mergedPicture;
}
