#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<strings.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<signal.h>
#include <fcntl.h>
#define MAXLINE 100
void doit(int);
int main(int argc,char *argv[])
{
    pid_t pid;
    int listenfd,connfd,n;
    int file_index;
    char buf[MAXLINE];
    struct sockaddr_in servaddr,cliaddr;
    socklen_t cliaddr_len;  
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    //server socket init
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(8000);
    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    listen(listenfd,20);
    printf("Waiting for message coming from our client ...\n");
    
    while(1)//complicating
    {
        cliaddr_len=sizeof(cliaddr);
        connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
        while((n=read(connfd,buf,MAXLINE)>0))
        {
            bzero(buf,MAXLINE);
            strcpy(buf,"HTTP/1.1 200 OK\r\n");
            write(connfd,buf,strlen(buf));
            bzero(buf,MAXLINE);
            strcpy(buf,"Content-Type:text/html\r\n");
            write(connfd,buf,strlen(buf));
            bzero(buf,MAXLINE);
            buf[0]='\r';
			buf[1]='\n';
            write(connfd,buf,strlen(buf));
            bzero(buf,MAXLINE);
            file_index=open("/index.html",O_RDONLY);//open index file return the file description
            ssize_t read_n;
            read_n=read(file_index,buf,MAXLINE);
            write(connfd,buf,MAXLINE);
        }
        close(file_index);
        close(connfd);//parent close connfd
    }
}

