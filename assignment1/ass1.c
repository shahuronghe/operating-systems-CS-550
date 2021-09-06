#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include<ctype.h>


bool checkArgumentsForInteger(int argc, char *argv[]);

void createProcessTree(int L, int N);


int main(int argc, char *argv[])
{

	//base check
	if(argc != 3 || !checkArgumentsForInteger(argc, argv)){
		printf("Invalid input parameters\n");
		printf("Please enter the number of levels of the tree (L) AND number of children of each internal node of the process tree (N).\n");
		return -1;
	}

	int L = atoi(argv[1]);
	int N = atoi(argv[2]);

	createProcessTree(L, N);

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

void createProcessTree(int L, int N){
	int status;
	if(L==0){
		exit(0);
	}
	pid_t pid =	fork();
	if(pid>0){
	printf("Process starting\n");
	printf("Parent's id: %d\n",getppid());
	printf("Level in the tree = %d\n", L);
	printf("Creating %d children at Level %d\n",N,L);
	}
	if(pid==0){
	for(int j = 0; j < N; j++){
//		if(execlp("echo","echo","Hello from child",(char*) NULL) ==-1){
//			printf("Process Creation Failed");
//			exit(-1);
//		}
		fork();
	}
	}
	wait(&status);
	createProcessTree(L-1,N);

	
}

bool checkArgumentsForInteger(int argc, char *argv[]){
        for(int i = 1; i < argc; i++){
		int val = atoi(argv[i]);
		if(val == 0){
			return false;
		}
	}
	return true;
}


