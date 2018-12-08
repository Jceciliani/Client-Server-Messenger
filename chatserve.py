# Author: Josh Ceciliani
# Title: Project 1
# Date: 10/24/2018
# Description: This is the client of the client/server connetion.

import socket
import sys
# Function to get a new connection after the server or client quits
def getNewClient():
	global connection
	global sock
	connection.close()
	connection, addr = sock.accept()
# Function to keep connection open
def startConnection(conn):
	global connection
	global sock
	connection = conn

	while 1:
		# Get message from client after connection
		message = connection.recv(512).decode()
		# Print message to screen
		print (message)
		# Find if the client quit the connection
		clientQuit = message.find("\quit")
		#print (clientQuit)
		# Check if client quit the connection
		if clientQuit != -1:
			print("The client has ended the connection")
			getNewClient()
			continue
		# Wait for user input
		print (username + "> ", end="")
		serverMessage = input()
		# Takes off newline from the message
		serverMessage = serverMessage.rstrip('\r\n')
		# Sending the message to the client
		connection.send((username + "> " + serverMessage).encode())
		# Check if the server quit the connection
		if serverMessage == "\quit":
			print("The server has ended the connection")
			getNewClient()
			continue


# Start Code **
# Checker for exactly 2 arguments
if len(sys.argv) != 2:
	sys.exit("SERVER: ERROR please enter two arguments\n")

# Create the socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Get correct port from agruments
port = int(sys.argv[1])
# Bind socket to port
try:
	sock.bind(('', port))
except socket.error as msg:
	print ('SERVER: Bind failed')
	sys.exit()

# Make socket listen for connections - currently set to 1 connection
sock.listen(1)
# Waiting to accept the connection
connection, addr = sock.accept()
# Hardcoded handle for server
username = ("Server")	
# Keep connection open with client
startConnection(connection)

# References 
# https://www.binarytides.com/python-socket-server-code-example/
# https://docs.python.org/release/2.6.5/library/socketserver.html
