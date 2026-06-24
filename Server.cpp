#include "Server.h"
#include "Database.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <thread>
#include <functional>

using namespace std;

void handleClient(SOCKET clientSocket, Database& db)
{
    while (1)
    {
        char buffer[1025];

        int bytesReceived = recv(
            clientSocket,
            buffer,
            1024,
            0);

        if (bytesReceived <= 0)
        {
            cout << "Client disconnected\n";
            break;
        }

        buffer[bytesReceived] = '\0';

        string input = buffer;

        stringstream commandStream(input);

        string operation;
        string key;
        string value;

        commandStream >> operation;
        commandStream >> key;
        commandStream >> value;

        string response;

        if (operation == "EXIT")
        {
            response = "BYE";
        }
        else if (operation == "SET")
        {
            db.set(key, value);
            response = "OK";
        }
        else if (operation == "GET")
        {
            response = db.get(key);
        }
        else if (operation == "EXISTS")
        {
            response = db.exists(key) ? "YES" : "NO";
        }
        else if (operation == "DEL")
        {
            response = db.del(key) ? "OK" : "Key not found";
        }
        else if (operation == "EXPIRE")
        {
            db.expire(key, stoi(value));
            response = "OK";
        }
        else if (operation == "TTL")
        {
            response = to_string(db.ttl(key));
        }
        else if (operation == "SAVE")
        {
            db.save();
            response = "SAVED";
        }
        else if (operation == "LOAD")
        {
            db.load();
            response = "LOADED";
        }

        send(
            clientSocket,
            response.c_str(),
            response.size(),
            0);

        if (response == "BYE")
        {
            break;
        }
    }
}

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
        (sockaddr*)&serverAddr,
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

        thread clientThread(
            handleClient,
            clientSocket,
            std::ref(db));

        clientThread.detach();
    }
}