#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** readPartFromFile(char* fileName, char** part, int partSize) 
{
	FILE *file = fopen(fileName, "r");
	
	if (file == NULL)
	{
		return NULL;
	}
    
    for(int i = 0; i < partSize; i++)
    {
    	for(int j  = 0; j < partSize; j++)
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
    
	fclose(file);
	
    return part;
}

char** mergeParts(char** mergedPicture, char*** parts, 
										int numberOfRows, int numberOfColumns,
										int partSize) 
{
    
    for(int i = 0; i < numberOfRows; i++)
    {
        for(int j = 0; j < partSize; j++)
        {
            memcpy(mergedPicture[j + i * partSize], 
            				parts[i][j], 
            				partSize * sizeof(char));
            for(int k = 1; k < numberOfColumns; k++)
            {
                memcpy(mergedPicture[j + i * partSize] + 
                							partSize * k, 
                							parts[i + k * numberOfRows][j], 
                							partSize * sizeof(char));
            }
            mergedPicture[j + i * partSize]
            					[numberOfColumns * partSize] = '\0';
        }
    }
    return mergedPicture;
}

// Use sort algorithm from assignment 1
char** sort(char **array, int length, int lengthOfFilename)
{
	int comparisionResult;
    char tmpLine[lengthOfFilename];

    if (length <= 1)
    {
        return array;
	}
	
    for (int i = 0; i < length - 1; i++)
    {
    	int swapped = 0;
    	
        for (int j = 0; j < length - i - 1; j++)
        {
            comparisionResult = strcmp(array[j], array[j+1]);
			
            if (comparisionResult > 0)
            {
                strcpy(tmpLine, array[j+1]);
                strcpy(array[j+1], array[j]);
                strcpy(array[j], tmpLine);
                swapped = 1;
            }
        }
        
        if(swapped == 0)
        {
        	break;
        }
    }

	return array;
}
