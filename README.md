# Multi-threaded TCP Key-Value Store in C++

A Redis-inspired key-value database built from scratch in C++.

This project implements a custom TCP client-server architecture with a thread pool, concurrent request handling, an in-memory database engine, TTL-based expiration, and file persistence.

The goal of this project was to understand how real-world backend systems handle networking, concurrency, request processing, and data storage.

---

# Features

## TCP Client-Server Communication

* Built using Windows Winsock API
* Custom TCP-based request-response communication
* Supports multiple simultaneous client connections
* Implements a lightweight text-based command protocol

---

## In-Memory Key-Value Database

The database engine uses hash-based storage:

* Fast key-value lookup using `unordered_map`
* Supports basic database operations
* Thread-safe access using mutex synchronization

Supported commands:

| Command              | Description                  |
| -------------------- | ---------------------------- |
| `SET key value`      | Store a key-value pair       |
| `GET key`            | Retrieve a value             |
| `DEL key`            | Delete a key                 |
| `EXISTS key`         | Check whether a key exists   |
| `EXPIRE key seconds` | Add expiration time to a key |
| `TTL key`            | Get remaining lifetime       |
| `SAVE`               | Persist database to disk     |
| `LOAD`               | Restore database from disk   |
| `EXIT`               | Close client connection      |

---

# System Architecture

```
                    Client
                      |
                      |
                 TCP Connection
                      |
                      |
              Winsock TCP Server
                      |
                      |
                   accept()
                      |
                      |
              ThreadPool Queue
                      |
        --------------------------------
        |              |               |
     Worker 1       Worker 2        Worker 3
        |              |               |
        --------------------------------
                      |
                      |
              Client Handler
                      |
                      |
                 Database Engine
                      |
          --------------------------
          |                        |
    Key-Value Store          TTL Manager
          |
          |
      data.txt
    Persistence Layer
```

---

# Thread Pool Design

The server uses a custom fixed-size thread pool.

Instead of creating a new thread for every client connection:

* Worker threads are created once during server startup.
* Incoming client sockets are placed into a shared queue.
* Available workers process connections from the queue.

Architecture:

```
Client Connection
        |
        v
 Client Queue
        |
        v
 Worker Thread
        |
        v
 Client Handler
```

Benefits:

* Avoids repeated thread creation overhead
* Allows concurrent client handling
* Provides controlled resource usage

---

# Concurrency Handling

The project demonstrates practical multithreading concepts:

## Mutex Protection

Shared resources are protected using mutex locks.

Protected components:

* Client connection queue
* Database storage
* Persistence operations

This prevents race conditions when multiple worker threads access shared data.

---

## Condition Variable Synchronization

Worker threads do not continuously poll the queue.

Instead:

* Workers sleep when no requests are available.
* New client connections wake waiting workers.
* Shutdown signals wake all workers.

This follows the producer-consumer design pattern.

---

# Graceful ThreadPool Shutdown

The ThreadPool supports clean worker termination.

Shutdown process:

```
Server exits
      |
      v
ThreadPool destructor
      |
      v
stop flag enabled
      |
      v
Workers notified
      |
      v
Worker threads exit
      |
      v
Threads joined safely
```

This prevents:

* orphan threads
* premature termination
* resource leaks

---

# TTL Expiration System

The database supports key expiration.

Example:

```
EXPIRE session 60
```

Internally:

* Expiration timestamps are stored separately.
* Requests check whether keys have expired.
* Expired keys are removed automatically during access.

Supported:

```
EXPIRE key seconds
TTL key
```

---

# Persistence

The database supports saving and loading data.

## SAVE

Stores database contents into:

```
data.txt
```

The file stores:

```
key value expiration_time
```

## LOAD

Restores stored data back into memory.

This provides basic durability beyond the lifetime of the server process.

---

# Request Processing Flow

Example:

Client:

```
SET username praveen
```

Flow:

```
Client
 |
TCP Socket
 |
Server accept()
 |
ThreadPool Queue
 |
Worker Thread
 |
ClientHandler
 |
Database.set()
 |
Response sent back
```

Response:

```
OK
```

---

# Technologies Used

* C++
* Winsock API
* TCP/IP Networking
* Multithreading
* std::thread
* std::mutex
* std::condition_variable
* STL Containers
* File I/O

---

# Project Structure

```
KV_store/

├── Server.cpp
├── Server.h

├── Client.cpp

├── ThreadPool.cpp
├── ThreadPool.h

├── ClientHandler.cpp
├── ClientHandler.h

├── Database.cpp
├── Database.h

├── data.txt
└── main.cpp
```

---

# Build Instructions

Compile the server:

```bash
g++ -std=c++17 Server.cpp ThreadPool.cpp ClientHandler.cpp Database.cpp main.cpp -o server.exe -lws2_32
```

Compile the client:

```bash
g++ -std=c++17 Client.cpp -o client.exe -lws2_32
```

---

# Running the Project

Start the server:

```bash
./server.exe
```

Start the client:

```bash
./client.exe
```

Example:

```
Enter command: SET name praveen


SERVER: OK


Enter command: GET name

SERVER: praveen
```

---

# Concepts Demonstrated

This project helped implement and understand:

* TCP socket programming
* Client-server architecture
* Network communication
* Thread pools
* Producer-consumer pattern
* Thread synchronization
* Mutex locking
* Condition variables
* Resource cleanup using destructors
* Database design basics
* Persistence mechanisms

---
