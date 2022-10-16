#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main(void) {
	
	pid_t pid;
	int fd1[2];
	int fd2[2];

	if(pipe(fd1) == -1) {
		fprintf(stderr, "PipeA");
	}
	
	pid = fork();
	
	if (pid < 0) {
		fprintf(stderr, "ForkA");
	}
	
	if (pid == 0) {
		//Child A
		dup2(fd1[WRITE_END], WRITE_END);
		close(fd1[READ_END]);
		execlp("sort", "sort", "./Names.txt",  NULL);
	}

	if(pipe(fd2) == -1) {
		fprintf(stderr, "PipeB");
	}

	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "ForkB");
	}
	if (pid == 0) {
		dup2(fd1[READ_END], READ_END);
		dup2(fd2[WRITE_END], WRITE_END);
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);
		execlp("uniq", "uniq", NULL);
	}

	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "ForkC");
	}
	if (pid == 0) {
		
		dup2(fd2[READ_END], READ_END);
		close(fd2[WRITE_END]);
		close(fd1[READ_END]);
		close(fd1[WRITE_END]);
		execlp("wc", "wc", "-l", NULL);
	}
	
	close(fd1[READ_END]);
	close(fd1[WRITE_END]);
	close(fd2[READ_END]);
	close(fd2[WRITE_END]);

	waitpid(-1, NULL, 0);

	
	return 0;
}


















