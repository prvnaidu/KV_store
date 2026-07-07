#include "ClientHandler.h"
#include "Database.h"

#include <iostream>
#include <sstream>

using namespace std;
void handleClient(SOCKET clientSocket, Database &db)
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
