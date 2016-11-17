#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

#define LENGTH_OF_FILENAME 256
#define DEFAUL_SIZE_OF_PART 30

char** readPartFromFile(char* fileName, char** part);

char** sort(char **array, int counter);

char** mergeParts(char** mergedPicture, char*** parts,
										int numberOfRows, int numberOfColumns);

int main(int argc, char* argv[]) 
{
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
    
    char* dirname;
    dirname = malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(dirname, argv[1]);
    
    // Allocate memory for array of file names
    fileNames = malloc(sizeOfListWithFileNames * sizeof(char*));
    
    // Allocate memory for file names
    for(int i = 0; i < sizeOfListWithFileNames; i++)
    {
        fileNames[i]=malloc(LENGTH_OF_FILENAME * sizeof(char));
    }
    
    // Check if the directory exists
    if((dir = opendir (dirname)) == NULL) 
    {
    	perror ("Couldn't open the directory. Please enter the hole path. ");
    	
    	for(int i = 0; i < sizeOfListWithFileNames; i++) 
    	{
        	free(fileNames[i]);
    	}
    
    	free(fileNames);
    	free(dirname);
    	
        return EXIT_FAILURE;
    }
    
    // Check if user didn't forget "/" at the end of the path to the direrctory
    // and append it if it's necessary
    if(dirname && *dirname && dirname[strlen(dirname) - 1] != '/') 
    {
    	dirname = realloc(dirname, (strlen(dirname) + 2) * sizeof(char));
    	strcat(dirname, "/");
    }
   
    // get all the files and directories within directory
    while ((ent = readdir (dir)) != NULL)
    {
        // get only .txt files
        if(strstr(ent->d_name, ".txt") != NULL)
        {
            if (sizeOfListWithFileNames > numberOfFileNames)
            {
            	strcpy(fileNames[numberOfFileNames], dirname);
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
                    
                    for(int i = 0; i < sizeOfListWithFileNames; i++) {
						free(fileNames[i]);
					}
					free(fileNames);
					closedir(dir);
    				free(dirname);
        
                    return EXIT_FAILURE;
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
                            
                            for(int i = 0; i < sizeOfListWithFileNames; i++) {
								free(fileNames[i]);
							}
							free(fileNames);
							closedir(dir);
    						free(dirname);
				
                            return EXIT_FAILURE;
                        }
                    }
                    //Allocates memory for new strings.
                    else
                    {
                        fileNames[i] = 
                        	malloc(LENGTH_OF_FILENAME * sizeof(char));
                    }
                }
                // Copies the last filename that was read from directory to the 
                // first empty index in the array.
                strcpy(fileNames[numberOfFileNames], dirname);
                strcat(fileNames[numberOfFileNames++], ent->d_name);
            }
        }
    }
        
    closedir(dir);
    free(dirname);
    
    // ============ Get number of columns and rows =====================
    
    fileNames = sort(fileNames, numberOfFileNames);
    
    int numberOfRows, numberOfColumns;
    char* p = fileNames[numberOfFileNames - 1];
    
    int first = 0;
    
    while (*p) // While there are more characters to process...
    { 
        if (isdigit(*p))
        { 
            long val = strtol(p, &p, 10);
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
        {
            p++;
        }
    }
    
    // Check if the file name contains numbers of both columns and rows.
    if (first <= 1)
    {
        printf("%s %s\n", "Wrong format of file name.", 
        					"Cannot detect the positions of the fragments");
        
        for(int i = 0; i < sizeOfListWithFileNames; i++) {
            free(fileNames[i]);
        }
        
        free(fileNames);
        
        return EXIT_FAILURE;
    }
    
    char*** parts;
    parts = malloc(numberOfFileNames * sizeof(char**));
    for(int i = 0; i < numberOfFileNames; i++) {
        parts[i] = malloc(DEFAUL_SIZE_OF_PART * sizeof(char*));
        for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++) {
            parts[i][j] = malloc(DEFAUL_SIZE_OF_PART * sizeof(char));
        }
    }
    
    // =============== reading files from the array =======================
    
    for(int i = 0; i < numberOfFileNames; i++)
    {
        parts[i] = readPartFromFile(fileNames[i], parts[i]);
        if (parts[i] == NULL)
        {
            for(int i = 0; i < numberOfFileNames; i++) 
            {
                for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++) 
                {
                    free(parts[i][j]);
                }
            }
            
            for(int i = 0; i < numberOfFileNames; i++) 
            {
                free(parts[i]);
            }
            
            free(parts);
            
        	for(int i = 0; i < numberOfFileNames; i++) 
        	{
        		free(fileNames[i]);
    		}
    
   			free(fileNames);
   			
        	return EXIT_FAILURE;
        }
    }
    
    // ===================== merging fragments ===========================
    
    char** mergedPicture;
    mergedPicture = malloc(numberOfRows * DEFAUL_SIZE_OF_PART * sizeof(char*));
    for(int i = 0; i < DEFAUL_SIZE_OF_PART * numberOfRows; i++)
    {
        mergedPicture[i] = 
        	malloc((DEFAUL_SIZE_OF_PART * numberOfColumns + 1) * sizeof(char));
        		
    }
    
    mergedPicture = mergeParts(mergedPicture, 
    									parts, numberOfRows, numberOfColumns);
    
    // =============== DEBUGGING =======================
        
    printf("Number of col: %d, number of rows: %d\n", 
    					numberOfColumns, numberOfRows);
    
    for(int i = 0; i < numberOfFileNames; i++) {
        printf("%d. %s\n", i + 1, fileNames[i]);
    }
    
    for(int i = 0; i < DEFAUL_SIZE_OF_PART * numberOfRows; i++)
    {
        printf("%s", mergedPicture[i]);
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

char** readPartFromFile(char* fileName, char** part) 
{
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
    	}
    }
    
    // Closes the file.
	fclose(file);
	
    return part;
}

char** mergeParts(char** mergedPicture, char*** parts, 
										int numberOfRows, int numberOfColumns) 
{
    
    for(int i = 0; i < numberOfRows; i++)
    {
        for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++)
        {
            memcpy(mergedPicture[j + i * DEFAUL_SIZE_OF_PART], 
            				parts[i][j], 
            				DEFAUL_SIZE_OF_PART * sizeof(char));
            for(int k = 1; k < numberOfColumns; k++)
            {
                memcpy(mergedPicture[j + i * DEFAUL_SIZE_OF_PART] + 
                							DEFAUL_SIZE_OF_PART * k, 
                							parts[i + k * numberOfRows][j], 
                							DEFAUL_SIZE_OF_PART * sizeof(char));
            }
            mergedPicture[j + i * DEFAUL_SIZE_OF_PART]
            					[numberOfColumns * DEFAUL_SIZE_OF_PART] = '\0';
        }
    }
    return mergedPicture;
}

char** sort(char **array, int length)
{
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
