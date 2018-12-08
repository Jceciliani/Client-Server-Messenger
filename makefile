
chatclient: chatclient.o
	gcc -o chatclient chatclient.c

clear:
	rm *.o chatclient

