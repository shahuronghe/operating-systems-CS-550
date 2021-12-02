#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(){
	int fd = open("/dev/simple_misc", O_RDWR);
	if(fd < 0){
		perror("Unable to open the device file\n");
		return -1;
	}

	char *head = malloc(sizeof(char) * 16);
	read(fd, head, sizeof(head) * 1024);
	printf("data from kernel: %s\n", head);

	close(fd);
	return 0;
}
