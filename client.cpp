// REFERENCE: https://www.geeksforgeeks.org/socket-programming-cc/
// REFERENCE: http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <iostream>
#include <string.h>

#define PORT 8080 
using namespace std;

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr;  
    char fdata[1024] = {0};

    if (argc != 3){
        cout << "Not enough arguments given" << endl;
        cout << "Please enter in the form ./client <ip address> <file name>" << endl;
        return 0;
    }

    // Attemps to open client file. Returns if cant be open.
    FILE *fd = fopen(argv[2], "r");
    if (fd == NULL) {
        cout << "File could not be opened!" << endl;
        return 0;
    }

    // Attempts to create a socket.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

    // Attempts to connect to ip address.
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    cout << "Connected to server...." << endl;

    // Locates the first / in the directory file name.
    // This is used to save the file as the same name the client has.
    int delimiter = 0;
    for (int i = strlen(argv[2]) -1; i >= 0; i--){
        if(argv[2][i] == '/'){
            delimiter = i+1;
            break;
        }
    }

    // Allocates room for the file name and copies it to a variable.
    char fname[strlen(argv[2])-delimiter+2];
    strcpy(fname, &argv[2][delimiter]);
    strcat(fname, "\n");

    // Reads from user file and stores it into buffer.
    // The file name is also stored here with a newline.
    while (!feof(fd)) {
        if ((fread(&fdata, sizeof(char), sizeof(fdata), fd)) > 0){
            char buffer[strlen(fdata)];
            buffer[0] = '\0';
            strcat(buffer, fname);
            strcat(buffer, fdata);
            buffer[strlen(buffer)] = '\0';
            send(sock, buffer, strlen(buffer) , 0); // Sends the file data to the server.
            cout << "File Sent....." << endl;
        }
        else
            break;
}
    // Closes the connection.
    fclose(fd);
    cout << "Connection Ended....." << endl;
    return 0; 
} 
