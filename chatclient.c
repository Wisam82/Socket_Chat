/****************************************************************************************
Program Name: chatclient.c
Author Name: Wisam Thalij
Date Created: 10-28-2015
Last Day Modified: 10-28-2015
Description: This program works as a client. The progran will require the user to enter
			a host name and a port number from the command line when running the program,
			then it will ask for a user name to display as a prompt. The user then will be
			able to send a message to the server.
			If the user wishes to end the session s/he should enter "\quit" to end the 
			connection and quit the program.
Extra:
	1- The server is a multi thread server, it will accept up to 5 connection at the
		same time. So you can run this program from a different window up to five runs
		and make up to 5 connections		
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h> 

// This function is called when a system call fails 
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{

	//Initialize the socket struct
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    const char *hostname = "localhost";

    // Set up some variables for the program
    // HostName		: to store the hostname from command line
    // MessageBuffer: to store the user entery for a message
    // MessageSent	: to store the final constructed message (handle + message) 
    // handle		: to store the user name and set it a prompt
    // quit			: to store the deconstructed message to check if "\quit" entered
    // MessageLn	: to store the messages lenghts
    // status		: varibale to set flag to continue a session between client and server
    // i 			: index for loop
    char HostName [40];
    char MessageSent[500];
    char MessageBuffer[500];
    char handle[40];
    char *quit = (char*) malloc(4);
    size_t MessageLn;
    int status = 0;
    int i;

    // Check for Usage validation
    if (argc < 3) {
       fprintf(stderr,"usage %s Hostname port\n", argv[0]);
       exit(0);
    }

    // Get the host name and the port from the command line
    sprintf(HostName,"%s",argv[1]);
    portno = atoi(argv[2]);

    // Get the user name to set as a prompt
	printf("Please nter your name (one-word): ");
	fgets(handle, 40, stdin);
	size_t ln = strlen(handle) - 1;
	if (handle[ln] == '\n')
			handle[ln] = '\0';

	strcat(handle, " >>");

	// Creatig the socket  
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Get the address of the host
    server = gethostbyname(HostName);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    // Initlize the serv_adder
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to the server
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // Send the start message to the server
    n = write(sockfd,"PORTNUM",7);
    if (n < 0) 
         error("ERROR writing to socket");

    // While there is a connection keep sending and receiving messages
    while(status == 0) {

    	// display handle and get the user message
    	printf("%s ", handle);
    	bzero(MessageBuffer,500);
	    fgets(MessageBuffer, 500, stdin);

	    MessageLn = strlen(MessageBuffer) - 1;
		if (MessageBuffer[MessageLn] == '\n')
    			MessageBuffer[MessageLn] = '\0';

    	// Construct the message + handle to be sent to server
    	bzero(MessageSent,500);
    	strcat(MessageSent, handle);
    	strcat(MessageSent, " ");
    	strcat(MessageSent, MessageBuffer);

    	// send the message to the server
    	n = write(sockfd,MessageSent,500);
    	if (n < 0) 
	         error("ERROR writing to socket");

	    // Check if the user sent a "quit" message to exit connection
	    MessageLn = strlen(MessageSent);
		for (i=0; i< MessageLn ; i++)
		{
			if (MessageSent[i] == '>')
				break;
		}
		i = i + 3;
		bzero(quit,5);
		strncpy(quit, MessageSent + i , 5);
		if (strcmp(quit,"\\quit") == 0)
		{
			printf ("Client sent a quit message! will quit now!\n");
			status = 1;
			break;
		}

		// If the message is not "quit" then continue the connection
	    bzero(MessageBuffer,500);
	    bzero(quit,5);
	    n = read(sockfd,MessageBuffer,500);
	    if (n < 0)
			error("ERROR reading from socket");

		// Check if the server sent a "quit" message to end connection
		strncpy(quit, MessageBuffer + 10 , 5);

		if (strcmp(quit,"\\quit") == 0)
		{
			printf ("Server Sent a quit message! will quit now!\n");
			status = 1;
			break;
		}
		// if not display the message
    	printf ("%s\n",MessageBuffer);
    }
    
   	// freeing the memory from quit allocation
    free(quit);
	   
    // Close the socket / end of the request
    close(sockfd);
    
    return 0;
}