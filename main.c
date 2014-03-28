#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6000
#define MAX_CLIENTS 64
#define BUFSIZE 8096

struct mime_info
{
    char *ext;
    char *filetype;
};

struct mime_info mime_info_map[] =
{
    {"html","text/html"},
    {0,0}
};

void send_error(int status_code)
{

}

void web_handler(int client_socket)
{
    printf("web_handler");

    int j, file_fd, buflen;
    long i, ret, len;
    char * fstr;

    static char buffer[BUFSIZE + 1];

    ret = read(client_socket, buffer, BUFSIZE);

    if(ret <= 0)
        send_error(400);
    else if (ret < BUFSIZE)
        buffer[ret] = 0;
    else
        buffer[0] = 0;

    if(strncmp(buffer,"GET ", 4))
        send_error(500);

    for(i=4; i<BUFSIZE; i++)
    {
        if(buffer[i] == ' ')
        {
            buffer[i] = 0;
            break;
        }
    }

    for(j=0;j<i-1;j++)
        if(buffer[j] == '.' && buffer[j+1] == '.')
            send_error(403);

    if(!strncmp(&buffer[0],"GET /\0", 6))
        (void)strcpy(buffer,"GET /index.html");

    buflen=strlen(buffer);
    fstr = (char *)0;
    for(i=0;mime_info_map[i].ext != 0;i++) {
        len = strlen(mime_info_map[i].ext);
        if( !strncmp(&buffer[buflen-len], mime_info_map[i].ext, len)) {
            fstr =mime_info_map[i].filetype;
            break;
        }
    }

    if(fstr == 0)
        send_error(500);

    if(( file_fd = open(&buffer[5],O_RDONLY)) == -1)
        send_error(500);

    len = (long)lseek(file_fd, (off_t)0, SEEK_END);
    lseek(file_fd, (off_t)0, SEEK_SET);
    sprintf(buffer,"HTTP/1.1 200 OK\nServer: TeaPot\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", len, fstr);
    write(client_socket, buffer, strlen(buffer));

    while ( (ret = read(file_fd, buffer, BUFSIZE)) > 0 ) {
        (void)write(client_socket,buffer,ret);
    }

    sleep(1);
    close(client_socket);
    exit(1);
}

void main_loop(int server_socket)
{
    int client_socket, pid;
    socklen_t client_address_length;

    static struct sockaddr_in client_address;

    while(1)
    {
        client_address_length = sizeof(client_address);

        if((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length)) < 0)
            printf("Error when accept()");

        if((pid = fork()) < 0) {
            printf("Error when fork()");
        }
        else
        {
            if(pid == 0)
            {
                close(server_socket);
                web_handler(client_socket);
            }
            else
            {
                close(client_socket);
            }
        }
    }
}

int main(int argc, char **argv)
{
    int server_socket;

    static struct sockaddr_in server_address;

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Error when socket()");

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) <0)
        printf("Error when bind()");

    if(listen(server_socket, MAX_CLIENTS) < 0)
        printf("Error when listen()");

    printf("TeaPot Start...");

    main_loop(server_socket);
}
