# UDP
UDP-C-Implementation

# Step 1
Open two terminals

# Step 1 - Compile
$ gcc udpServer.c -o udpServer

$ gcc udpClient.c -o udpClient -lm


# Step 2 - Run 
$ ./udpServer 127.0.0.1 3456 

IP Port

$ ./udpClient 1234 myInfo 10

IP Port Message [maxRetry]
