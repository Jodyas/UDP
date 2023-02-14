#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void printCurTime()
{
	//Exponential Backoff - Calculate current time
	char time_string[15];
	struct timeval curTime;
	struct tm* ptm;	

	//Calculate current time - Get the current time
	gettimeofday(&curTime, NULL);
	ptm = localtime(&curTime.tv_sec);
	
	//Calculate current time - Format the current time
	strftime(time_string, sizeof(time_string), "%H:%M:%S", ptm);

	//Calculate current time - Print the current time with milliseconds
	printf("Timestamp of sending message: %s.%03ld\n", time_string, curTime.tv_usec/1000);	
}

int main(int argc, char *argv[]){
	
	//Check arguments count
	if (argc != 4 && argc != 5)
	{
		printf("Usage: %s IP port message [maxRetry_optional]\n", argv[0]);	
		exit(0);
	}

	int socketFD;
	struct sockaddr_in serverAddr;
	int bufferLen = 1024;
	char buffer[bufferLen];
	socklen_t addr_size;

	//Create socket
	socketFD = socket(AF_INET, SOCK_DGRAM, 0);

	//Assign server address
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(atoi(argv[2]));

	//clear and assign buffer
	memset(buffer, '\0', sizeof(buffer));
	strcpy(buffer, argv[3]);
	buffer[strlen(argv[3])] = '\0';

	//Send buffer to the server
	addr_size = sizeof(serverAddr); 
	ssize_t sendResult = sendto(socketFD, buffer, bufferLen, 0, (struct sockaddr*)&serverAddr, addr_size);				
	printCurTime();
	
	//Exponential Backoff - Calculate time interval
	int base = 500;
	int multiplier = 2;
	int maxWaitInterval = 8000;
	int waitInterval = base;
	struct timespec timeSpec;
	
	//Exponential Backoff - Calculate count of retry
	int countRetry = 0;
	int maxRetry = atoi(argv[4]);
	
	//Exponential Backoff - Algorithm
	while (sendResult < 0 && countRetry < maxRetry && waitInterval <= maxWaitInterval)
	{
		//Calculate time interval and sleep
		timeSpec.tv_sec = waitInterval / 1000;
		timeSpec.tv_nsec = (waitInterval % 1000) * 1000000;
		nanosleep(&timeSpec, NULL);		
		//Send buffer to the server
		ssize_t sendResult = sendto(socketFD, buffer, bufferLen, 0, (struct sockaddr*)&serverAddr, addr_size);				
		
		//Update the variables for next time retry, if needed
		countRetry++;
		waitInterval = base * pow(multiplier, countRetry);

		printCurTime();
	}
		
	//Check whether fail sending
	if ((sendResult < 0) && (countRetry >= maxRetry || waitInterval > maxWaitInterval))
	{
		printf("Failed: sending the message after %u time retry\n", countRetry);
		return 1;
	}	
	
	//Received from the server
	ssize_t recvResult = recvfrom(socketFD, buffer, bufferLen, 0, (struct sockaddr*) &serverAddr, &addr_size);
	printf("Message received: %s\n", buffer);

	close(socketFD);
	return 0;
}
