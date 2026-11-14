#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    char sentence[1024];

    // 1. Get a full sentence (including spaces) from the user
    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);
    sentence[strcspn(sentence, "\n")] = '\0'; // Remove trailing newline

    // 2. Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 3. Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 4. Send sentence to server
    sendto(sockfd, sentence, strlen(sentence), 0, 
           (struct sockaddr *)&server_addr, addr_len);

    // 5. Receive counts from server
    int counts[5];
    recvfrom(sockfd, counts, sizeof(counts), 0, 
             (struct sockaddr *)&server_addr, &addr_len);

    // 6. Display the results
    printf("\n--- Sentence Analysis ---\n");
    printf("Words              : %d\n", counts[0]);
    printf("Vowels             : %d\n", counts[1]);
    printf("Consonants         : %d\n", counts[2]);
    printf("Digits             : %d\n", counts[3]);
    printf("Special characters : %d\n", counts[4]);

    // 7. Close socket
    close(sockfd);
    return 0;
}