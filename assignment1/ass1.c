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

	int status;
	pid_t pid=-1;
	printf("Process starting\n");		
	printf("Parent's id: %d\n", getppid());
	printf("Level in the tree = %d\n", lvl);
	printf("Creating %d children at Level %d\n", n, lvl);
	
	for(int j = 0; j < n; j++){
		if(pid==-1 || pid > 0)
			pid = fork();
	}
	if(pid<0){
		perror("fork failed\n");
		exit(-1);
	}
	
	if(pid > 0){
		int ret = waitpid(pid,&status,0);
		if(ret < 0){
			perror("waitpid failed\n");
		}else{
			printf("Terminating at Level: %d\n",lvl);
		}
	}
	if(pid == 0){
		sleep(1);
		char l[10];
                char n2[10];
                sprintf(l,"%d",lvl-1);
                sprintf(n2,"%d",n);

		if(execlp("./ass1","./ass1", l, n2, (char *) NULL)==-1){
			printf("exec call failed\n");
		}
	}
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


