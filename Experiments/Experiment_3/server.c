#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 1. Create socket and bind to port 8080
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);
    printf("Server waiting...\n");

    // 2. Accept client connection
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // 3. Receive number of students N
    int N;
    recv(client_fd, &N, sizeof(N), 0);

    // 4. Receive marks (N students * 3 subjects)
    float marks[N][3];
    recv(client_fd, marks, sizeof(marks), 0);

    // 5. Calculate total, average, and determine grade for each student
    for (int i = 0; i < N; i++) {
        float total = marks[i][0] + marks[i][1] + marks[i][2];
        float avg = total / 3.0;
        char grade;

        if (avg >= 90)      grade = 'A';
        else if (avg >= 80) grade = 'B';
        else if (avg >= 70) grade = 'C';
        else if (avg >= 60) grade = 'D';
        else                grade = 'F';

        // Print results on server side
        printf("Student %d -> Total: %.2f, Average: %.2f, Grade: %c\n", i+1, total, avg, grade);
        
        // Send computed values back (sending total, avg, and grade as a small array/packed info)
        // To keep it simple, we can send total and avg back, or send a custom response back.
        // Let's send back total and average directly.
    }

    // (For absolute simplicity, let's send back computed totals and averages arrays)
    float totals[N], averages[N];
    char grades[N];
    for (int i = 0; i < N; i++) {
        totals[i] = marks[i][0] + marks[i][1] + marks[i][2];
        averages[i] = totals[i] / 3.0;
        grades[i] = (averages[i] >= 90) ? 'A' : (averages[i] >= 80) ? 'B' : (averages[i] >= 70) ? 'C' : (averages[i] >= 60) ? 'D' : 'F';
    }

    send(client_fd, totals, sizeof(totals), 0);
    send(client_fd, averages, sizeof(averages), 0);
    send(client_fd, grades, sizeof(grades), 0);

    close(client_fd);
    close(server_fd);
    return 0;
}