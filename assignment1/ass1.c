#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include<ctype.h>
#include <regex.h>



bool checkArgumentsForInteger(int argc, char *argv[]);


int main(int argc, char *argv[]){
	//base check
	if(argc != 3 || !checkArgumentsForInteger(argc, argv)){
		printf("Invalid input parameters\n");
		printf("Please enter the number of levels of the tree (L) AND number of children of each internal node of the process tree (N).\n");
		return 0;
	}

	int lvl = atoi(argv[1]);
	int n = atoi(argv[2]);

	if(lvl==0){
		return 0;
	}

	/**
	pid_t pid;
	int status;
	int ret;
	pid = fork();
	pid = fork();
 	if (pid < 0) { 
		perror("fork failed:"); 
		exit(1); 
	} 
	if (pid == 0) { // Child executes this block
		printf("This is the child with id: %d\n",getpid()); 
		exit(99); 
	} 
	if (pid > 0) { //Parent executes this block
		printf("This is parent. The child is %d\n", pid); 

		ret = waitpid(pid, &status, 0);
		if (ret < 0) {
			perror("waitpid failed:");
			exit(2); 
		}
		printf("Child exited with status %d\n", WEXITSTATUS(status));
	}
	*/
	int status;
	pid_t pid = fork();
	printf("Process starting\n");		
	printf("Parent's id: %d\n",getppid());
	printf("Level in the tree = %d\n",lvl);
	printf("Creating %d children at Level %d\n",n,lvl);
	for(int j = 0; j < n; j++){

	}
	wait(&status);
	return 0;	
}

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


