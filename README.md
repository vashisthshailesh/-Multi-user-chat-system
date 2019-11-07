Description of your code and how you implemented the function – the logical and implementation details


Used AF_UNIX socket family as it used to communicate between the process on the same machine efficiently.
Since there was a need to deliver multiple packets through the socket and since we are communicating large chunks of messages usage of TCP - SOCK_STREAM is the best for this scenario
And the ​Protocol value for Internet Protocol(IP), is 0.
Hence now we have all the parameters to generate a socket
A UNIX domain socket is represented in the following manner:-
struct sockaddr_un {
         sa_family_t sun_family;
         char        sun_path[108];
            }
/* AF_UNIX */
/* Pathname */
Instead of identifying a server by an IP address and port, a UNIX domain socket is known by a pathname. Obviously the client and server have to agree on the pathname for them to find each other. The server binds the pathname to the socket:
Hence for this project let us the path be "/tmp/foo"
Clearing any prior data which present int the soackaddr_un object using memset Equating sun_family to AF_UNIX and path to the above-mentioned path.
Now we have to unlink any prior connection to the given address as when we close the ./server file the socket is not closed and is still linked to the given address and hence if we run the file again without unlinking it will give
bind error: Address already in use
The following error
Hence:- unlink(socket_path);
All that left is now to bind the given socket to the path. Which is done by the bind() function.
Similarly, the sockets are created for clients and are given the same path as that of the server to complete the communication the inputs is implemented via thread.



Description of how to compile and test the program
Run the make file after which Run
./server
./client
Making 3 clients would be sufficient to check the entire scenario. First, give a personal message between any2 clients that message won’t show up in the other client and when the message is sent to all the clients it will be in all the client’s windows.


The inputs the user should give
Each client sends the message in the following format: “<id>/<message>” where <id> equals “-1” for a group message to all and the specific id of the person for an individual message. Each client also receives messages in the following format “<id>>/<message>” where <id> represents the user if of the sender.




Errors Handle
1. If the user enters an invalid sending mode : Eg: “oo/Hey All!”
Since ‘oo’ is not a valid sending mode/id, it would display an error message to the
sending user.
2.If the id entered by user does not exist:
Eg: “19/hi!”
However, if id 19 does not exist, it would display an error message to the sending user!
3. If there is binding error it would show if it priorly linked it would show :-

Bind failed: Address already in use
4. If the path mentioned is not correct it would show the following error:- bind error: No such file or directory
5. And if there was an error in connecting the socket then it will show:- connect error: No such file or directory




Expected output (and how to interpret it).
./server
If it shows : - “Waiting for connections ...”
Then this means the server is connected successfully and it is waiting for the clients. If any error shows then refer to the handled errors.
./client
If it shows this:-
Connected.
>> Welcome to my chat app!
>> New user added with user id :0
Then it means it has successfully connected with the server and it has been given an id of 0.
Now we can start chatting in the way it is mentioned in the - the inputs user should give.
