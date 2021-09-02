#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include<ctype.h>


bool checkArgumentsForInteger(char *argv[]);



int main(int argc, char *argv[])
{

	//base check
	if(argc != 3 || !checkArgumentsForInteger(argv)){
		printf("Invalid input parameters\n");
		printf("Please enter the number of levels of the tree (L) AND number of children of each internal node of the process tree (N).");
		return -1;
	}
	int L = atoi(argv[1]);
	int N = atoi(argv[2]);
	

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
	return 0;
}


bool checkArgumentsForInteger(char *argv[]){
        int i;
        for(i = 1; i < sizeof(&argv)-1;i++){
		int val = atoi(argv[i]);
		if(val == 0){
			return false;
		}
	}
	return true;
}
