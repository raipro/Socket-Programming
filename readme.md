to compile run gcc client.c and gcc server.c in different directories.
server.c executable should be started first followed by client.

on exiting client, server will resume waiting for another client.

general description:
On the server side, we first open up a new socket and then bind it to port 8080. Now we use listen and accept to wait for a connection.The client opens a socket of its own and connects to the server.
Now once the connection is done,two commands are implemented get and exit.exit or ctrl C on the client will exit the client But the server can only be exited by ctrl c on the server. We take in file names through get command and send them to server to find out if the file exists or not. if not we print invalid files,if yes then we read it line by line from the server and write after opening the file on the client side.Acknowledgements have also been sent to ensure no desynchoronization(like "yes/no" "size received" ,etc.).
