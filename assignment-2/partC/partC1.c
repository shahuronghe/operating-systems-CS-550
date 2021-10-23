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
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/time.h>


#define SHM_SIZE 1024  /* make it a 1K shared memory segment */


//function declarations.
bool check_arguments_for_integer(int argc, char *argv[]);
struct GameVariables *attach_shm_mem();
void delete_shm_mem();
void lock(struct GameVariables *data);
void unlock(struct GameVariables *data);

//shared memory id
int shmid;

//struct for storing the variable such as  addition variables, winnerId and semaphore.
struct GameVariables {
	int val1;
	int val2;
	int winnerId;
	sem_t sem;
	long total_child_time;
	long number_of_cycles;
};

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
	struct timeval start_time;
	gettimeofday(&start_time, NULL);

	int num_of_children = atoi(argv[1]);
        int max_value = atoi(argv[2]);
	struct GameVariables *data= attach_shm_mem();
	long child_avg = 0;
	long number_of_cycles = 0;	
	data->val1 = 1;
        data->val2 = 2;
        data->winnerId = -1;
	data->total_child_time = 0;
	data->number_of_cycles = 0;

	sem_t semaphore;
        pid_t pid = -1;
	sem_init(&data->sem, 1, 1);
	for(int i = 0; i < num_of_children; i++){
		if(pid == -1 || pid > 0){
			pid = fork();

		}
	}

      	if(pid > 0){
		//parent block
		//writing initial values to shared memory
		//data->val1 = 1;
	    	//data->val2 = 2;
	    	//data->winnerId = -1;
              
	    	while ((waitpid(-1, 0, 0)) != -1);
	}
        if (pid == 0){
		//child block
		//printf("%d - Executing child\n",getpid());
		
		while(1){
			data->number_of_cycles++;
			struct timeval t1;
			gettimeofday(&t1, NULL);
			lock(data);
			struct timeval t2;
			gettimeofday(&t2, NULL);
			long diff = t2.tv_usec - t1.tv_usec;
			printf("child diff: %ld\n",diff);
			data->total_child_time +=diff;


			int total = data->val1 + data->val2;
			//sleep(1);
		       	if(data->winnerId != -1){
				//winnder pid already found, so exiting the current child process.
				unlock(data);
				exit(0);
			}

			//printf("%d - Calculating Sum\t Value 1: %d\t Value 2: %d\t Total: %d\n", getpid(), data->val1, data->val2, total);

			if (total > max_value && data->winnerId == -1){
                                //winner pid found and putting it in shared memory
                                data->winnerId = getpid();
                                unlock(data);
                                exit(0);
                        }


			if (data->val1 < data->val2)
				data->val1 = total;
	               	else
                		data->val2 = total;
			
			unlock(data);
		}
	}
	printf("\n*** Winner PID: %d, value 1: %d, value 2: %d, Total: %d, max value: %d ***\n\n", data->winnerId, data->val1, data->val2, data->val1 + data->val2, max_value);
	delete_shm_mem();
	struct timeval end_time;
	gettimeofday(&end_time, NULL);
	long elapsed = end_time.tv_usec - start_time.tv_usec;
	printf("Parent Total Execution Time: %ld\n", elapsed);
	printf("Children Total Time: %ld, cycles: %ld", data-> total_child_time, data->number_of_cycles);
	long avg = data->total_child_time / data->number_of_cycles;
	printf("Children Average Lock Aquired Time: %ld\n", avg);
	delete_shm_mem();
	return 0;	
}

/*
 * Function:  attach_shm_mem
 * -----------------------------------
 *  Helper function to create, attach and retreive data from shared memory using the key.
 *
 *  returns: pointer to the shared memory.
 */
struct GameVariables *attach_shm_mem(){
	struct GameVariables *data;
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
        if (data == (struct GameVariables *)(-1)) {
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
 * Function:	lock
 * -----------------------------------
 *  Helper function to DOWN the semaphore value.
 *
 *  data: GameVariables struct contains values, winnerPID and semaphore value.
 *
 */

void lock(struct GameVariables *data){
        while (sem_trywait(&data->sem) < 0 ) {
                if( errno == EAGAIN)
                        continue;
                else perror("sem_trywait failed:");
        }
        //printf("%d - Lock aquired\n", getpid());
}

/*
 * Function	unlock
 * -----------------------------------
 *  Helper function to UP the semaphore value.
 *
 *  data: GameVariables struct contains values, winnerPID and semaphore value.
 *
 */
void unlock(struct GameVariables *data){
        sem_post(&data->sem);
        //printf("%d - Lock released\n", getpid());
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
