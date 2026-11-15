#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    double num1, num2;
    char op;

    // 1. Get input from user
    printf("Enter first number: ");
    scanf("%lf", &num1);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &op); // Space before %c handles newline buffer
    printf("Enter second number: ");
    scanf("%lf", &num2);

    // 2. Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Send values one by one to server
    sendto(sockfd, &num1, sizeof(num1), 0, (struct sockaddr *)&server_addr, addr_len);
    sendto(sockfd, &num2, sizeof(num2), 0, (struct sockaddr *)&server_addr, addr_len);
    sendto(sockfd, &op, sizeof(op), 0, (struct sockaddr *)&server_addr, addr_len);

    // 4. Receive result from server
    double result;
    recvfrom(sockfd, &result, sizeof(result), 0, (struct sockaddr *)&server_addr, &addr_len);

    // 5. Display result
    printf("Result from server: %.2lf\n", result);

    close(sockfd);
    return 0;
}