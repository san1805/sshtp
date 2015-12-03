#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <netdb.h> 
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FIFO_NAME "./fifofile" 

int main()
{
    int len;
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    char buffer[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    

    listen(listener, 1);
    
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        
        switch(fork())
        {
        case -1:
            perror("fork");
            break;
            
        case 0:
            close(listener);
            while(1)
            {
                bytes_read = recv(sock, buffer, 1024, 0);
                
                
                send(sock, buf, 1024, 0);
                close(sock);
                _exit(0);
            }

            
            
        default:
            close(sock);
        }
    }
    
    close(listener);

    return 0;
}
