#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define SOCK_PATH "echo_socket"

int ThreadDead = 0;
int count = 0;
void *myThread(void *s)
{
    char str[100];
    char Bufer[1024];
    while(printf(""), fgets(str, 100, stdin), !feof(stdin)) {
        //printf("%s", str);
        if(ThreadDead == 0){
            //printf("stop");
        }
        if (send(s, str, strlen(str), 0) == -1) {
            perror("send");
            ThreadDead = 1;
            exit(1);
        }
        
    }
    
    return NULL;
}


int main(int argc , char*argv[])
{
    
    struct sockaddr_un remote, address;
    char str[100];
    int s, t, len;
    char *socket_path = "/tmp/foo";
    if(argc >1){
        printf("more inputs than needed");
        exit(1);
    }
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    if(ThreadDead == 0){
        count++;
        //printf("%d", count);
    }
    if(ThreadDead == 1){
        //printf("%d", count);
    }
    
    memset(&remote, 0, sizeof(remote));
    printf("Trying to connect...\n");
    address.sun_family  = PF_UNIX;
    remote.sun_family = AF_UNIX;
    strncpy(remote.sun_path, socket_path, sizeof(remote.sun_path)-1);
    if (connect(s, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
        perror("connect");
        exit(1);
    }
    
    int *i = malloc(sizeof(*i));
    i=s;
    printf("Connected.\n");
    pthread_t tid;

    pthread_create(&tid, NULL, myThread, (void *)i);
    int numberofInputs = 0, numberOfCyles = 0;
    while(1) {
        if ((t=recv(s, str, 100, 0)) > 0) {
            str[t] = '\0';
            numberofInputs++;
            printf(">> %s", str);
        } else {
            //printf ("%d", numberofInputs);
            if (t < 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }
        numberOfCyles++;
    }
    
    close(s);
    
    return 0;
}



