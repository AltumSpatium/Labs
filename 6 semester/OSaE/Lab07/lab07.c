#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>


char* FIFO_DIR = "/tmp/passfifo";
int file_exists(char* file_name);
int server();
int start_server();
void generate_password(char* , int );
int client();


int main(int argc, char *argv[]) {
	if (file_exists(FIFO_DIR)) {
		return client();
	} else {
		return server();
	}
}


int file_exists(char* file_name) {
	return access(file_name, F_OK) != -1;
}


int server() {
	int pid = fork();
    if (pid==0) {
        umask(0);
        setsid();
        chdir("/");
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        return start_server();
    }
	return 0;
}


int start_server() {
	char password[16];
	
	struct timespec timeout;
	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;
	sigset_t sigset;
	siginfo_t siginfo;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGPIPE);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	
	mkfifo(FIFO_DIR, 0666);
	int pass_fifo = open(FIFO_DIR, O_WRONLY);

	while (1) {
		generate_password(&password[0], 16);
		write(pass_fifo, password, 16);

		sigtimedwait(&sigset, &siginfo, &timeout);

		if (siginfo.si_signo == SIGPIPE) {
			sigtimedwait(&sigset, &siginfo, &timeout);
		}

		if (siginfo.si_signo == SIGINT) {
			close(pass_fifo);
			unlink(FIFO_DIR);
			return 0;
		}
	}
}


void generate_password(char* buffer, int length){
	srand(time(NULL));
	int i;
	for (i=0; i<length; i++) {
		switch(rand() % 3) {
			case 0:
				buffer[i] = (char)((rand() % 26) + 65);
				break;
			case 1:
				buffer[i] = (char)((rand() % 26) + 97);
				break;
			default:
				buffer[i] = (char)((rand() % 10) + 48);
		}
	}
}


int client() {
	char buffer[16];
	int pass_fifo = open(FIFO_DIR, O_RDONLY);
    read(pass_fifo, buffer, 16);
    printf("%s", buffer);
    close(pass_fifo);
	return 0;
}
