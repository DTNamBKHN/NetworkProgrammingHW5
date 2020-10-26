#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <errno.h>
#define BUFLEN 512	//Max length of buffer
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi toi

    char sendbuff[BUFLEN];
    char recvbuff[BUFLEN];

    //Client nhan tham so hostname va port tu dong lenh
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]); //Chuyen cong dich vu thanh so nguyen
   
   //Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
      
    
    memset(&serv_addr, '0', sizeof(serv_addr));

    //Thiet lap dia chi cua server de ket noi den
    serv_addr.sin_family = AF_INET;        //Mac dinh
    serv_addr.sin_port = htons(portno);    //Cong dich vu
   
    //Dia chi ip/domain may chu
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    //Goi ham connect de thuc hien mot ket noi den server
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }
    while(1){
        printf("Please enter the message: ");
    
        memset(sendbuff, '\0', BUFLEN); //Khoi tao buffer
        fflush(stdin);
        fgets(sendbuff,BUFLEN,stdin); //Chua thong diep doc tu ban phim 
        if (sendbuff[0] == '\n'){
            //send data to server
            sendbuff[1] = '\0';
            n = write(sockfd,sendbuff,strlen(sendbuff));
            if (n < 0) 
                error("ERROR writing to socket");
            
            //Nhan du lieu tu server
            memset(recvbuff, '\0', BUFLEN);
            n = read(sockfd,recvbuff,BUFLEN);
            if (n < 0) 
                error("ERROR reading from socket");
            if(strcmp(recvbuff, "Closing...") == 0){
                printf("%s\n",recvbuff);
                break;
            }
        }
        else{
            sendbuff[strlen(sendbuff) - 1] = '\0';
            //send data to server
            n = write(sockfd,sendbuff,strlen(sendbuff));
            if (n < 0) 
                error("ERROR writing to socket");
            
            //Nhan du lieu tu server
            memset(recvbuff, '\0', BUFLEN);
            n = read(sockfd,recvbuff,BUFLEN);
            if (n < 0) 
                error("ERROR reading from socket");
            if(strcmp(recvbuff, "Closing...") == 0)
                break;
            printf("%s\n",recvbuff);
        }
    }

    close(sockfd); //Dong socket 
    return 0;
}