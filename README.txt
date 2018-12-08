How to run Project 1:

1) Unzip the zipped folders (File include: chatclient.c, chatserve.py, makefile, and README.txt
2) Place the folder contents into the flip using a file transfer application (i.e. FileZilla)
3) Login to flip, either with access.engr, or flip1, flip2 or flip3
4) Go to the location you place the files
5) Type "make" into the command line to create the executable of the chatclient.c file
6) Start the server: type "python3 clientserve.py [port number]" into the command line. I would recommend any number from 15000-16000. NOTE: Do this in the first instance of putty.
7) Start the client: type "chatclient localhost [port number]" into the command line, where port number is the same as the number you typed in for the server. NOTE: Do this in the second instance of putty.
8) Type in your username into the client side, and then type your message. The connection will continue to work until either the server or client types "\quit". When this happens, the server will remain open until another client connects. 
9) If you wish to close down the server, press ctrl C.
