#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
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

    struct sockaddr_in server, client;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1)
    {
        perror("socket: ");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(8989);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(&server.sin_zero, 8);

    len = sizeof(struct sockaddr_in);

    if(bind(sock, (struct sockaddr*)&server, len) == -1)
    {
        perror("bind");
        exit(-1);
    }

    if(listen(sock, 10) == -1)
    {
        perror("listen");
        exit(-1);
    }
    cout << "Server is now listening." << endl;

    cout << "\nWaiting for connections......" << endl;

    cli = accept(sock, (struct sockaddr*)&client, &len);

    if(cli == -1)
    {
        perror("accept");
        exit(-1);
    }

    cout << "\nConnection Established : Server to Client[" << cli << "]\n" << endl;

    while(true)
    {
        bzero(message, 1024);

        cout << "Type a message for Client : ";
        scanf("%[^\n]%*c", message);    //Taking String input with space

        sent_bytes = send(cli, message, strlen(message), 0); 

        if(sent_bytes == -1)
        {
            perror("Message can't be sent");
            exit(-1);
        }       
        cout << "Sent " << sent_bytes << " bytes to client: " << inet_ntoa(client.sin_addr) << "\n";
        
        bzero(message, 1024);

        int valread = recv(cli , message, 1024, 0);

        if(valread == -1)
        {
            perror("Message can't be read");
            exit(-1);
        }
        printf("\nClient %d says : %s \n", cli, message );

        string check(message);
        if (check == "Bye" || check == "bye")
        {
            cout << "\n<<<<---- Closing connection ---->>>>" << endl;
            break;
        }
    }
    close(cli);
    close(sock);
}