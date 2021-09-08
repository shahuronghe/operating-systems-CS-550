#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>


//function declaration.
bool checkArgumentsForInteger(int argc, char *argv[]);

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
	int lvl = atoi(argv[1]);
	int n = atoi(argv[2]);

	//checking if level is zero, then end of program.
	if(lvl==0){
		return 0;
	}

	int status;
	pid_t pid = -1;
	printf("Process starting\n");		
	printf("Parent's id: %d\n", getppid());
	printf("Level in the tree = %d\n", lvl);
	printf("Creating %d children at Level %d\n", n, lvl);
	
	//creating N children for parents
	for(int j = 0; j < n; j++){
		if(pid == -1 || pid > 0){
			pid = fork();
		}
	}

	if(pid < 0){
		perror("fork failed\n");
		exit(-1);
	}
	
	if(pid > 0){
		int ret = waitpid(pid, &status, 0);
		if(ret < 0){
			perror("waitpid failed\n");
		} else {
			printf("Terminating at Level: %d\n", lvl);
		}
	}
	if(pid == 0){
		//1sec sleep for seamless simulation of process tree.
		sleep(1);
		
		char lvl2[10];
                char n2[10];
		
		//conversion of int to char to pass them as arguments to this program.
                sprintf(lvl2, "%d", lvl-1);
                sprintf(n2, "%d", n);

		if(execlp("./ass1", "./ass1", lvl2, n2, (char *) NULL)==-1){
			printf("exec call failed\n");
			exit(-1);
		}
	}
	return 0;	
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


