#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>


int CHUNK_SIZE = 16;
int CHUNK_COUNT = 5;
char* SHARED_MEMORY_OBJECT_NAME = "/copy_shm";
char* SEMAPHORE_NAME = "/copy_sem";
char* READ_SEMAPHORE_NAME = "/copy_read_sem";
char* WRITE_SEMAPHORE_NAME = "/copy_write_sem";
char* SOURCE_NAME="/home/smart/lab08/source.txt";
char* DEST_NAME="/home/smart/lab08/output.txt";


int create_daemon(void (*deamon_function)());
void reader();
void writer();


int main(int argc, char *argv[]) {
	if (argc == 3) {
		SOURCE_NAME = argv[1];
		DEST_NAME = argv[2];
	}
	int shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT | O_RDWR, 0777);
	ftruncate(shm, CHUNK_SIZE + 1);
	sem_t *read_sem = sem_open(READ_SEMAPHORE_NAME, O_CREAT, 0777, 0);
	sem_t *write_sem = sem_open(WRITE_SEMAPHORE_NAME, O_CREAT, 0777, 0);
	create_daemon(reader);
	create_daemon(writer);
	return 0;
}


int create_daemon(void (*deamon_function)()) {
    int pid = fork();
    if (pid == -1) {
        printf("Error: Daemon start is failed.");
        return -1;
    }
    else if (!pid) {
        umask(0);
        setsid();
        chdir("/");
        close(STDIN_FILENO);
        //close(STDOUT_FILENO);
        close(STDERR_FILENO);
        deamon_function();
    }
    else {
        return 0;
    }
}


void reader() {
	int shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_RDWR, 0777);
	char* shm_pointer = mmap(0, CHUNK_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
	sem_t *read_sem = sem_open(READ_SEMAPHORE_NAME, 0);
	sem_t *write_sem = sem_open(WRITE_SEMAPHORE_NAME, 0);
	FILE* source_file = fopen(SOURCE_NAME, "r");
	int length = 0;
	
	printf("Reading...\n");
	while ((length = read(fileno(source_file), shm_pointer+4, CHUNK_SIZE-4)) > 0) {
		((int*)shm_pointer)[0] = length;
		printf("Reading done (%d).\n", length);
		sem_post(read_sem);
		printf("Reading semaphore opened.\n");
		printf("Waiting for writer...\n");
		sem_wait(write_sem);
		printf("Reading...\n");
	}
	((int*)shm_pointer)[0] = 0;
	sem_post(read_sem);
	printf("Reader closed.\n");
	
	fclose(source_file);	
	sem_close(write_sem);
	sem_close(read_sem);
	munmap(shm_pointer, CHUNK_SIZE);
    close(shm);
	exit(0);
}


void writer() {
	int shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_RDWR, 0777);
	char* shm_pointer = mmap(0, CHUNK_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
	sem_t *read_sem = sem_open(READ_SEMAPHORE_NAME, 0);
	sem_t *write_sem = sem_open(WRITE_SEMAPHORE_NAME, 0);
	FILE* dest_file = fopen(DEST_NAME, "w");
	int length = 0;
	
	do {
		printf("Waiting for reader...\n");
		sem_wait(read_sem);
		printf("Writing...\n");
		length = ((int*)shm_pointer)[0];
		write(fileno(dest_file), shm_pointer+4, length);
		printf("Writing done (%d).\n", length);
		sem_post(write_sem);
		printf("Writing semaphore opened.\n");
	} while(length);
	printf("Writer closed.\n");
	
	
	fclose(dest_file);
	sem_close(write_sem);
	sem_close(read_sem);
	munmap(shm_pointer, CHUNK_SIZE);
    close(shm);
	sem_unlink(WRITE_SEMAPHORE_NAME);
	sem_unlink(READ_SEMAPHORE_NAME);
	shm_unlink(SHARED_MEMORY_OBJECT_NAME);
	exit(0);
}
