CS372 INTRO TO COMPUTER NETWORK
Project#1
Wisam Thalij


File Description: Client program writtien in C and server program written in C++

chatclient.c

	This program works as a client. The progran will require the user to enter
a (hostname) and a (port number) from the command line when running the program,
then it will ask for a user name to display as a prompt. The user then will be
able to send a message to the server.
If the user wishes to end the session s/he should enter "\quit" to end the 
connection and quit the program.


chatserver.cpp

	This program works as a Server. The progran will require the user to enter
a (port number) from the command line to wait on, then the program run and wait
for a connection.
once a connection made the program will start sending and receiving message 
with the client.
If the program receives a "\quit" message from the client, it will close that
connection and wait for a nother connection.
If the server wishes to close the connection, the user will have to enter "\quit" and 
send it to the client. the program will close the that connection and go back to wait
for another connection.

To stop the server from runing, user will have to terminate it by (ctrl + C) from command
line to send a SIGINT signal.

Extra Credit:

	The server is a multi-thread server, it will accept up to 5 connection at the same time.
If you wish you can open another window and run the chatclient on the same port, and start chating with the server.


For Compiling ( command line ):

g++ -o chatserver chatserver.cpp
gcc -o chatclient chatclient.c 

To Run the program ( command line ):

 ./chatclient flip1.engr.oregonstate.edu 30020
 ./chatserver 30020

 Testing:
 	I uploaded the two programs to the engineering server and accessed the two filp servers from the command line in two windows:

ssh thalijw@flip1.engr.oregonstate.edu   to run the server
shh thalijw@flip2.engr.oregonstate.edu	 to run the client

I was being able to send and recieve messages between both client and server on both flips.


