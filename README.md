# Client/Server FTP

## Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Usage](#usage)

## About <a name = "about"></a>

This was my final project for CS371 Network Programming that provides remote access from a client to a server for sending files one way.

By default this server/client relationship uses port 8080, but can be changed by the user with the define definition on line 9.

## Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Installing

Compile both server.cpp and client.cpp with the following:


```
g++ server.cpp -o server
g++ client.cpp -o client
```

## Usage <a name = "usage"></a>

You can start the server on any machine by running:
```
./server
```

You can then send files to that instance by running:
```
./client <ip-address of server> <directory of file>

Ex. ./client 127.0.0.1 test.txt
```
Note: If the file is in the root directory of the client executable you do not have to use the full path. Otherwise you must use the full path of the file to be sent.
