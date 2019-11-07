#include <stdio.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define TRUE 1
#define FALSE 0

#define SOCK_PATH "echo_socket"

int testy  = 0;
int main(int argc , char *argv[])
{
    //int opt = TRUE;
    fd_set readfds;
    int max_sd;
    char *socket_path = "/tmp/foo";
     char *message = "Welcome \r\n";
    struct sockaddr_un address;
    int toput_message[100];
     int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 15 , activity, i , valread , sd;
    for( i = 0; i< max_clients; i++){
        toput_message[i] = 0;
    }
    
    char buffer[1025]; //data buffer of 1K
    
    //set of socket descriptors
   
    
    //a message
   
    int shouldBreak = 0;

    //initialise all client_socket[] to 0 so not checked
    for (int k = 0; k < 15; k++)
    {
        client_socket[k] = 0;
    }
    
    //create a master socket
    memset(buffer,0,sizeof(buffer));
    if( (master_socket = socket(AF_UNIX , SOCK_STREAM , 0)) == -1)
    {
        //perror("socket failed");
        //printf("1st error");
        exit(EXIT_FAILURE);
    }
    
    
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_path, sizeof(address.sun_path)-1);
    memset(buffer,0,sizeof(buffer));

    unlink(socket_path);
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
       perror("bind");
        shouldBreak = 1;
        printf("second error");
        exit(1);
    }
    
    int continueListen = 1;
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        printf("third error");
        exit(EXIT_FAILURE);
    }
    
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    
    int totNumber = 0;
    int curr_reciept = 0;
    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);
        
        //add master socket to set
        FD_SET(master_socket, &readfds);
        struct sockaddr_un temp ;
        max_sd = master_socket;
        
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];
            
            //if valid socket descriptor then add to read list

            
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
            
            if(sd > 0){
                FD_SET( sd , &readfds);
            }
        }
        
        temp.sun_family = AF_UNIX;
        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        
        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }
        if(shouldBreak == 1){
            break;
        }
        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                //perror("accept");
                continueListen = 0;
                exit(EXIT_FAILURE);
            }
            
            
            printf("New Connection!\n");
            
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                //perror("send");
                continueListen = 0;
            }
            
            puts("Welcome message sent ");
            
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                    continueListen = 1;
                    break;
                }
            }
            
            
            for(int j=0;j<max_clients;j++){
                
                char mess[100]="New user added with user id : \n";
                mess[29]=i+'0';
                send(client_socket[j],mess,strlen(mess),0);
                
                
            }
            
            
        }
        for (i = 0; i < 15; i++)
        {
            for( int io = 0; io< max_clients; io++){
                char *mess = "user is disconnected";
                toput_message[i] = client_socket[i];
            }
            sd = client_socket[i];
            
            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
                                (socklen_t*)&addrlen);
                    printf("Host Disconnected");
                    
                    //Close the socket and mark as 0 in list for reuse

                    client_socket[i] = 0;
                    continueListen = 0;
                    close( sd );
                    temp.sun_family = AF_UNIX;
                    
                }
                
                if(argc > 2){
                    char *updatedsocketpath  = argv[2];
                }
                
                //Echo back the message that came in
                else
                {
                    char text[100];
                    strcpy(text,buffer);
                    
                    char *token = strtok(buffer, "/");
                    int check = 10;
                    if(token [0] != '0'){
                        check = 1;
                    }
                    printf("recieved value is : -%s- and token is -%s-\n",text,token);
                    
                    int n=atoi(token);
                    
                    
                    if(n == 0 && check == 1){
                        char notify[]="Please enter a valid sending mode!\n";
                        continueListen =1;
                        send(sd,notify,strlen(notify),0);
                        continue;
                        
                    }
                    int valuee = strcmp(token,"-1");
                    if(valuee==0){
                        token = strtok(NULL, "-");
                        printf("message is : %s\n",token);
                        text[valread]='\0';
                        buffer[valread] = '\0';
                        
                        continueListen =1;
                        text[1]=">";
                        int x = split(text);
                        text[0]=i+'0';
                        
                        
                        for(int j=0;j<max_clients;j++){
                            if(client_socket[j] == sd){
                                curr_reciept = j;
                                continue;
                            }
                            send(client_socket[j],text,strlen(text),0);
                            
                        }
                    }
                    else{
                        printf("personal message to id : %s\n",buffer);
                        int ret=atoi(buffer);
                        for(int counter = 0; counter < max_clients; counter++){
                            if(counter == curr_reciept){
                                toput_message[counter] = 1;
                            }
                        }
                        token = strtok(NULL, "-");
                        printf("Message is : %s and index is : %i\n",token,ret);
                        buffer[valread] = '\0';
                        int x = split(text);

                        text[1]=">";
                        text[0]=i+'0';
                        
                        
                        text[valread]='\0';
                        
                        for(int j=0;j<max_clients;j++){
                            if(j==ret){
                                totNumber++;
                                printf("%i %i\n",j,client_socket[j]);
                                if(client_socket[j]==0){
                                    char notify[]="User with given id does not exist!";
                                    continueListen =1;
                                    send(sd,notify,strlen(notify),0);
                                }
                                else{
                                    //printf("here2\n",ret);
                                    int x = split(text);
                                    send(client_socket[j],text,strlen(text),0);
                                }
                                break;
                                
                            }
                        }
                        if(continueListen == 0){
                            //printf("closed connection");
                            continueListen = 1;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}

int split(const char *s1){
    int x = (char)s1[0] - '0';
    return x;
}

char* concat(const char *s1, const char *s2 )
{
    testy++;
    char *result = malloc(strlen(s2)+strlen(s1)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

