char** readPartFromFile(char* fileName, char** part, int partSize);

char** sort(char **array, int counter, int lengthOfFilename);

char** mergeParts(char** mergedPicture, char*** parts,
										int numberOfRows, int numberOfColumns,
										int partSize);

int writeToFile(char* name, char** mergedPicture, int numberOfLines);
