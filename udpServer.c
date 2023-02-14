#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv){
	//Check arguments count
	if(argc != 3){
		printf("Usage: %s IP port\n", argv[0]);
		exit(0);
	}

	
	struct sockaddr_in serverAddr, clientAddr;
	int bufferLen = 1024;
	char buffer[bufferLen];
	int socketFD;
	socklen_t addr_size;

	//Create socket
	socketFD = socket(AF_INET, SOCK_DGRAM, 0);
	
	//Assign server address
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(atoi(argv[2]));

	//Bind the server
	bind(socketFD, (struct sockaddr*) &serverAddr, sizeof(serverAddr));  
  
	//Receive buffer from the client
	addr_size = sizeof(clientAddr);
	recvfrom(socketFD, buffer, bufferLen, 0, (struct sockaddr*) &clientAddr, &addr_size);
	printf("Message received: %s\n", buffer);

	//Send buffer tos the client
	sendto(socketFD, buffer, bufferLen, 0,    (struct sockaddr *)&clientAddr, addr_size);
	printf("Message sent: %s\n", buffer);

	close(socketFD);
	return 0;
}