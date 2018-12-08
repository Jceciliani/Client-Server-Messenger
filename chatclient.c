/* Author: Josh Ceciliani
 * Title: Project 1
 * Date: 10/24/2018
 * Description: This is the client of the client/server connetion.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>


// Error messages
void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int socketFD;
	int portNumber;
	int charsWritten;
	int charsRead;
	int messageLength;
	int quit1 = 5;
	char message[500]; // max 500 characters
	char completeMessage[512]; // Includes 500 for message, 10 for username and  2 for '> '
	char username[10]; // max 10 characters
	char buffer[512]; // Includes 500 for message, 10 for username, and 2 for '> '
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	
	if(argc < 3)
	{
		fprintf(stderr, "USAGE: %s hostname port'n", argv[0]);
		exit(0);
	}

	// Set up server address
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear address struct
	portNumber = atoi(argv[2]); // Get port number, convert from string to int
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname(argv[1]); // Get host name from arguments 
	
	if(serverHostInfo == NULL)
	{
		fprintf(stderr, "CLIENT: ERROR, no such host\n");
		exit(0);
	}

	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	// Error test socket
	if(socketFD < 0)
	{
		error("CLIENT: ERROR opening socket");
	}
	
	// Connect to the Server
	if(connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) //Connect socket to address
	{
		error("CLIENT: ERROR connecting");
	}	

	// Memset all character arrays
	memset(username, '\0', sizeof(username));
	memset(message, '\0', sizeof(message)); 
	memset(completeMessage, '\0', sizeof(completeMessage));
	// Get username for handlebar	
	printf("Enter your username: ");
	fgets(username, sizeof(username) - 1, stdin); // Get input from client user
	username[strcspn(username, "\n")] = '\0';
	// Error if length is more than 10 characters
	if(strlen(username) > 10)
	{
		fprintf(stderr, "Usernames must be under 10 characters\n");
		exit(0);
	}	

	// Get message from user to send to server
	printf("%s> ", username); // Print username handle
	fgets(message, sizeof(message) - 1, stdin); // Get input from client user
	message[strcspn(message, "\n")] = '\0'; // Take off newline that fgets adds
	//Error if length is more than 500 characters
	if(strlen(message) > 500)
	{
		fprintf(stderr, "Message must be under 500 characters\n");
		exit(0);
	}	

	// Combine username and message together with > for handlebar
	strcat(completeMessage, username);
	strcat(completeMessage, "> ");
	strcat(completeMessage, message);
	messageLength = strlen(completeMessage);

	// Send message to server
	charsWritten = send(socketFD, completeMessage, strlen(completeMessage), 0);
	// Error for sending
	if(charsWritten < 0)
	{
		error("CLIENT: ERROR writing to socket");
	}
	// Error - complete message not sent
	if(charsWritten != messageLength)
	{
		error("CLIENT: ERROR not all chars written");
	}

	// Get return message from server and maintain connection
	while(1)
	{
		// Memset buffer to clear
		memset(buffer, '\0', sizeof(buffer));
		// Message back from server
		charsRead = recv(socketFD, buffer, sizeof(buffer), 0);		
		// Error for receiving	
		if(charsRead < 0)
		{
			error("CLIENT: ERROR reading from socket");
		}	
		// Check if server quit the connection	
		if(strstr(buffer, "\\quit") != 0)
		{	
			printf("The server has ended the connection\n");
			break;
		}
		else
		{
			printf("%s\n", buffer);
		}
		
		// Memset message and completeMessage to be used again
		memset(message, '\0', sizeof(message)); 
		memset(completeMessage, '\0', sizeof(completeMessage));
		
					
		// Get message from user to send to server
		printf("%s> ", username);
		fgets(message, sizeof(message) - 1, stdin); // Get input from client user
		message[strcspn(message, "\n")] = '\0'; // Take off newline that fgets adds
		//Error if length is more than 500 characters
		if(strlen(message) > 500)
		{
			fprintf(stderr, "Message must be under 500 characters\n");
			exit(0);
		}	

		// Check if client quit
		quit1 = strcmp(message, "\\quit");
		// Combine username and message together with > for handlebar
		strcat(completeMessage, username);
		strcat(completeMessage, "> ");
		strcat(completeMessage, message);
		messageLength = strlen(completeMessage);
		// If client quit 
		if(quit1 == 0)
		{
			
			// Send message to server
			charsWritten = send(socketFD, completeMessage, messageLength, 0);
			// Error for sending
			if(charsWritten < 0)
			{
				error("CLIENT: ERROR writing to socket");
			}
			// Error - complete message not sent
			if(charsWritten != messageLength)
			{
				error("CLIENT: ERROR not all chars written");
			}

			break;
		}
		else
		{
			// Send message to server
			charsWritten = send(socketFD, completeMessage, messageLength, 0);
			// Error for sending
			if(charsWritten < 0)
			{
				error("CLIENT: ERROR writing to socket");
			}
			// Error - complete message not sent
			if(charsWritten != messageLength)
			{
				error("CLIENT: ERROR not all chars written");
			}	
		}
	
	}

	// Print notice to screen
	if(quit1 == 0)
	{
		printf("The client has ended the connection\n");
	}
	// Close the socket
	close(socketFD);
	return 0;
}


