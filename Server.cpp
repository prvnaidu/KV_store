#include "Server.h"
#include "Database.h"
#include "ThreadPool.h"
#include "ClientHandler.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;


static const int NUM_WORKER_THREADS = 4;

void Server::start()
{
    WSADATA wsadata;

    int result = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (result != 0)
    {
        cout << "Winsock initialization failed\n";
        return;
    }

    cout << "Winsock initialized successfully\n";

    SOCKET serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed\n";
        return;
    }

    cout << "Socket created successfully\n";

    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int resultBind = bind(
        serverSocket,
        (sockaddr *)&serverAddr,
        sizeof(serverAddr));

    if (resultBind == SOCKET_ERROR)
    {
        cout << "Bind failed\n";
        return;
    }

    cout << "Bind successful\n";

    result = listen(serverSocket, SOMAXCONN);

    if (result == SOCKET_ERROR)
    {
        cout << "Listen failed\n";
        return;
    }

    cout << "Listen successful\n";

    Database db;

    
    ThreadPool pool(NUM_WORKER_THREADS, db);

    while (1)
    {
        SOCKET clientSocket =
            accept(serverSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET)
        {
            cout << "Accept failed\n";
            continue;
        }

        cout << "Client connected\n";


        pool.addClient(clientSocket);
    }
}