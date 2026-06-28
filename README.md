# KV Store

A multi-threaded TCP Key-Value Store built in C++ using Winsock.

This project allows multiple clients to connect to a server over TCP and perform database operations such as storing, retrieving, deleting, and persisting key-value pairs. The database supports key expiration (TTL) and file-based persistence.

## Features

* In-memory key-value database using `unordered_map`
* TCP server using Winsock
* TCP client using Winsock
* Multi-client support using `std::thread`
* Key expiration (TTL)
* File persistence
* Concurrent client handling
* Command-based interface

## Supported Commands

| Command            | Description                              |
| ------------------ | ---------------------------------------- |
| SET key value      | Store a key-value pair                   |
| GET key            | Retrieve the value associated with a key |
| DEL key            | Delete a key                             |
| EXISTS key         | Check whether a key exists               |
| EXPIRE key seconds | Set expiration time for a key            |
| TTL key            | Get remaining lifetime of a key          |
| SAVE               | Save the database to disk                |
| LOAD               | Load the database from disk              |
| EXIT               | Disconnect the client                    |

## Architecture

```text
Client
   |
TCP Socket
   |
Server
   |
Database
   |
File Storage
```

## Technologies Used

* C++
* Winsock2
* TCP/IP Networking
* Multithreading (`std::thread`)
* File I/O
* STL Containers (`unordered_map`)
* String Parsing (`stringstream`)

## How It Works

1. The server initializes Winsock and starts listening on port 8080.
2. Clients connect to the server using TCP sockets.
3. Each client connection is handled in a separate thread.
4. Commands received from clients are parsed and executed on the database.
5. Results are sent back to the client.
6. Data can be saved to disk and loaded when required.

## Example Session

```text
SET name praveen
OK

GET name
praveen

EXISTS name
YES

DEL name
OK
```

## Concepts Learned

* TCP Client-Server Architecture
* Socket Programming
* Winsock API
* Multithreading
* Concurrent Client Handling
* File Persistence
* Time-To-Live (TTL) Systems
* Request-Response Communication
* In-Memory Databases

