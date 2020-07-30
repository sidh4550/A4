#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

// A structure to track system resources
typedef struct resources
{

	int available[100]; 		// The availible amount of each resource
	int max[100][100];		 	// The maximum demand of each customer
	int allocated[100][100]; 	// The amount currently allocated to each customer
	int need[100][100];		 	// The remaining need of each customer
	int num_customers;			// # of customers (used to size matrix)
	int num_unique_resources;	// # of unique resources (used to size matrix)

} Resources;

// Function Declarations
char *readFile(char *file_name);					// Function to read from sample4_in.txt
int populateAvailibleArray(int argc, char *argv[]);	// Fills availible array with data
int populateMaxMatrix(char *resource_data); 		// Fills Max matrix with data
int initialize_data_structure();					// Fills all matricies with -1
int promt_user();									// Prompts user for input
int print_availible_array();						// Prints the availibility array
int print_max_matrix();								// Prints the max matrix
int print_allocated_matrix();						// Prints the allocation matrix
int print_need_matrix();							// Prints the need matrix


// Global Variables
char lines[20][100];
Resources data;
sem_t mutexalloc; 
sem_t mutexavail; 
sem_t mutexneed; 
//int check_safe(Resources* data);

// Main
int main(int argc, char *argv[])
{

	// Read from sample4_in.txt
	char *resource_data;
	resource_data = readFile("sample4_in.txt");

	// Create and populate resources structure
	populateMaxMatrix(resource_data);
	populateAvailibleArray(argc, argv);
	
	// Create sem locks
	sem_init(&mutexalloc,0,1);
	sem_init(&mutexavail,0,1);
	sem_init(&mutexneed,0,1);
	
	// Promp user for input
	promt_user();

	return 0;
}

char *readFile(char *file_name)
{

	// Open file
	FILE *in = fopen(file_name, "r");

	// Check that file has opened correctly
	if (!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		exit(0);
	}

	// Create dynamic array to hold file content
	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';

	// Loop through file and append contents to fileContent
	while (!feof(in))
	{
		char line[100];
		if (fgets(line, 100, in) != NULL)
		{
			strncat(fileContent, line, strlen(line));
			strcpy(lines[0], line);
		}
	}

	// Close file
	fclose(in);

	return fileContent;
}

int populateAvailibleArray(int argc, char *argv[])
{
	
	for (int i = 1; i < argc - 1; i++) 
	{
		data.available[i - 1] = atoi(argv[i]);
	}
	
	
	return 0;
}

int populateMaxMatrix(char *resource_data)
{

	// Store information
	char *temp_lines[100];

	// Seperate string by each line, save to temp_lines
	int i = 0;
	char *line = strtok(resource_data, "\n");

	while (line != NULL)
	{
		temp_lines[i] = line;
		line = strtok(NULL, "\n");

		data.num_customers++;
		i++;
	}

	// Seperate each line by commas, save to data.max[][]
	i = 0;
	int j = 0;
	char *c;

	while (i < data.num_customers)
	{
		c = strtok(temp_lines[i], ",");
		data.num_unique_resources = 0;

		while (c != NULL)
		{
			data.max[i][j] = atol(c);
			c = strtok(NULL, ",");

			data.num_unique_resources++;
			j++;
		}

		j = 0;
		i++;
	}

	return 0;
}

int release_resources(int customer_num, int request[]) 
{
	
	int i;
	for (i = 0; i < data.num_unique_resources; i++) 
	{
		
		// Update allocations matrix
		sem_wait(&mutexalloc);
		data.allocated[customer_num][i] -= request[i];
		sem_post(&mutexalloc);
		
		// Update availiblity matrix
		sem_wait(&mutexavail);
		//data.available[customer_num][i] += request[i];
		sem_post(&mutexavail);
		
		// Update needs matrix
		sem_wait(&mutexneed);
		data.need[customer_num][i] = data.max[customer_num][i] + data.allocated[customer_num][i];
		sem_post(&mutexneed);
	}
	
	return 1;
}

int check_safe()
{

	int ii, jj, work[data.num_unique_resources], finish[data.num_customers];
	int success = 0;

	// Set work equal to availiblity
	for (ii = 0; ii < data.num_unique_resources; ii++) 
	{
		work[ii] = data.available[ii];
	}

	// Populate finish[] with 0's
	for (ii = 0; ii < data.num_customers; ii++) 
	{ 
		finish[ii] = 0;
	}

	// Find element where finish[] = 0 and need[] <= work
	for (ii = 0; ii < data.num_customers; ii++) 
	{

		if (finish[ii] == 0) 
		{

			for (jj = 0; jj < data.num_unique_resources; jj++) 
			{

				if (data.need[ii][jj] > work[jj]) 
				{
					success = -1;
				}
				
				return success;
			}

			for (jj = 0; jj < data.num_unique_resources; jj++) 
			{

				work[jj] += data.allocated[ii][jj];
				
			}

			success = 1;
		}
	}

	return success;
}

int promt_user() 
{
	// Store availible commands
	char print_command[1];
	strcpy(print_command, "*");
	
	char request_command[2];
	strcpy(request_command, "RQ");
	
	char release_command[2];
	strcpy(release_command, "RL");
	
	char run_command[3];
	strcpy(run_command, "Run");
	
	// Create variable to store user input 
	char request[100];
	
	// Prompt user for input
	fgets(request, 100, stdin);
	
	// Determine which command was given
	if (strncmp(request, print_command, 1) == 0) {
		printf("Printing Data Structures...\n\n");
		
		printf("Availible Resources: \n");
		print_availible_array();
		
		printf("Max Request Matrix: \n");
		print_max_matrix();
		
		printf("Allocation Matrix: \n");
		print_allocated_matrix();
		
		printf("Need Matrix: \n");
		print_need_matrix();
		
	} else if (strncmp(request, request_command, 2) == 0) {
		printf("Processing Resouce Request...\n");
		
		
	} else if (strncmp(request, release_command, 2) == 0) {
		printf("Releaseing Resources...\n");
		
	} else if (strncmp(request, run_command, 3) == 0) {
		printf("Running Safety Algorithm...\n");
	}
	
	return 0;
}

int print_availible_array() 
{
	// Print out values of data.max array (for testing)
	int i = 0;
	while (i < data.num_unique_resources)
	{
		printf("%d, ", data.available[i]);
		i++;
	}
	printf("\n\n");
	
	return 0;
}

int print_max_matrix() 
{
	
	// Print out values of data.max array (for testing)
	int i = 0;
	int j = 0;
	while (i < data.num_customers)
	{
		while (j < data.num_unique_resources)
		{
			printf("%d, ", data.max[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	
	printf("\n");
	
	return 0;
}

int print_allocated_matrix() 
{
	
	// Print out values of data.max array (for testing)
	int i = 0;
	int j = 0;
	while (i < data.num_customers)
	{
		while (j < data.num_unique_resources)
		{
			printf("%d, ", data.allocated[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	
	printf("\n");
	
	return 0;	
}

int print_need_matrix() 
{
	
	// Print out values of data.max array (for testing)
	int i = 0;
	int j = 0;
	while (i < data.num_customers)
	{
		while (j < data.num_unique_resources)
		{
			printf("%d, ", data.need[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	
	printf("\n");
	
	return 0;
}

