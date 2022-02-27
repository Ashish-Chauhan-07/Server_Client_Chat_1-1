#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace std;

int main()
{
    int sock, cli;
    int sent_bytes;
    unsigned int len;
    char message[1024];

    struct sockaddr_in server_add;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1)
    {
        perror("socket: ");
        exit(-1);
    }

    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(8989);

    if(inet_pton(AF_INET, "127.0.0.1", &server_add.sin_addr) == -1) 
    {
        printf("\nInvalid Address / Address not supported.\n");
        exit(-1);
    }

    if (connect(sock, (struct sockaddr *)&server_add, sizeof(server_add)) == -1)
    {
        printf("\nConnection Failed.\n");
        exit(-1);
    }

    cout << "Connected to Server.. \nWaiting for response..\n"; 

    while(true)
    {
        bzero(message, 1024);
        int valread = recv(sock , message, 1024, 0);
        printf("\nMessage from Server : %s\n", message );

        bzero(message, 1024);
        cout << "Type a message for server : ";
        scanf("%[^\n]%*c", message);    //Taking String input with space
        sent_bytes = send(sock, message, strlen(message), 0);
        cout << "Message Sent.." << endl;

        string check(message);
        if (check == "Bye" || check == "bye")
        {
            cout << "\n<<<<---- Exiting ---->>>>" << endl;
            break;
        }
    }
    close(sock);
}