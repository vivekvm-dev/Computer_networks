#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    int N;

    // 1. Get input from user
    printf("Enter number of students (N): ");
    scanf("%d", &N);

    float marks[N][3];
    for (int i = 0; i < N; i++) {
        printf("Enter marks in 3 subjects for student %d: ", i + 1);
        scanf("%f %f %f", &marks[i][0], &marks[i][1], &marks[i][2]);
    }

    // 2. Connect to server
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // 3. Send data to server
    send(sock, &N, sizeof(N), 0);
    send(sock, marks, sizeof(marks), 0);

    // 4. Receive results from server
    float totals[N], averages[N];
    char grades[N];
    recv(sock, totals, sizeof(totals), 0);
    recv(sock, averages, sizeof(averages), 0);
    recv(sock, grades, sizeof(grades), 0);

    // 5. Display results
    printf("\n--- Results from Server ---\n");
    for (int i = 0; i < N; i++) {
        printf("Student %d -> Total: %.2f | Average: %.2f | Grade: %c\n", 
               i + 1, totals[i], averages[i], grades[i]);
    }

    close(sock);
    return 0;
}