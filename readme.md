## Assignment 2. Merge	ASCII-art	

### Precondition
The program is fully functional with the test data in "wolf" and "mickey" folders (this is the second version of the test data). To get a correct result please use the test data from the folders attached to this project. 

### Compiling
The project contains the program (c_assignment_2.c) and file with util functions (merge-util.c). Before runing the program you need to compile both of them:
´gcc -Wall -Wextra -std=c11 merge-util.c c_assignment_2.c -o program´

### Execution
To execute the program you need to pass the name of directory with the test data as an argument:
´./program wolf/´
The program was tested with Valgrind. You can run:
´valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./program mickey/´

### How does the program work
1. The program create a list of files that contain the parts of an ASCII-picture by checking the content of the folder that was passed as argument by user.
2. The program reads each file from the list and puts the content of the file into a 2D array of characters (size of array is set as constant and is equal 30). 
3. 
