// This code is a simple TCP client that connects to an ESP32 server, sends a message, and waits for a response.
// It handles the connection, sending, and receiving of messages.
// It also includes error handling for socket creation, connection, and message sending/receiving.
// This code is intended to be run on a Linux system with the necessary libraries installed.
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 8080           // Port number for the server
#define SERVER_IP "192.168.100.67" // IP address of the server (ESP32)
// Note: Make sure to replace the IP address with the actual IP address of your ESP32 server.

using namespace std;

// Global variables
int clientSocket = 0;

// Function prototypes
static int prepareSocket();
static uint8_t sendMessageToESP32(string message, int sock);

int main() {

    if (prepareSocket() < 0) {
        return -1;
    }

    cout << "Waiting for messages from ESP32..." << endl;
    while (true) 
    {
        string request;
        static string lastRequest;
        char ch;
        ssize_t result;

        // Read one request (ending with '\n') one character at a time
        // This is a blocking call, so it will wait for data to arrive
        while ((result = recv(clientSocket, &ch, 1, 0)) > 0) 
        {
            if (ch == '\n') break;
            request += ch;
        }

        if (result == 0) 
        {
            cout << "ESP32 closed the connection." << endl;
            break;
        } 
        else if (result < 0) 
        {
            perror("recv");  // Timeout or error
            continue;        // Retry
        }

        request.erase(request.find_last_not_of(" \r\n\t") + 1);       

        // send requests only on transition from switch
        if (request == "0" && lastRequest == "1")
        {
            cout<< "Hello World!"<< endl;
            sendMessageToESP32("LED_ON\n", clientSocket);
        }
        else if (request == "1" && lastRequest == "0")
        {
            cout << "Good Bye World!"<< endl;
            sendMessageToESP32("LED_OFF\n", clientSocket);
            break; // Exit the loop
        }

        lastRequest = request; // Store the last request      
    }

    close(clientSocket);
    return 0;
}

// Function to prepare the socket for communication
// It creates a socket, sets up the server address, and connects to the server.
// Returns 0 on success, -1 on failure
static int prepareSocket()
{
    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) 
    {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    // Set up server address
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connect to server
    if (connect(clientSocket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        cerr << "Connection failed" << endl;
        close(clientSocket);
        return -1;
    }

    // Set a timeout of 5 seconds on recv()
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    return 0;
}

// Function to send a message to the ESP32 server
// It takes a message string and the socket file descriptor as parameters.
// It sends the message to the server and handles any errors.
// Returns 0 on success, 1 on failure
static uint8_t sendMessageToESP32(string message, int sock)
{
    ssize_t sent = send(sock, message.c_str(), message.size(), 0);
    if (sent < 0) 
    {
        perror("send failed");
        return 1;
    } 
    else 
    {
        return 0;
    }
}