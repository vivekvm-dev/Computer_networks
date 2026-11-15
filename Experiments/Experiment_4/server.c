#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 1. Create UDP socket and bind
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("UDP Calculator Server ready...\n");

    // 2. Receive numbers and operator values separately (simplest approach)
    double num1, num2, result = 0;
    char op;

    recvfrom(sockfd, &num1, sizeof(num1), 0, (struct sockaddr *)&client_addr, &addr_len);
    recvfrom(sockfd, &num2, sizeof(num2), 0, (struct sockaddr *)&client_addr, &addr_len);
    recvfrom(sockfd, &op, sizeof(op), 0, (struct sockaddr *)&client_addr, &addr_len);

    // 3. Perform calculation
    if (op == '+')      result = num1 + num2;
    else if (op == '-') result = num1 - num2;
    else if (op == '*') result = num1 * num2;
    else if (op == '/') result = (num2 != 0) ? (num1 / num2) : 0;

    // 4. Send result back to client
    sendto(sockfd, &result, sizeof(result), 0, (struct sockaddr *)&client_addr, addr_len);

    close(sockfd);
    return 0;
}