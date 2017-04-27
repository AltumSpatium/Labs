#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

const int SIG_COUNT = 28;
const char* CONFIG_PATH = "/home/smart/Labs/Lab06/lab06.conf";
const char* LOG_FILE = "/home/smart/Labs/Lab06/lab06.log";

typedef struct signal_t {
	const char* name;
	int num;
} sigs;

sigs signals[28] = {
	{"HUP", 1},
	{"INT", 2},
	{"QUIT", 3},
	{"ILL", 4},
	{"TRAP", 5},
	{"ABRT", 6},
	{"FPE", 8},
	{"KILL", 9},
	{"BUS", 10},
	{"SEGV", 11},
	{"SYS", 12},
	{"PIPE", 13},
	{"ALRM", 14},
	{"TERM", 15},
	{"USR1", 16},
	{"USR2", 17},
	{"CHLD", 18},
	{"TSTP", 20},
	{"URG", 21},
	{"POLL", 22},
	{"STOP", 23},
	{"CONT", 25},
	{"TTIN", 26},
	{"TTOU", 27},
	{"VTALRM", 28},
	{"PROF", 29},
	{"XCPU", 30},
	{"XFSZ", 31}};

bool load_config(const char*);

int read_string(int hFile, char* buf, int maxLen) {
	int offset = 0;
	int len;

	while(1) {
		len = read(hFile, buf + offset, 1);
		if (len == -1)
			return -1;

		if (len > 0) {
			if (buf[offset] != '\n')
				offset += len;
			else len = 0;
		}
		else if (offset == 0) return 0;

		buf[offset] = '\0';

		if (len == 0)
			return offset + 1;
	}
}

char* itoc(int num) {
	static char str[15];
	sprintf(str, "%d", num);
	return str;
}

void writeLog(char* msg) {
	int logFile = open(LOG_FILE, O_APPEND | O_RDWR, 0666);
	if (logFile == -1) return;

	write(logFile, msg, strlen(msg));
	close(logFile);
}

void handler(int sig_num) {
	time_t currTime = time(NULL);
	char logMsg[1000];

	strcpy(logMsg, "Signal registered: Name: ");
	strcat(logMsg, sys_siglist[sig_num]);
	strcat(logMsg, ", code: ");
	strcat(logMsg, itoc(sig_num));
	strcat(logMsg, ", time: ");
	strcat(logMsg, ctime(&currTime));
	strcat(logMsg, "\n");

	writeLog(logMsg);

	if (sig_num == SIGHUP) {
		load_config(CONFIG_PATH);
	}
}

void reset_all_handlers() {
	int i;
	for (i = 0; i < SIG_COUNT; i++)
		signal(signals[i].num, SIG_DFL);
}



bool load_config(const char* path) {
	reset_all_handlers();

	int confFile = open(path, O_RDONLY);
	if (confFile == -1) return false;
	
	char buf[1000];
	int ret;
	int i;

	while((ret = read_string(confFile, buf, sizeof(buf))) > 0) {
		for (i = 0; i < SIG_COUNT; i++) {
			if (strcmp(buf, signals[i].name) == 0) {
				signal(signals[i].num, handler);
			}
		}
	}

	close(confFile);

	return ret == 0 ? true : false;
}

int create_daemon() {
	pid_t pid = fork();
	if (pid == -1) return -1;
	else if (pid != 0) return pid;

	if (chdir("/") == -1) return -1;

	return 0;
}

int main() {
	int daemon = create_daemon();
	if (daemon == -1) return 1;
	else if (daemon != 0) return 0;

	if (signal(SIGHUP, handler) == SIG_ERR) return 1;

	if (!load_config(CONFIG_PATH)) {
		writeLog("Error while loading config file!");
		return 1;
	}

	while (1) {}
	return 0;
}

