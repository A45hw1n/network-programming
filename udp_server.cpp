#include <iostream>
#include <string>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 1234
#define BACKLOG 5

using namespace std;

int main()
{
  int sockfd;                                  // Socket file descriptor
  char ipstr[INET6_ADDRSTRLEN];                // Client IP address in human readable format
  char buff[1024];                             // To store recieved message
  struct sockaddr_in server_addr, client_addr; // Server and Client Addresses
  socklen_t client_len;                        // Length of Client Address
  client_len = sizeof client_addr;

  // Initialize the socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  // Socket address structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = PORT;
  server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces (eg, tun0, eth0)

  // Bind local address to socket
  bind(sockfd, (struct sockaddr *)&server_addr, sizeof server_addr);

  // Recieve message from client
  while (1)
  {
    cout << "\nWaiting for message..." << endl;
    recvfrom(sockfd, buff, sizeof buff, 0, (struct sockaddr *)&client_addr, &client_len);

    // Convert client address to readable format to print the IP address
    inet_ntop(AF_INET, &client_addr.sin_addr, ipstr, sizeof ipstr);
    cout << "Message recieved from " << ipstr << ": " << buff;

    // Send response to Client
    char resp[1024] = "\nServer: Message recieved";
    sendto(sockfd, resp, sizeof resp, 0, (struct sockaddr *)&client_addr, client_len);
  }
}