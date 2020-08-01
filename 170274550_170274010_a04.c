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
int populateNeedMatrix();							// Updates Need matrix
int initialize_data_structure();					// Fills all matricies with -1
int promt_user();									// Prompts user for input
int print_availible_array();						// Prints the availibility array
int print_max_matrix();								// Prints the max matrix
int print_allocated_matrix();						// Prints the allocation matrix
int print_need_matrix();							// Prints the need matrix
void requestResource(int input[]);

// Global Variables
char lines[20][100];
Resources data;

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
	populateNeedMatrix();
	
	// Create sem locks
	
	promt_user();
	
	/*
	// Promp user for input
	int flag = 0;
	while (flag == 0) 
	{
		flag = promt_user();
	}
	*/

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

int populateNeedMatrix()
{
	for (int i = 0; i < data.num_customers; i++)
	{
		for (int j = 0; j < data.num_unique_resources; j++)
		{
			data.need[i][j] = data.max[i][j] - data.allocated[i][j];
		}
	}
	
	return 0;
}

int release_resources(int input[]) 
{
	
	
	int customer_num = input[0];
	if(input[1]<=data.allocated[customer_num][0]&&input[2]<=data.allocated[customer_num][1]&&input[3]<=data.allocated[customer_num][2]&&input[4]<=data.allocated[customer_num][3]){
		for (int i = 0; i < data.num_unique_resources; i++) 
		{
			
			// Update allocations matrix
			//sem_wait(&mutexalloc);
			data.allocated[customer_num][i] -= input[i+1];
			//sem_post(&mutexalloc);
			
			// Update availiblity matrix
			//sem_wait(&mutexavail);
			data.available[i] += input[i+1];
			//sem_post(&mutexavail);
			
			// Update needs matrix
			//sem_wait(&mutexneed);
			data.need[customer_num][i] += input[i+1];
			//sem_post(&mutexneed);
		}
		
	}
	else{
		printf("Release cannon exceed allocated resources");
	}
	
	
	return 1;
}

void requestResource(int input[]){
	int id = input[0];
	if(input[1]<=data.need[id][0]&&input[2]<=data.need[id][1]&&input[3]<=data.need[id][2]&&input[4]<=data.need[id][3]){
		if(input[1]<=data.available[0]&&input[2]<=data.available[1]&&input[3]<=data.available[2]&&input[4]<=data.available[3]){
			for (int i = 0; i < data.num_unique_resources; i++){
				data.available[i]-=input[i+1];
				data.allocated[id][i]+=input[i+1];
				data.need[id]	[i]-=input[i+1];

			}
				

		}
		else{
			printf("Request exceeds available resources");
		}
	}
	else{
	printf("Request exceeds customer need\n");
}
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
	int i = 0;
	int input[100];
	char *token;
	
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
		
		// Fill input[] array with command
		token = strtok(request, " ");
		i = 0;
		
		while(token != NULL)
		{
			token = strtok(NULL, " ");
			
			if (token != NULL)
			{
				input[i] = *token - '0';
				printf("input --> %d\n", input[i]);
				i++;
			}
			
		}
		
		
		requestResource(input);
		//check_safe(input);
		
		
	} else if (strncmp(request, release_command, 2) == 0) {
		printf("Releaseing Resources...\n");
		
		// Fill input[] array with command
		token = strtok(request, " ");
		i = 0;
		
		while(token != NULL)
		{
			token = strtok(NULL, " ");
			
			if (token != NULL)
			{
				input[i] = *token - '0';
				printf("input --> %d\n", input[i]);
				i++;
			}
			
		}
		
		release_resources(input);
		//check_safe(input);
		
	} else if (strncmp(request, run_command, 3) == 0) {
		printf("Running Safety Algorithm...\n");
		
		// Fill input[] array with command
		token = strtok(request, " ");
		i = 0;
		
		// Fill input[] array with command
		token = strtok(request, " ");
		i = 0;
		
		while(token != NULL)
		{
			token = strtok(NULL, " ");
			
			if (token != NULL)
			{
				input[i] = *token - '0';
				printf("input --> %d\n", input[i]);
				i++;
			}
			
		}
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
