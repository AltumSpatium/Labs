#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
	const char* errorMsg = "Error opening temp file!\n";
	int tmpFile = open("/tmp/bytesTmp", O_CREAT | O_RDWR, 0666);
	char buf[1000];
	size_t bufLen = 0;
	size_t readLen;

	if (tmpFile == -1) {
		write(STDERR_FILENO, errorMsg, strlen(errorMsg));
		return 1;
	}

	while(read(STDIN_FILENO, buf, 1)) {
		write(tmpFile, buf, 1);
		bufLen++;
	}

	while (bufLen > 0) {
		readLen = bufLen > sizeof(buf) ? sizeof(buf) : bufLen;
		bufLen -= readLen;

		lseek(tmpFile, bufLen, SEEK_SET);
		read(tmpFile, buf, readLen);

		while (readLen-- > 0) 
			write(STDOUT_FILENO, buf + readLen, 1);
	}

	close(tmpFile);
	remove("/tmp/bytesTmp");
}

