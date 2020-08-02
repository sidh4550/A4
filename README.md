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
Deepinder:

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
	

__Features__ 
Error checker for too many char
Thread clearer
Exit command for when done. 
Type "Exit" to close program.

__Tests__
EX: ./Assignment_4 10 10 10 10

RQ 0 1 1 1 1

RL 0 1 1 1 1

*

RUN

__Code Example__

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


__Authors__
Deepinder Sidhu @sidh4550


Matteo Straditto @stra4010

__Credits__
All credits go to Deepinder Sidhu and Matteo Straditto. Some inspiration taken from online tutorials and previous Assignments done in class.

__LICENSE__

COPYRIGHT Deepinder Sidhu and Matteo Straditto 2020
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
