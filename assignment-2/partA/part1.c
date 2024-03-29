#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */


//function declarations.
bool check_arguments_for_integer(int argc, char *argv[]);
int *attach_shm_mem();
void delete_shm_mem();

int shmid;

/*
 * Function: main
 * --------------------
 *  main function that takes inputs (P and M) to create child process and calculate the sum of two static numbers and get the winner PID where sum is greater than M.
 *
 *  argc: number of arguments.
 *
 *  argv[1]: as P, number of child processes.
 *
 *  argv[2]: as M, maximum value of Sum of static two numbers.
 *
 *  returns: the primary end of program with return value.
 *  		0 if no errors, otherwise -1
 */
int main(int argc, char *argv[]){
	//base check
	if(argc != 3 || !check_arguments_for_integer(argc, argv)){
		printf("Invalid input parameters\n");
		printf("Please enter the number of levels of the tree (L) AND number of children of each internal node of the process tree (N).\n");
		return -1;
	}

	int num_of_children = atoi(argv[1]);
        int max_value = atoi(argv[2]);
        pid_t pid = -1;

	//create and attach shared memory
	int *data= attach_shm_mem();	
	for(int i = 0; i < num_of_children; i++){
		if(pid == -1 || pid > 0){
			pid = fork();

		}
	}

      	if(pid > 0){
		//writing initial values to shared memory
		data[0] = 1;
	    	data[1] = 2;
	    	data[2] = -1;
              
	    	while ((waitpid(-1, 0, 0)) != -1);
	}

        if (pid == 0){
		//child block
		printf("Executing child %d\n",getpid());
		while(data[0] < max_value && data[1] < max_value && data[2] == -1){			
			int total = data[0] + data[1];
			printf("%d - Calculating Sum\t Value 1: %d\t Value 2: %d\t Total: %d\n", getpid(), data[0], data[1], total);
			if (total > max_value && data[2] == -1){
				data[2] = getpid();
				printf("\n*** Winner PID: %d, Max Value: %d, Total: %d ***\n\n", getpid(), max_value, total);
				exit(0);
			} else {
				if (data[0] < data[1])
        	                        data[0] = total;
	                        else
                	                data[1] = total;
			}

		}
	}

	delete_shm_mem();	
	return 0;	
}

/*
 * Function:  attach_shm_mem
 * -----------------------------------
 *  Helper function to create, attach and retreive data from shared memory using the key.
 *
 *  returns: integer pointer to the shared memory.
 */
int *attach_shm_mem(){
	int *data;
	key_t key;
	/* make the key: */
        if ((key = ftok("test_file", 'X')) < 0) {
		perror("ftok");
                exit(1);
	}

        /* create the shared memory segment: */
        if ((shmid = shmget(key, SHM_SIZE, 0644)) == -1) {
		if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT | IPC_EXCL )) < 0) {
			perror("shmget");
                        exit(1);
                }
        }

        /* attach to the segment to get a pointer to it: */
        data = shmat(shmid, (void *)0, 0);
        if (data == (int *)(-1)) {
		perror("shmat");
                exit(1);
        }
	return data;
}

/*
 * Function:  delete_shm_mem
 * -----------------------------------
 *  Helper function to delete the created shared memory associated with shared memory ID.
 */
void delete_shm_mem(){
	/* delete the segment */
       if( shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
       }
}

/*
 * Function:  check_arguments_for_integer
 * -----------------------------------
 *  Helper function to check if the arguments are positive integer.
 *
 *  argc: number of arguments
 *
 *  argv[]: contains command line arguments.
 *
 *  returns: false, if any of the argument is not a positive integer, else true
 */
bool check_arguments_for_integer(int argc, char *argv[]){
	regex_t regex;
	int return_value;
	for(int i = 1; i < argc; i++){
		regcomp(&regex, "^[0-9]*$", 0);
		return_value = regexec(&regex, argv[i], 0, NULL, 0);
		if (return_value != 0) {
		    return false;
		}
	}
	return true;
}
