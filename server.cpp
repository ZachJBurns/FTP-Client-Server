// REFERENCE https://www.geeksforgeeks.org/socket-programming-cc/
// REFERENCE: http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#define PORT 8080
using namespace std;

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed!");
        exit(EXIT_FAILURE);
    }

    // Setting the sock options on network.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Could not set sock options");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Could not bind socket to port!");
        exit(EXIT_FAILURE);
    }
    // Listening on network for connections
    if (listen(server_fd, 1) < 0)
    {
        perror("Could not listen on network!");
        exit(EXIT_FAILURE);
    }

    cout << "Server Started....." << endl;

    // Will run forever until force quit.
    while (1)
    {
        char buffer[1024];
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        cout << "Client Connected....." << endl;

        // Reads data sent from client. Adds null terminating character to end of buffer.
        valread = read(new_socket, buffer, sizeof(buffer));
        buffer[strlen(buffer)] = '\0';

        cout << "File Received....." << endl;
        char *fname;
        char *newbuf;

        // Loops through and finds the first new line character
        // The text before this newline represents the filename on the client side.
        for (int i = 0; i < strlen(buffer); i++)
        {
            if (buffer[i] == '\n')
            {
                fname = (char *)malloc(sizeof(char) * i + 1); // Allocates space for file name.
                // Copies filename sent from client into variable and adds null terminating character to end.
                strncpy(fname, buffer, i);
                fname[strlen(fname)] = '\0';

                newbuf = (char *)malloc(sizeof(char) * (sizeof(buffer) - i) + 1); // Allocates space for file content.
                // Copies file data sent from client into variable and adds null terminating character to end.
                // breaks from loop after that.
                strncpy(newbuf, &buffer[i] + 1, sizeof(buffer) - i);
                newbuf[strlen(newbuf)] = '\0';
                break;
            }
        }

        // Creates a file output for writing to.
        FILE *fd = fopen(fname, "w");
        if (fd == NULL)
            cout << "Could not open file for writing....." << endl;
        
        // Writes buffer to file and closes file.
        fprintf(fd, "%s", newbuf);
        fclose(fd);

        // Deallocates all memory used for storing data for next connection.
        delete[] fname;
        delete[] newbuf;
        bzero(buffer, sizeof(buffer));

        close(new_socket); // Closes the socket.

        cout << "Connection Ended....." << endl;
    }
    return 0;
}

