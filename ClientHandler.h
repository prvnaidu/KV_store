#pragma once

#include <winsock2.h>

class Database;

void handleClient(SOCKET clientSocket, Database& db);