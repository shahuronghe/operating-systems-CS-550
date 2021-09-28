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


//function declaration.
bool checkArgumentsForInteger(int argc, char *argv[]);
void writeValues(int val1, int val2, int pid);


int *data;
int shmid;

/*
 * Function: main
 * --------------------
 *  main function that takes inputs (L and N) to create a process tree using fork() and execlp() system calls.
 *
 *  argc: number of arguments.
 *
 *  argv[1]: as lvl, number of level in process tree.
 *
 *  argv[2]: as n, number of children at each level.
 *
 *  returns: the primary end of program with return value.
 *  		0 if no errors, otherwise -1
 */
int main(int argc, char *argv[]){
	//base check
	if(argc != 3 || !checkArgumentsForInteger(argc, argv)){
		printf("Invalid input parameters\n");
		printf("Please enter the number of levels of the tree (L) AND number of children of each internal node of the process tree (N).\n");
		return -1;
	}
	int arr[3]= {1,2,-1};
	int winner_id;

	int P = atoi(argv[1]);
        int M = atoi(argv[2]);
        pid_t pid = -1;
	
	for(int i = 0; i < P; i++){
		if(pid == -1 || pid > 0){
			pid = fork();
		}
	}

        
      	if(pid > 0){
		//parent block
                key_t key;
//                int shmid;
  //              int *data;
                int arr[3]={1,2,-1};
                int Winner_id;
                int mode;
                int total;

           /* make the key: */
           if ((key = ftok("test_shm", 'X')) < 0) {
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

	    //writing initial values to shared memory
	    writeValues(arr[0], arr[1], arr[2]);
	    printf("values: %d , %d , %d \n",data[0],data[1],data[2]);
              
	    while ((pid = waitpid(-1, 0, 0)) != -1);
	}
	printf("pid: %d\n",pid);
        if (pid == 0){
		//child block
		printf("executing child, %d , %d\n",data[0],data[1]);
		while(data[0] < M || data[1] < M){
			int total = data[0] + data[1];
			if (data[0] < data[1])
				data[0] = total;
			else
				data[1] = total;

			if(total > M)
				data[2] = getpid();
		}
		printf("Winner PID: %d\n",data[2]);
		exit(0);
	}
	printf("exited\n");
	return 0;	
}

void writeValues(int val1, int val2, int pid){
	data[0] = val1;
	data[1] = val2;
	data[2] = pid;
}


/*
 * Function:  checkArgumentsForInteger
 * -----------------------------------
 *  Helper function to check if the arguments are positive integer.
 *
 *  argc: number of arguments
 *
 *  argv[]: contains command line arguments.
 *
 *  returns: false, if any of the argument is not a positive integer, else true
 */

bool checkArgumentsForInteger(int argc, char *argv[]){
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


