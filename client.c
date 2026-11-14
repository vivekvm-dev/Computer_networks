#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    int N;

    // 1. Get matrix size from user
    printf("Enter matrix size N: ");
    scanf("%d", &N);

    int total_elements = N * N;
    int matrix[total_elements];

    // 2. Get matrix elements from user
    printf("Enter %d elements for the %dx%d matrix:\n", total_elements, N, N);
    for (int i = 0; i < total_elements; i++) {
        scanf("%d", &matrix[i]);
    }

    // 3. Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 4. Setup server address (localhost: 127.0.0.1, Port: 8080)
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 5. Connect to the server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // 6. Send matrix size N, then send the matrix elements
    send(sock, &N, sizeof(N), 0);
    send(sock, matrix, sizeof(matrix), 0);

    // 7. Receive results from server
    int counts[4];
    recv(sock, counts, sizeof(counts), 0);

    // 8. Display the output
    printf("\n--- Results ---\n");
    printf("Even elements     : %d\n", counts[0]);
    printf("Odd elements      : %d\n", counts[1]);
    printf("Prime numbers     : %d\n", counts[2]);
    printf("Non-prime numbers : %d\n", counts[3]);

    // 9. Close socket
    close(sock);
    return 0;
}