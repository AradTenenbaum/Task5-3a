#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct employee
{
	int name_length;
	char* name;
	float salary;
} Employee;

typedef struct student {
	char* name;
	int average;
}STUDENT;


void memoryAndFileValidation(void* ptr);
void Exe5Q1(char* fname, unsigned int x);
void freeStringArray(char** strArr, int size);

void Exe5Q2(char* fname1, char* fname2);

void Exe5Q3(char* fname);

char** findAverageGrade(char* database, int avgGrade, int* resSize);

void main() {
	char fname1[100];
	char fname2[100];
	printf("Enter a file name: ");
	scanf("%s", fname1);

	Exe5Q3(fname1);

}


void Exe5Q1(char* fname, unsigned int x) {
	FILE* fp = fopen(fname, "rb");
	memoryAndFileValidation(fp);


	int size = 0;
	char** stringArray;
	char c;
	int smallIndex = 0;

	stringArray = malloc(sizeof(char*) * x);
	memoryAndFileValidation(stringArray);

	for (int i = 0; i < x; i++)
	{
		// Get size of string
		fread(&size, sizeof(int), 1, fp);
		stringArray[i] = malloc(sizeof(char) * (size + 1));
		memoryAndFileValidation(stringArray[i]);
		
		smallIndex = 0;
		
		for (int j = 0; j < size; j++)
		{
			// Get char
			fread(&c, sizeof(char), 1, fp);
			// Insert to array only the small characters
			if (c <= 'z' && c >= 'a') {
				stringArray[i][smallIndex] = c;
				smallIndex++;
			}
		}

		// End the string
		stringArray[i][smallIndex] = '\0';

		if ((smallIndex + 1) < size) {
			stringArray[i] = realloc(stringArray[i], sizeof(char) * (smallIndex + 1));
			memoryAndFileValidation(stringArray[i]);
		}


	}

	fclose(fp);

	// Create the new file name
	char* newName = malloc(sizeof(char) * (strlen(fname) + 4));
	memoryAndFileValidation(newName);

	strcpy(newName, fname);
	strcat(newName, ".txt");

	// Open the file as text for writing
	FILE* fpTxt = fopen(newName, "w");
	memoryAndFileValidation(fpTxt);

	// Insert the strings with enter
	for (int i = 0; i < x; i++)
	{
		fprintf(fpTxt, "%s\n", stringArray[i]);
	}

	fclose(fpTxt);

	// Free all strings
	freeStringArray(stringArray, x);

}

// Free a string array
void freeStringArray(char** strArr, int size) {
	for (int i = 0; i < size; i++)
	{
		free(strArr[i]);
	}
	free(strArr);
}

// Free an employee array
void freeEmployeeArray(Employee** empArr, int size) {
	for (int i = 0; i < size; i++)
	{
		free(empArr[i]->name);
		free(empArr[i]);
	}
	free(empArr);
}

// Swap two employees in array
void swapEmployee(Employee** emp1, Employee** emp2) {
	Employee* temp;
	temp = *emp1;
	*emp1 = *emp2;
	*emp2 = temp;
}

// Insert into sorted array of employees pointers
void insertSorted(Employee** empArr, int size) {
	int i = size - 1;
	// While the salary is higer than the index we go to lower indexes
	// Swap each employee till then
	while ((i-1) >= 0 && (empArr[i]->salary > empArr[i-1]->salary)) {
		swapEmployee(&(empArr[i]), &(empArr[i-1]));
		i--;
	}
}

void Exe5Q2(char* fname1, char* fname2) {
	Employee** empArr;
	Employee* currEmp;
	int nameLength = 0;
	int arrSize = 0;
	float addition = 0;

	// Allocate employee pointer Array
	empArr = malloc(sizeof(Employee*));
	memoryAndFileValidation(empArr);

	// Allocate employee pointer
	currEmp = malloc(sizeof(Employee));

	// Open files
	FILE* fp1 = fopen(fname1, "rb");
	memoryAndFileValidation(fp1);
	FILE* fp2 = fopen(fname2, "rb");
	memoryAndFileValidation(fp2);

	// Read the first element
	fread(&nameLength, sizeof(int), 1, fp1);

	while (feof(fp1) == 0) {
		arrSize++;

		empArr = realloc(empArr, sizeof(Employee*) * arrSize);
		memoryAndFileValidation(empArr);

		// Allocation of employee
		empArr[arrSize - 1] = malloc(sizeof(Employee));
		memoryAndFileValidation(empArr[arrSize - 1]);

		// Get name length and allocate name memory
		empArr[arrSize - 1]->name_length = nameLength;
		empArr[arrSize - 1]->name = malloc(sizeof(char) * (nameLength));
		memoryAndFileValidation(empArr[arrSize - 1]->name);

		// Get the name
		fread((empArr[arrSize - 1]->name), sizeof(char), nameLength, fp1);
		//empArr[arrSize - 1]->name[nameLength] = '\0';

		// Get salary
		fread(&(empArr[arrSize - 1]->salary), sizeof(float), 1, fp1);

		// Addition from second file
		fread(&addition, sizeof(float), 1, fp2);

		empArr[arrSize - 1]->salary += addition;

		// Sort array with the new element
		insertSorted(empArr, arrSize);

		// Get the next name length, end of loop so the feof works
		fread(&nameLength, sizeof(int), 1, fp1);
	}

	fclose(fp1);

	// Open the file for writing
	fp1 = fopen(fname1, "wb");
	memoryAndFileValidation(fp1);
	
	// Write the new data to the file
	for (int i = 0; i < arrSize; i++)
	{
		fwrite(&(empArr[i]->name_length), sizeof(int), 1, fp1);
		for (int j = 0; j < empArr[i]->name_length; j++) fwrite(&(empArr[i]->name[j]), sizeof(char), 1, fp1);
		fwrite(&(empArr[i]->salary), sizeof(float), 1, fp1);
	}

	fclose(fp1);
	fclose(fp2);
	freeEmployeeArray(empArr, arrSize);
}

void Exe5Q3(char* fname) {
	FILE* fp = fopen(fname, "rb");
	memoryAndFileValidation(fp);

	int size = 0;
	int nameSize = 0;
	char* name;
	int avg = 0;
	int loc;

	// Get size
	fread(&size, sizeof(short int), 1, fp);

	// Allocate an array of indexes of (100 x size)
	// Allocate an array of the size of each cell
	// Assuming the max grade is 100
	int** indexes = malloc(sizeof(int*) * 100);
	memoryAndFileValidation(indexes);
	int* indexesSizes = calloc(sizeof(int), 100);
	memoryAndFileValidation(indexesSizes);

	for (int i = 0; i < 100; i++)
	{
		indexes[i] = malloc(sizeof(int) * size);
		memoryAndFileValidation(indexes[i]);
	}

	for (int i = 0; i < size; i++)
	{
		// Get the student location in the file
		loc = ftell(fp);

		// Read name size and name
		fread(&nameSize, sizeof(short int), 1, fp);
		name = malloc(sizeof(char) * nameSize);
		memoryAndFileValidation(name);
		fread(name, sizeof(char), nameSize, fp);

		// Read avarage
		fread(&(avg), sizeof(int), 1, fp);

		// Inset avarage into bucket
		indexesSizes[avg-1]++;
		indexes[avg-1][indexesSizes[avg-1]-1] = loc;


		free(name);
	}

	fclose(fp);

	// Create the new file name
	char* newName = malloc(sizeof(char) * (strlen(fname) + 5));
	memoryAndFileValidation(newName);
	strcpy(newName, fname);
	int fnameLength = strlen(fname);
	newName[fnameLength-3] = 'i';
	newName[fnameLength-2] = 'n';
	newName[fnameLength-1] = 'd';
	strcat(newName, ".bin");

	// Open new file
	fp = fopen(newName, "wb");
	memoryAndFileValidation(fp);

	// Write to the file using the indexes array
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < indexesSizes[i]; j++) {
			fwrite(&(indexes[i][j]), sizeof(int), 1, fp);
		}
	}

	fclose(fp);
	// Free all indexes
	for (int i = 0; i < 100; i++)
	{
		free(indexes[i]);
	}

	free(indexes);
	free(indexesSizes);


}

char** findAverageGrade(char* database, int avgGrade, int* resSize) {

	int index = 0;
	int nameSize = 0;
	char* name;
	int avg = 0;

	char** namesArr = malloc(sizeof(char*));

	// Create index file name
	char* indexFileName = malloc(sizeof(char) * (strlen(database) + 5));
	memoryAndFileValidation(indexFileName);
	strcpy(indexFileName, database);
	int fnameLength = strlen(database);
	indexFileName[fnameLength - 3] = 'i';
	indexFileName[fnameLength - 2] = 'n';
	indexFileName[fnameLength - 1] = 'd';
	strcat(indexFileName, ".bin");

	// Open files
	FILE* fpIndexes = fopen(indexFileName, "rb");
	memoryAndFileValidation(fpIndexes);
	FILE* fpDB = fopen(database, "rb");
	memoryAndFileValidation(fpDB);

	// Read the first index
	fread(&index, sizeof(int), 1, fpIndexes);

	while (feof(fpIndexes) == 0) {

		// Go the the student index in database
		fseek(fpDB, index, SEEK_SET);

		// Get name size and name
		fread(&nameSize, sizeof(short int), 1, fpDB);
		name = malloc(sizeof(char) * nameSize);
		memoryAndFileValidation(name);
		fread(name, sizeof(char), nameSize, fpDB);

		// Read avarage
		fread(&(avg), sizeof(int), 1, fpDB);

		// If avarage is bigger than input avarage 
		// Allocate more space in namesArr and insert the student's name
		if (avg > avgGrade) {
			namesArr = realloc(namesArr, sizeof(char*) * (++(*resSize)));
			namesArr[(*resSize) - 1] = name;
		}

		// Read index at the end so the feof works
		fread(&index, sizeof(int), 1, fpIndexes);
	}

	fclose(fpDB);
	fclose(fpIndexes);
	free(indexFileName);

	return namesArr;

}

void memoryAndFileValidation(void* ptr) {
	if (ptr == NULL)
	{
		printf("File/Memory allocation falied.\n");
		exit(1);
	}
}
