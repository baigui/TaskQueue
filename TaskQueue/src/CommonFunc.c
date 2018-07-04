/**
 */

#include "CommonFunc.h"

#if defined(_WIN32) || defined(_WIN64) || defined(_CONSOLE)
#include <WinSock2.h>
#include <windows.h>
#include <stdint.h>

#pragma comment(lib,"ws2_32.lib")
#else

#include <time.h>
#include <sys/time.h>
#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#ifdef __ANDROID__
#include <sys/select.h>
#else

#include <execinfo.h>

#endif

#endif

int sleepSocketCount = 0;

#if defined(_WIN32) || defined(_WIN64) || defined(_CONSOLE)
SOCKET sleepSocket;
#else
int sleepSocket;
#endif
fd_set sleepSocketFds;

void initSleepFunc() {
	if (sleepSocketCount == 0) {
#if defined(WIN32) || defined(WIN64) || defined(_CONSOLE)
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		WSAStartup(sockVersion, &wsaData);
#endif
		sleepSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		FD_ZERO(&sleepSocketFds);
		FD_SET(sleepSocket, &sleepSocketFds);
	}
	sleepSocketCount++;
}

void deinitSleepFunc() {
	sleepSocketCount--;
	if (sleepSocketCount == 0) {
#if defined(WIN32) || defined(WIN64) || defined(_CONSOLE)
		closesocket(sleepSocket);
#else
		close(sleepSocket);
#endif
	}
}

int64_t getCurrentTimeUsec() {
#if defined(WIN32) || defined(WIN64) || defined(_CONSOLE)
	LARGE_INTEGER  large_interger;
	QueryPerformanceFrequency(&large_interger);
	double freq = large_interger.QuadPart;
	QueryPerformanceCounter(&large_interger);
	return (int64_t)(large_interger.QuadPart * 1000000.0 / freq);
#elif defined(__ANDROID__)
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (int64_t) now.tv_sec * 1000000LL + now.tv_nsec / 1000;
#else
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return (int64_t) (tm.tv_sec * 1000000 + tm.tv_usec);
#endif
}

void sleepUs(int64_t us) {
	if (sleepSocketCount == 0) return;
	struct timeval tv;
	fd_set dummy = sleepSocketFds;
	tv.tv_sec = us / 1000000L;
	tv.tv_usec = (int) (us % 1000000L);
	select(0, 0, 0, &dummy, &tv);     //这里就是简单的阻塞。函数停止运行罢了。
}

void printStackTrace() {
#if defined(WIN32) || defined(WIN64) || defined(_CONSOLE)

#else
#ifndef __ANDROID__
	printf("\r\n");
	int size = 16;
	void *array[16];
	int stack_num = backtrace(array, size);
	char **stacktrace = backtrace_symbols(array, stack_num);
	for (int i = 0; i < stack_num; ++i) {
		printf("%s\n", stacktrace[i]);
	}
	free(stacktrace);
	printf("\n\n");
#endif
#endif
}
