#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define HTTP_PORT 6000
#define MAX_CONNECTIONS 100

int main(int argc, char** argv)
{
    int server_socket;
    int client_socket;
    int client_address_size;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    
    char client_head_buf[10240];
    
    const char hello[] = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\nContent-Type: text/plant\r\n\r\nHello World";
    
    printf("coffeepot start.\n");

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(server_socket <= 0)
    {
        printf("create server socket failed.\n");
        exit(1);
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(HTTP_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(server_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr)) == -1)
    {
        printf("bind failed.\n");
        exit(1);
    }
    
    if(listen(server_socket, MAX_CONNECTIONS) == -1)
    {
        printf("listen failed.\n");
        exit(1);
    }
    
    while(1)
    {
        client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        
        if(client_socket == -1)
        {
            printf("accept failed.\n");
            continue;
        }
        
        if(recv(client_socket, (void*)client_head_buf, 10240, 0) == -1)
        {
                printf("recv failed.\n");
                continue;
        }
        
        if(send(client_socket, hello, sizeof(hello), 0) == -1)
        {
            printf("send Faild.\n");
            continue;
        }
        
        close(client_socket);
    }
    
    close(server_socket);

    return 0;
}
