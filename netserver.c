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
    FILE * f;
    FILE * o;
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
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
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
                if(bytes_read <= 0) break;
                const char *b=buffer;
                f = popen(b, "r");
                if (f == NULL)
                {
                    perror("ошибка:\n");
                    return -1;
                }
                o = fopen("log.txt", "w");
                while ((len = fread(buf, 1, 1024, f)) != 0)
                {
                     
                     fwrite(buf, 1, len, o);
                }
                
                send(sock, buf, 1024, 0);
                pclose(f);
                fclose(o);
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
