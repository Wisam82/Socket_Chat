/****************************************************************************************
Program Name: chatserver.cpp
Author Name: Wisam Thalij
Date Created: 10-28-2015
Last Day Modified: 10-28-2015
Description: This program works as a Server. The progran will require the user to enter
			a port number from the command line to wait on, then the program run and wait
			for a connection.
			once a connection made the program will start sending and receiving message 
			with the client.
			if the program receives a "\quit" message from the client, will close that
			connection and wait for a nother connection to be made.
			if the server wishes to cloes the connecting user have to enter "\quit" and 
			send it to the client. the program will close the connection and still wait
			for another connection.
Extra:
	1- The server is a multi-thread server, it will accept up to 5 connection at the
		same time.
*****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

// This function is called when a system call fails 
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main (int argc, char *argv[]) {

	// Create the socket
	int sockfd, newsockfd, portno;
    socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	// Set up some variables for the program
	// pid 			: Variable to store the process ID
	// handle		: to store the user name and set it a prompt
	// message 	 	: to store the user message sent by client
	// Input  		: to store the message entered by the server user
	// MessageSent	: to store the final constructed message (handle + Input)
	// n 			: Variable to store the results of read and write
	// i , status 	: indexes for loops
	pid_t pid;
	char handle [] = "Wisam >> ";
	char message[500];
	char Input[500];
	//string MessageSent;
	char MessageSent[500];
    char *quit = (char*) malloc(4);
    size_t MessageLn;
	int n;
	int i;
	int status = 0;
    
	// Check for Usage validation
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
	 	exit(1);
	}

	// Initialize the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// Bind the socket to the port
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	    error("ERROR on binding");

	// Listen to the clients up to 5 at the time
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	// Go to infinit while loop and wait for client requests
	while(1) {

		// Accept a connection and make new socket
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0) 
		  error("ERROR on accept");

		// Fork a new process to process this client request and go back to be availabe again
		pid = fork();

		if(pid < 0) {
			error("ERROR on fork");
		}

		if(pid == 0){

			// When in child process close the original socke
			close(sockfd);
			// wipe out the content of message
			bzero(message,500);

			// Get the initial message from client to start a connection
			n = recv(newsockfd,message,7,0); 
			if (n < 0)
				error("ERROR reading from socket");
			
			if (strcmp(message,"PORTNUM") == 0)
			{
				printf("We started a connection!\n");

			}
		    
		    // While there is a connection keep sending and receiving messages
		    while (status == 0)
		    {
		    	// empty the message and read the client message to it
		    	bzero(message,500);
		    	n = recv(newsockfd,message,500,0);
				if (n < 0)
					error("ERROR reading from socket");

				// Check if the client sent a "quit" message
  				MessageLn = strlen(message);
  				for (i=0; i< MessageLn ; i++)
  				{
  					if (message[i] == '>')
  						break;
  				}
  				i = i + 3;
  				bzero(quit,5);
  				strncpy(quit, message + i , 5);
					
				if (strcmp(quit,"\\quit") == 0)
				{
					printf ("Client sent a quit message!, close the connection and continue listening\n");
					status = 1;
					break;
				}
				// If message not a "quit" display it
		    	printf ("%s\n",message);

		    	// Construct a message to send to client
		    	printf("%s", handle);
		    	bzero(Input,500);
	    		fgets(Input, 500, stdin);
	   			MessageLn = strlen(Input) - 1;
				if (Input[MessageLn] == '\n')
		    			Input[MessageLn] = '\0';

		    	bzero(MessageSent,500);
		    	strcat(MessageSent, handle);
		    	strcat(MessageSent, " ");
		    	strcat(MessageSent, Input);

    			n = send(newsockfd,MessageSent,500,0);
    			if (n < 0) 
	         		error("ERROR writing to socket");
	   			
	   			// Check if the server sent a "quit" message
	         	bzero(quit,5);
  				strncpy(quit, MessageSent + 10 , 5);
  				if (strcmp(quit,"\\quit") == 0)
				{
					printf ("Server sent a quit message!,close the connection and continue listening\n");
					status = 1;
					break;
				}
		    }
			
		    // Free the quit string and exit the child process
		    free(quit);
			exit(0);	
			
		} else
			close(newsockfd);
	}

	close(sockfd);
	return 0;

}