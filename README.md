# UDP
Implement UDP with exponential backoff algorithm by C


# Step 1
Open two terminals


# Step 2 - Compile
$ gcc udpServer.c -o udpServer

$ gcc udpClient.c -o udpClient -lm


# Step 3 - Run 
$ ./udpServer 127.0.0.1 3456 

IP Port

$ ./udpClient 1234 myInfo 10

IP Port Message [maxRetry]
