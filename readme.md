## Assignment 2. Merge	ASCII-art	

### Precondition
The program is fully functional with the test data in "wolf" and "mickey" folders (this is the second version of the test data). To get a correct result please use the test data from the folders attached to this project. 

### Compiling
The project contains the program (c_assignment_2.c) and file with util functions (util.c). Before runing the program you need to compile both of them:

`gcc -Wall -Wextra -std=c11 util.c c_assignment_2.c -o program`

### Execution
To execute the program you need to pass the name of directory with the test data as an argument:

`./program wolf/`

The program was tested with Valgrind. You can run:

`valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./program mickey/`

### How does the program work
1. The program creates a list of files that contain the parts of an ASCII-picture by checking the content of the folder that was passed as argument by user, and detects how many columns and rows the original picture has (by checking respective numbers in file names).
2. The program reads each file from the list and puts the content of the file into a 2D array of characters (size of array is set as constant and is equal 30). 
3. The program places content of each file (i.e. 2D array of chars) into a 3D array. When the program has read all the files, the 3D array contains all the data from the files.
4. Then the program merges fragments from respective indices of the 3D array into strings and put them into a new 2D array that will contain the result of merging. On this stage the program converts sequences of characters into strings by adding '\0' at the end.
5. When all data from the 3D array are merged and placed into 2D array, the program writes the 2D array into file. 

### Alternative solutions
The program uses 3D array as buffer that stores all the data from the files and places the merged data into a 2D array before writing it into a file. Another approach is to read original files part by part, merge respective parts and put them into the result file. In this case the program won't use external memory resources for temporal saving of the data, but it should deal simultaneously with n+1 files, where n is the number of the files with fragments of the picture plus one file with the result of the merging. 
The merging can also be done with just one 2D array of characters for buffering the data. In this case each index of that array will represent a column of files and the content of each index will be a sequence of characters from all the files from that column. 
