# A4
Title:

CP386 Assignment 4: Bankers' Algorithm

Description:
This program simulates a "banker" that would manage and keep track of resources. The program handles resource requests and releases, and ensures that each customer request is "safe" meaning that the system has enough resources to manage the request.

Motivation:

To run the bankers' algorithm in C on a variety of processes that require a variety of resources of varying types in an easy-to-understand format.


Installation Instructions

	Rename makefile4.mk to Makefile using the following linux CLI command
		$mv makefile4.mk Makefile 

	To compile code use the following command
		$make

	Then run the code with the following command
		$./Assignment_4 [your numbers here]

		- *NOTE* the numbers following ./Assignment_4 are the maximum number
			 of availible reasources for each unique resource type
Features:

-Gets resouce numbers from a text file and then saves them into an array for future use 

-Asks user for input to see how many resources the program starts with.

-Lets the user request and release resources from processes, and ensure that the request and release are doable, before letting them go through

-Uses a saftey check, which ensure that each request and release is allowed, and the system has enough resources to accomadate.


Screenshots:



![Image 1](https://github.com/sidh4550/A4/blob/master/1.PNG)
![Image 2](https://github.com/sidh4550/A4/blob/master/2.PNG)
![Image 3](https://github.com/sidh4550/A4/blob/master/3.PNG)
![Image 4](https://github.com/sidh4550/A4/blob/master/4.PNG)


Contribution:
Deepinder- 
	void requestResource(int input[]);
	int check_safe(int type, int input[]);
	int release_resources(int input[]);
	void *runThread(void *thread);
	void runProgram(int input[]);
	
Matteo:
int populateAvailibleArray(int argc, char *argv[]);	// Fills availible array with data
	int populateMaxMatrix(char *resource_data); 		// Fills Max matrix with data
	int populateNeedMatrix();							// Updates Need matrix
	int initialize_data_structure();					// Fills all matricies with -1
	int promt_user();									// Prompts user for input
	int print_availible_array();						// Prints the availibility array
	int print_max_matrix();								// Prints the max matrix
	int print_allocated_matrix();						// Prints the allocation matrix
	int print_need_matrix();	
	
