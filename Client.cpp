#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main()
{
    WSADATA wsadata;

    int result = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (result != 0)
    {
        cout << "Winsock initialization failed\n";
        return 1;
    }

    cout << "Winsock initialized successfully\n";

    SOCKET clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET)
    {
        cout << "Client socket creation failed\n";
        return 1;
    }

    cout << "Client socket created successfully\n";

    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    result = connect(
        clientSocket,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr));

    if (result == SOCKET_ERROR)
    {
        cout << "Connection failed\n";
        return 1;
    }

    cout << "Connected to server\n";

    while (1)
    {
        string command;

        cout << "Enter command: ";
        getline(cin, command);

        send(
            clientSocket,
            command.c_str(),
            command.size(),
            0);

        char responseBuffer[1025];

        int bytesReceived = recv(
            clientSocket,
            responseBuffer,
            1024,
            0);

        if (bytesReceived <= 0)
        {
            cout << "Server disconnected\n";
            break;
        }

        responseBuffer[bytesReceived] = '\0';

        cout << "SERVER: " << responseBuffer << "\n";

        if (command == "EXIT")
        {
            break;
        }
    }

    return 0;
}