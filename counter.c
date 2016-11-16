#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    
    int z = argc;
    if(z == 1) printf("Error");

    FILE *file = fopen(argv[1], "r");
    
    if (file == NULL)
        return -1; //could not open file
    

    char picture[30][30];
    
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            picture[i][j] = (char) fgetc(file);
        }
    }
    
    fclose(file);
    
    int counter = 0;
    
    for(int j = 0; j < 30; j++)
        {
            for(int k = 0; k < 30; k++) 
            {
                printf("%c", picture[j][k]);
                counter++;
                if(counter % 31 == 0) printf("\n");
            }
        }
    
    /*for(int i = 0; i < 30; i++)
    {
        printf("%s\n", picture[i]);
    }*/
    
    return 0;
}

