#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include "util.h"

#define LENGTH_OF_FILENAME 256
#define DEFAUL_SIZE_OF_PART 30

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
   
    // Get all the files and directories within directory (using code from
    // assignment 1)
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
                
                sizeOfListWithFileNames *= 2;
                
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
                
                for(int i = 0; i < sizeOfListWithFileNames; i++)
                {
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
                    else
                    {
                        fileNames[i] = 
                        	malloc(LENGTH_OF_FILENAME * sizeof(char));
                    }
                }
                strcpy(fileNames[numberOfFileNames], dirname);
                strcat(fileNames[numberOfFileNames++], ent->d_name);
            }
        }
    }
        
    closedir(dir);
    free(dirname);
    
    // ============ Get number of columns and rows =====================
    
    fileNames = sort(fileNames, numberOfFileNames, LENGTH_OF_FILENAME);
    
    int numberOfRows, numberOfColumns;
    char* p = fileNames[numberOfFileNames - 1];
    
    int first = 0;
    
    while (*p)
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
    
    // Allocate memory for 3D array to store contain of the files.
    char*** parts;
    parts = malloc(numberOfFileNames * sizeof(char**));
    for(int i = 0; i < numberOfFileNames; i++) {
        parts[i] = malloc(DEFAUL_SIZE_OF_PART * sizeof(char*));
        for(int j = 0; j < DEFAUL_SIZE_OF_PART; j++) {
            parts[i][j] = malloc(DEFAUL_SIZE_OF_PART * sizeof(char));
        }
    }
    
    // ================= Reading files from the array =======================
    
    for(int i = 0; i < numberOfFileNames; i++)
    {
        parts[i] = readPartFromFile(fileNames[i], parts[i], DEFAUL_SIZE_OF_PART);
        
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
    
    // ======================= Merging fragments =============================
    
    // Allocating memory for array of strings containing the resault of merging
    char** mergedPicture;
    mergedPicture = malloc(numberOfRows * DEFAUL_SIZE_OF_PART * sizeof(char*));
    for(int i = 0; i < DEFAUL_SIZE_OF_PART * numberOfRows; i++)
    {
        mergedPicture[i] = 
        	malloc((DEFAUL_SIZE_OF_PART * numberOfColumns + 1) * sizeof(char));
        		
    }
    
    mergedPicture = mergeParts(mergedPicture, 
    									parts, numberOfRows, numberOfColumns,
    									DEFAUL_SIZE_OF_PART);
    
    
    // ==================== Write result to the file ==========================
    
    if(writeToFile("result.txt", mergedPicture,
                                    DEFAUL_SIZE_OF_PART * numberOfRows) == -1)
	{
		perror ("Error opening file");
		
		for(int i = 0; i < DEFAUL_SIZE_OF_PART * numberOfRows; i++)
		{
		    free(mergedPicture[i]);
		}
		
		free(mergedPicture);
		
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
    
    printf("%s %s\n%s\n", "Result of merging files is saved in", "result.txt",
    				   "You can ispect it by calling cat result.txt");
    
    // ====================== Freeing the memory =============================
    
    
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
