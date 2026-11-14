#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

// Simple function to check if a number is prime
int is_prime(int num) {
    if (num <= 1) return 0; // 0 and 1 are not prime
    for (int i = 2; i < num; i++) {
        if (num % i == 0) return 0; // Divisible by another number
    }
    return 1; // It is prime
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 1. Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Setup server address & port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept any local IP
    address.sin_port = htons(8080);       // Port 8080

    // 3. Bind socket to port and start listening
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);
    printf("Server waiting for client...\n");

    // 4. Accept connection from client
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // 5. Receive matrix size N
    int N;
    recv(client_fd, &N, sizeof(N), 0);

    // 6. Receive N*N matrix elements
    int total_elements = N * N;
    int matrix[total_elements];
    recv(client_fd, matrix, sizeof(matrix), 0);

    // 7. Count even, odd, prime, and non-prime numbers
    int counts[4] = {0, 0, 0, 0}; // [0]=even, [1]=odd, [2]=prime, [3]=non-prime

    for (int i = 0; i < total_elements; i++) {
        int val = matrix[i];

        // Even or Odd
        if (val % 2 == 0) counts[0]++;
        else              counts[1]++;

        // Prime or Non-Prime
        if (is_prime(val)) counts[2]++;
        else               counts[3]++;
    }

    // 8. Send the 4 counts back to client
    send(client_fd, counts, sizeof(counts), 0);

    // 9. Close connections
    close(client_fd);
    close(server_fd);
    return 0;
}