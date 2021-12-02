#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

/**
 * defined structure to store process related data.
 */
struct process_data {
        int pid;
        int ppid;
        int cpu;
        long state;
};

/*
 * Function:  parse_state
 * -----------------------------------
 *  Helper function to get state name depending upon the state value.
 *
 *  state: the value of state as long
 *
 *  returns: the desired state name
 */

char * parse_state(long state) {
        if (state == 0) { return "TASK_RUNNING"; }
        else if (state == 1) { return "TASK_INTERRUPTIBLE"; }
        else if (state == 2) { return "TASK_UNINTERRUPTIBLE"; }
        else if (state == 4) { return "TASK_STOPPED"; }
        else if (state == 8) { return "TASK_TRACED"; }
        else if (state == 16) { return "EXIT_DEAD"; }
        else if (state == 32) { return "EXIT_ZOMBIE"; }
        else if (state == 64) { return "TASK_DEAD"; }
        else if (state == 128) { return "TASK_WAKEKILL"; }
        else if (state == 256) { return "TASK_WAKING"; }
        else if (state == 512) { return "TASK_PARKED"; }
        else if (state == 1024) { return "TASK_NOLOAD"; }
        else if (state == 2048) { return "TASK_NEW"; }
        else if (state == 4096) { return "TASK_STATE_MAX"; }
        else if (state == (16 | 32)) { return "EXIT_ZOMBIE,EXIT_DEAD"; }
        else if (state == (128 | 2)) { return "TASK_WAKEKILL,TASK_UNINTERRUPTIBLE"; }
        else if (state == (128 | 4)) { return "TASK_WAKEKILL,TASK_STOPPED"; }
        else if (state == (128 | 8)) { return "TASK_WAKEKILL,TASK_TRACED"; }
        else if (state == (2 | 1024)) { return "TASK_UNINTERRUPTIBLE,TASK_NOLOAD"; }
        else if (state == (1 | 2)) { return "TASK_INTERRUPTIBLE,TASK_UNINTERRUPTIBLE"; }
        else if (state == (0 | 1 | 2 | 4 | 8 | 32 |16)) { return "TASK_RUNNING,TASK_INTERRUPTIBLE,TASK_UNINTERRUPTIBLE,TASK_STOPPED,TASK_TRACED,EXIT_ZOMBIE,EXIT_DEAD"; }
        else if (state == (1 | 2 | 4 | 8)) { return "TASK_NORMAL,TASK_STOPPED,TASK_TRACED"; }
        else { return "OTHER"; }

}

/**
 * main function to run the driver program which communicates with the kernel module to get the data by invoking read function.
 */
int main(){

	// file descriptor for the kernel module present in /dev
	int fd = open("/dev/process_list", O_RDWR);
	if(fd < 0){
		perror("Unable to open the device file\n");
		return -1;
	}

	// infinite loop to get live process states and cpu from kernel module 
	while(1){
		struct process_data data[1024];

		// read function to read process data from task struct and assigning only required values to process_data struct.
		int size = read(fd, data, sizeof(struct process_data) * 1024);
		if(size > 0){
			for(int i = 0; i < size; i++){
				struct process_data d = data[i];
				//if(d.pid == 0 && d.ppid == 0)
				//	break;
				printf("PID=%d \t PPID=%d \t CPU=%d \t STATE=%s\n", d.pid, d.ppid, d.cpu, parse_state(d.state));
			}
			printf("CTRL + C to exit\n");
			sleep(1);
		} else {
			printf("read error! exiting program\n");
			break;
		}
	}

	// closing file descriptor
	close(fd);
	return 0;
}
